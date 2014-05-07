#include "MVM.h"

EquivalenceClass::EquivalenceClass()
{
	this->_totalCount = 0;
}

bool EquivalenceClass::Included(int index)
{
	bool result = false;
	if(this->_inclusionMap.count(index))
	{
		result = true;
	}
	return result;
}

int EquivalenceClass::Add(int index, std::string class_value)
{
	this->_inclusionMap[index] = true;
	if(this->_classCounts.count(class_value))
	{
		this->_classCounts[class_value]++;
	}
	else 
	{
		this->_classCounts[class_value] = 1;
	}
	this->_totalCount++;

	return 0;
}

double EquivalenceClass::Ap(std::string class_value)
{
	return (this->_classCounts.count(class_value) ? this->_classCounts[class_value] : 0)/ ((double) this->_totalCount);
}


int GetAttributes(DATA_SET & data, std::list<std::string> & attributes, std::string class_col)
{
	for(auto it = data.begin(); it != data.end(); ++it)
	{
		if(it->first != class_col)
		{
			attributes.push_back(it->first);
		}
	}
	return 0;
}

int FindClasses(DATA_SET & data, std::string class_column, std::vector<std::string> & classes)
{
	std::unordered_map<std::string, bool> discovered;
	int numrows = data[class_column].size();
	int i = 0;
	for(; i < numrows; i++)
	{
		std::string value = data[class_column][i];
		if(!discovered.count(value))
		{
			classes.push_back(value);
			discovered[value] = true;
		}
	}
	return 0;
}

int FindEquivalences(DATA_SET & data, std::vector<std::pair<std::string, EquivalenceClass *>> & equivalence_assoc, std::unordered_map<std::string, EquivalenceClass> & equivalences, std::string class_column, std::vector<std::string> & attributes) 
{
	int numrows = data[class_column].size();
	int i;
	std::pair<std::string, EquivalenceClass *> dummy;
	if(!attributes.empty())
	{
		for(i = 0; i < numrows; i++)
		{

			std::string class_value = data[class_column][i];
			std::string attribute_string;

			for(auto it = attributes.begin(); it != attributes.end(); ++it)
			{
				attribute_string += data[*it][i];
			}
			EquivalenceClass & ec = equivalences[attribute_string];
			ec.Add(i, class_value);
			dummy.first = class_value;
			dummy.second = &ec;
			equivalence_assoc.push_back(dummy);
		}
	}
	return 0;
}

double Sigma_p(DATA_SET & data, std::vector<std::pair<std::string, EquivalenceClass *>> & equivalences, std::string class_value, int num_classes)
{
	int i = 0;
	double sum_same = 0;
	double sum_different = 0;

	for(auto it = equivalences.begin(); it != equivalences.end(); ++it)
	{
		std::string & current_class = it->first;
		EquivalenceClass & ec = *(it->second);
		if(current_class == class_value)
		{
			sum_same += SQUARE(1 - ec.Ap(class_value));
		}
		else 
		{
			sum_different += SQUARE(ec.Ap(class_value));
		}
	}

	return(sqrt((sum_same + sum_different) / equivalences.size()));
}

double Sigma(DATA_SET & data, std::vector<std::pair<std::string, EquivalenceClass *>> & equivalences, std::vector<std::string> classes)
{
	int num_classes = classes.size();
	double sum = 0;
	for(auto it = classes.begin(); it != classes.end(); ++it)
	{
		sum += Sigma_p(data, equivalences, *it, num_classes);
	}
	return(sum / num_classes);
}

int MVM(DATA_SET & data, double max_threshold, std::string class_column, std::vector<std::string> & best_features) 
{
	std::list<std::string> attributes;
	std::vector<std::string> red_copy;
	std::vector<std::pair<std::string, EquivalenceClass *>> equivalence_assoc;
	std::unordered_map<std::string, EquivalenceClass> equivalences;
	std::vector<std::string> classes;
	std::vector<double> sigmas;

	GetAttributes(data, attributes, class_column);
	FindClasses(data, class_column, classes);

	equivalence_assoc.reserve(data[class_column].size());

	while(!attributes.empty())
	{
		double min_value = std::numeric_limits<double>::infinity();
		std::string min_col;
		for(auto attr_it = attributes.begin(); attr_it != attributes.end(); ++attr_it)
		{
			red_copy.push_back(*attr_it);

			double sigma_new;
			double sigma_old;

			if(best_features.empty())
			{
				sigma_old = 0;
			}
			else
			{
				sigma_old = sigmas.back();
			}

			FindEquivalences(data, equivalence_assoc, equivalences, class_column, red_copy);
			sigma_new = Sigma(data, equivalence_assoc, classes);

			double result = sigma_new - sigma_old;
			if(result < min_value)
			{
				min_value = result;
				min_col = *attr_it;
			}

			equivalence_assoc.clear();
			equivalences.clear();
			red_copy.pop_back();
		}

		if(best_features.empty() || std::abs(min_value) < max_threshold)
		{
			sigmas.push_back(min_value + (sigmas.empty() ? 0 : sigmas.back()));
			best_features.push_back(min_col);
			red_copy.push_back(min_col);
			attributes.remove(min_col);
		}
		else {
			break;
		}
	}

	return 0;
}
