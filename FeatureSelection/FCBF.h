#ifndef _FCBF_

#define _FCBF_

#include <unordered_map>
#include <vector>
#include <string>
#include <list>

#include "ReadCsv.h"

#define MISSING_PROB -1

#define LOG2(X) (std::log(X)/std::log(2))

// TODO refactor into structs
#define PROB_HASH std::unordered_map<std::string, std::unordered_map<std::string, double>>
#define PROB_GIVEN_HASH std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, double>>>>

double GetProb(std::string feature, std::string value, PROB_HASH & hash);
double GetGivenProb(std::string feature, std::string value, std::string given_feature, std::string given_value, PROB_GIVEN_HASH & hash);

int CalculateProbabilities(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string y_col_name);
int CalculateGivenProbabilities(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string column, std::string given_column);

double H(PROB_HASH & prob, std::string feature);
double H(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string feature, std::string given_feature);

double IG(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string feature, std::string given_feature);
double SU(DATA_SET & data, PROB_HASH & prob, PROB_GIVEN_HASH & given_prob, std::string feature, std::string given_feature);

int FCBF(DATA_SET & data, double min_threshold, std::string class_column, std::vector<std::string> & features);

bool TupleCompare(const std::pair<std::string, double> & x1, const std::pair<std::string, double> & x2);
#endif
