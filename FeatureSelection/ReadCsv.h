#ifndef _READ_CSV_

#define _READ_CSV_

#include <unordered_map>
#include <vector>
#include <string>

#define DATA_SET std::unordered_map<std::string, std::vector<std::string>>

int GetKeys(const DATA_SET & data, std::vector<std::string> & keys);

int ReadCsv(std::string fileName, DATA_SET & map, bool hasHeaders = true);
int ReadLine(std::istream & input, std::vector<std::string> & row);

#endif
