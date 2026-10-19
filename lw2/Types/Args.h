//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef PP_ARGS_H
#define PP_ARGS_H
#include <string>

struct Args
{
	std::string input;
	std::string output;
	int threads = 0;
	int radius = 1;
};

#endif //PP_ARGS_H
