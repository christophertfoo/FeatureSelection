#ifndef _MVM_
#define _MVM_

#include <vector>
#include <unordered_map>
#include <string>
#include <list>
#include <limits>

#include "ReadCsv.h"

#define SQUARE(X) ((X) * (X))

class EquvalenceClass
{
public:
	EquvalenceClass(std::string attributeValues);
	bool Included(int index);
	bool Add(int index, std::string attributeValues);
private:
	std::string _attributeValues;
	std::unordered_map<int, bool> _inclusionMap;
	int _totalCount;
	std::unordered_map<std::string, int> _classCounts;
};

int MVM(DATA_SET & data, double max_threshold, std::string class_column, std::vector<std::string> & best_features) ;

#endif
