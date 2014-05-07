#include "MVM.h"

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

int FindEquivalences(DATA_SET & data, std::vector<std::pair<std::string, double>> & counts, std::string class_column, std::vector<std::string> & attributes) 
{
	int numrows = data[class_column].size();
	int i, j;
	std::pair<std::string, double> dummy;
	if(!attributes.empty())
	{
		for(i = 0; i < numrows; i++)
		{
			int count_equivalent = 0;
			int count_same = 0;
			std::string class_value = data[class_column][i];
			for(j = 0; j < numrows; j++)
			{

				bool equivalent = true;

				for(auto it = attributes.begin(); it != attributes.end(); ++it)
				{
					if(data[*it][j] != data[*it][i])
					{
						equivalent = false;
						break;
					}
				}

				if(equivalent)
				{
					count_equivalent++;

					if(data[class_column][j] == class_value)
					{
						count_same++;
					}
				}
			}
			dummy.first = class_value;
			dummy.second = count_same / ((double) count_equivalent);
			counts.push_back(dummy);
		}
	}
	return 0;
}

double Sigma_p(DATA_SET & data, std::vector<std::pair<std::string, double>> & counts, std::string class_value, int num_classes)
{
	int i = 0;
	double sum_same = 0;
	double sum_different = 0;

	for(auto it = counts.begin(); it != counts.end(); ++it)
	{
		if(it->first == class_value)
		{
			sum_same += SQUARE(1 - it->second);
		}
		else 
		{
			sum_different += SQUARE(it->second);
		}
	}

	return(sqrt((sum_same + sum_different) / counts.size()));
}

double Sigma(DATA_SET & data, std::vector<std::pair<std::string, double>> & counts, std::vector<std::string> classes)
{
	int num_classes = classes.size();
	double sum = 0;
	for(auto it = classes.begin(); it != classes.end(); ++it)
	{
		sum += Sigma_p(data, counts, *it, num_classes);
	}
	return(sum / num_classes);
}

int MVM(DATA_SET & data, double max_threshold, std::string class_column, std::vector<std::string> & best_features) 
{
	std::list<std::string> attributes;
	std::vector<std::string> red_copy;
	std::vector<std::pair<std::string, double>> counts_red;
	std::vector<std::pair<std::string, double>> counts_red_copy;
	std::vector<std::string> classes;
	std::vector<double> sigmas;

	GetAttributes(data, attributes, class_column);
	FindClasses(data, class_column, classes);

	counts_red.reserve(data[class_column].size());
	counts_red_copy.reserve(data[class_column].size());

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

			FindEquivalences(data, counts_red_copy, class_column, red_copy);
			sigma_new = Sigma(data, counts_red_copy, classes);

			double result = sigma_new - sigma_old;
			if(result < min_value)
			{
				min_value = result;
				min_col = *attr_it;
			}

			counts_red.clear();
			counts_red_copy.clear();
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
