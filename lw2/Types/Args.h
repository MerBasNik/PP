//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef PP_ARGS_H
#define PP_ARGS_H
#include <string>

constexpr int MIN_THREADS = 1;
constexpr int MAX_THREADS = 16;

struct Args
{
	std::string input;
	std::string output;
	int threads = MIN_THREADS;
	int radius = 1;
};

#endif //PP_ARGS_H
