#include "src/Economy/Economy.h"
#include <exception>
#include <iostream>

constexpr int COUNT_ARGS_FROM_PARAMS = 2;
int StartSimulationEconomy(int argc, const char* argv[]);

int main(const int argc, const char* argv[])
{
	return StartSimulationEconomy(argc, argv);
}

int StartSimulationEconomy(const int argc, const char* argv[])
{
	try
	{
		std::istream& input = std::cin;
		int iterations = 0;
		if (argc == COUNT_ARGS_FROM_PARAMS)
		{
			iterations = std::stoi(argv[1]);
		}
		else
		{
			input >> iterations;
		}
		const Economy economy(iterations);
		economy.StartEconomy();
	}
	catch (std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
