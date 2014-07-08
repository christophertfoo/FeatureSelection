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

int RunTestSet(std::string set_name, std::string csv_file, double fcbf_threshold, double mvm_threshold);

int main(void)
{
	//RunTestSet("Car", "car.csv", 0.2, 0.05);
	//RunTestSet("Mushroom", "mushroom.csv", 0.2, 0.0475);
	//RunTestSet("Soybean", "soybean-large.csv", 0.2, 0.0335);
	//RunTestSet("Connect-4", "connect-4.csv", 0.01, 0.007);

	RunTestSet("Car", "car.csv", 0, 1);
	RunTestSet("Mushroom", "mushroom.csv", 0, 1);
	RunTestSet("Soybean", "soybean-large.csv", 0, 1);
	RunTestSet("Connect-4", "connect-4.csv", 0, 1);
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

int RunTestSet(std::string set_name, std::string csv_file, double fcbf_threshold, double mvm_threshold) {
	std::cout << set_name << " - FCBF" << std::endl;
	RunTest(csv_file, FCBF_TYPE, fcbf_threshold);
	std::cout << std::endl;

	std::cout << set_name << " - MVM" << std::endl;
	RunTest(csv_file, MVM_TYPE, mvm_threshold);
	std::cout << std::endl;

	return 0;
}
