#include "ReadCsv.h"
#include "FCBF.h"
#include "MVM.h"

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#define FCBF_TYPE 0
#define MVM_TYPE 1

int RunTest(std::string file_name, int type, double threshold);

int main(void)
{
	std::cout << "Car - FCBF" << std::endl;
	RunTest("car.csv", FCBF_TYPE, 0.2);
	std::cout << std::endl;

	std::cout << "Car - MVM" << std::endl;
	RunTest("car.csv", MVM_TYPE, 0.2);
	std::cout << std::endl;

	std::cout << "Mushroom - FCBF" << std::endl;
	RunTest("mushroom.csv", FCBF_TYPE, 0.2);
	std::cout << std::endl;

	std::cout << "Mushroom - MVM" << std::endl;
	RunTest("mushroom.csv", MVM_TYPE, 0.2);
	std::cout << std::endl;

	std::cin.get();

	return 0;
}

int RunTest(std::string file_name, int type, double threshold)
{
	std::clock_t start;
	double duration;

	DATA_SET data;
	std::vector<std::string> best_features;
	ReadCsv(file_name, data);

	start = std::clock();

	if(type == FCBF_TYPE)
	{
		FCBF(data, threshold, "class", best_features);
	}
	else if(type == MVM_TYPE)
	{
		MVM(data, threshold, "class", best_features);
	}
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	std::cout << "Selected Features:" << std::endl; 

	for(auto it = best_features.begin(); it != best_features.end(); ++it)
	{
		std::cout << "  " << *it << std::endl;
	}

	std::cout<<"Duration: "<< duration << " s" << std::endl;
	return 0;
}