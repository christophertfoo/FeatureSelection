#include <cmath>
#include <utility>
#include <algorithm>
#include "FCBF.h"

double GetProb(std::string feature, std::string value, PROB_HASH & hash)
{   
	double probability = MISSING_PROB;
	if(hash.count(feature) && hash[feature].count(value))
	{
		probability = hash[feature][value];
	}
	return probability;
}
double GetGivenProb(std::string feature, std::string value, std::string given_feature, std::string given_value, PROB_GIVEN_HASH & hash)
{
	double probability = MISSING_PROB;
	if(hash.count(feature) && hash[feature].count(value) && hash[feature][value].count(given_feature) && hash[feature][value][given_feature][given_value])
	{
		probability = hash[feature][value][given_feature][given_value];
	}
	return probability;
}
int CalculateProbabilities(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string y_col_name)
{
	std::vector<std::string> keys;
	if(GetKeys(data, keys))
	{
		// ERROR
	}
	else
	{
		// Calculate probabilities
		for(auto it = keys.begin(); it != keys.end(); ++it)
		{
			std::string col_name = *it;
			const std::vector<std::string> & column = data[col_name];

			// Count Occurrences
			for(auto col_it = column.begin(); col_it != column.end(); ++col_it)
			{
				std::string value = *col_it;

				if(prob.count(col_name) && prob[col_name].count(value))
				{
					prob[col_name][value] += 1;
				}
				else
				{
					prob[col_name][value] = 1;
				}
			}

			// Divide by total
			int size = column.size();
			std::unordered_map<std::string, double> & col_probs = prob[col_name];
			for(auto val_it = col_probs.begin(); val_it != col_probs.end(); ++val_it)
			{
				col_probs[val_it->first] = val_it->second / size;
			}
		}

		// Calculate given probabilities for the y column to every other column
		const std::vector<std::string> & y_column = data[y_col_name];
		int size = y_column.size();
		for(auto it = keys.begin(); it != keys.end(); ++it)
		{
			std::string given_col_name = *it;
			if(given_col_name != y_col_name)
			{
				const std::vector<std::string> & given_column = data[given_col_name];

				// Count occurrences
				for(int i = 0; i < size; i++)
				{
					std::string y_value = y_column[i];
					std::string given_value = given_column[i];
					if(given_prob.count(y_col_name) && given_prob[y_col_name].count(given_col_name) && given_prob[y_col_name][given_col_name].count(given_value) && given_prob[y_col_name][given_col_name][given_value].count(y_value))
					{
						given_prob[y_col_name][given_col_name][given_value][y_value] += 1;
					}
					else
					{
						given_prob[y_col_name][given_col_name][given_value][y_value] = 1;
					}
				}

				// Divide by total
				auto & column_probs = given_prob[y_col_name][given_col_name];
				for(auto y_it = column_probs.begin(); y_it != column_probs.end(); ++y_it)
				{
					std::string given_value = y_it->first;
					auto & given_column_probs = y_it->second;
					int count_match = 0;
					for(auto given_it = given_column_probs.begin(); given_it != given_column_probs.end(); ++given_it)
					{
						count_match += given_it->second;
					}
					for(auto given_it = given_column_probs.begin(); given_it != given_column_probs.end(); ++given_it)
					{
						std::string column_value = given_it->first;
						given_prob[y_col_name][given_col_name][given_value][column_value] = given_it->second / count_match;
					}
				}
			}
		}
	}

	return 0;
}
int CalculateGivenProbabilities(DATA_SET data, PROB_GIVEN_HASH & given_prob, std::string column_name, std::string given_column_name)
{
	const std::vector<std::string> & column = data[column_name];
	int size = column.size();


	const std::vector<std::string> & given_column = data[given_column_name];

	// Count occurrences
	for(int i = 0; i < size; i++)
	{
		std::string column_value = column[i];
		std::string given_value = given_column[i];
		if(given_prob.count(column_name) && given_prob[column_name].count(given_column_name) && given_prob[column_name][given_column_name].count(given_value) && given_prob[column_name][given_column_name][given_value].count(column_value))
		{
			given_prob[column_name][given_column_name][given_value][column_value] += 1;
		}
		else
		{
			given_prob[column_name][given_column_name][given_value][column_value] = 1;
		}
	}

	// Divide by total
	auto & column_probs = given_prob[column_name][given_column_name];
	for(auto column_it = column_probs.begin(); column_it != column_probs.end(); ++column_it)
	{
		std::string given_value = column_it->first;
		auto & given_value_probs = column_it->second;
		int count_match = 0;
		for(auto given_it = given_value_probs.begin(); given_it != given_value_probs.end(); ++given_it)
		{
			count_match += given_it->second;
		}
		for(auto given_it = given_value_probs.begin(); given_it != given_value_probs.end(); ++given_it)
		{
			std::string column_value = given_it->first;
			given_prob[column_name][given_column_name][given_value][column_value] = given_it->second / count_match;
		}
	}
	return 0;
}

