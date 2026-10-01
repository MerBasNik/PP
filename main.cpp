//
// Created by Вадим Патрушев on 24.09.2026.
//
#include <charconv>
#include <iostream>
#include <string_view>
#include <thread>
#include <vector>
#include <syncstream>

std::mutex coutMutex;

void Worker(const int index)
{
	const std::string message =
		"Поток № " + std::to_string(index) + " выполняет свою работу\n";

	std::lock_guard lock(coutMutex);
	std::cout << message;
}

int main(const int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "При вызове программы нужно передавать аргументов число потоков" << std::endl;
		return 1;
	}

	const int threadCount = std::stoi(argv[1]);

	std::vector<std::jthread> threads;
	threads.reserve(threadCount);

	for (int i = 1; i <= threadCount; ++i)
	{
		threads.emplace_back(Worker, i);
	}

	return 0;
}