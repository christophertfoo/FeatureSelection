#ifndef _MVM_
#define _MVM_

#include <vector>
#include <unordered_map>
#include <string>
#include <list>
#include <limits>

#include "ReadCsv.h"

#define SQUARE(X) ((X) * (X))

int MVM(DATA_SET & data, double max_threshold, std::string class_column, std::vector<std::string> & best_features) ;

#endif