double H(PROB_HASH & prob, std::string feature)
{
	double sum = 0;
	std::unordered_map<std::string, double> & feature_prob = prob[feature];
	for(auto it = feature_prob.begin(); it != feature_prob.end(); ++it)
	{
		sum += it->second * LOG2(it->second);
	}
	return -sum;
}
double H(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string feature, std::string given_feature)
{

	if(!given_prob.count(feature) || !given_prob[feature].count(given_feature))
	{
		CalculateGivenProbabilities(data, given_prob, feature, given_feature);
	}

	double sum = 0;
	auto & given_value_probs = given_prob[feature][given_feature];
	auto & given_feature_probs = prob[given_feature];

	for(auto it = given_value_probs.begin(); it != given_value_probs.end(); ++it)
	{
		std::string given_value = it->first;
		std::unordered_map<std::string, double> & value_probs = it->second;

		double given_value_prob = given_feature_probs[given_value];

		double temp_sum = 0;
		for(auto value_it = value_probs.begin(); value_it != value_probs.end(); ++value_it)
		{
			temp_sum += value_it->second * LOG2(value_it->second);
		}
		sum += temp_sum * given_value_prob;
	}
	return -sum;
}

double IG(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string feature, std::string given_feature)
{
	return H(prob, feature) - H(data, prob, given_prob, feature, given_feature);
}
double SU(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string feature, std::string given_feature)
{
	return 2 * (IG(data, prob, given_prob, feature, given_feature) / (H(prob, feature) + H(data, prob, given_prob, feature, given_feature)));
}


bool TupleCompare(const std::pair<std::string, double> & x1, const std::pair<std::string, double> & x2)
{
	return (x1.second > x2.second);
}

int FCBF(DATA_SET & data, double min_threshold, std::string class_column, std::vector<std::string> & best_features) 
{
	PROB_HASH probs;
	PROB_GIVEN_HASH given_probs;
	std::vector<std::string> feature_names;
	std::list<std::pair<std::string, double>> list;
	GetKeys(data, feature_names);
	CalculateProbabilities(data, probs, given_probs, class_column);

	for(auto it = feature_names.begin(); it != feature_names.end(); ++it)
	{
		if(*it != class_column)
		{
			std::pair<std::string, double> tuple;
			tuple.first = *it;
			tuple.second = SU(data, probs, given_probs, class_column, *it);

			if(tuple.second > min_threshold) 
			{
				list.push_back(tuple);
			}
		}
	}

	list.sort(TupleCompare);

	auto F_j = list.begin();

	while (F_j != list.end())
	{
		auto F_i = F_j;
		++F_i;

		while (F_i != list.end())
		{
			double su_ij = SU(data, probs, given_probs, F_j->first, F_i->first);
			if(su_ij >= F_i->second)
			{
				F_i = list.erase(F_i);
			}
			else
			{
				++F_i;
			}
		}
		++F_j;
	}

	best_features.clear();
	best_features.reserve(list.size());
	for(auto it = list.begin(); it != list.end(); ++it)
	{
		best_features.push_back(it->first);
	}

	return 0;
}
