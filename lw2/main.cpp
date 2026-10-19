//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "Utils.h"
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

int main(const int argc, char* argv[])
{
	try
	{
		const auto args = ParseArgs(argc, argv);
		Run(args);
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Ошибка: " << e.what() << std::endl;
		return 1;
	}
}
