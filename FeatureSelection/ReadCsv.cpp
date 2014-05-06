#include "ReadCsv.h"
#include <iostream>
#include <fstream>
#include <sstream>

int GetKeys(const DATA_SET & data, std::vector<std::string> & keys)
{
	keys.reserve(data.size());
	for(auto it = data.begin(); it != data.end(); ++it)
	{
		keys.push_back(it->first);
	}
	return 0;
}

int ReadCsv(std::string fileName, DATA_SET & map, bool hasHeaders) 
{
	int numTokens, i;
	std::vector<std::string> names;

	std::ifstream input(fileName);

	std::vector<std::string> headers;
	if(hasHeaders)
	{
		ReadLine(input, headers);
	}
	int j = 1;

	if(headers.empty())
	{
		std::stringstream convert;
		std::string key;

		while(input.good())
		{
			std::vector<std::string> row;
			ReadLine(input, row);
			numTokens = row.size();
			for(i = 0; i < numTokens; i++)
			{
				map[convert.str()].push_back(row[i]);
				convert.str("");
			}
		}
		j++;
	}
	else
	{
		while(input.good())
		{
			std::vector<std::string> row;
			ReadLine(input, row);
			numTokens = row.size();

			for(i = 0; i < numTokens; i++)
			{
				map[headers[i]].push_back(row[i]);
			}
			j++;
		}

	}

	return 0;
}

int ReadLine(std::istream & input, std::vector<std::string> & row)
{
	int returnCode = 0;

	std::string rowString;
	std::string token;
	if(input.good())
	{
		std::getline(input, rowString);

		std::stringstream stream(rowString);
		if(!rowString.empty())
		{
			while(stream.good())
			{
				std::getline(stream, token, ',');
				row.push_back(token);	
			}
		}
	}
	else
	{
		returnCode = 1;
	}
	return returnCode;
}
