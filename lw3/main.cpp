#include "src/Args/Args.h"
#include "src/Economy/Economy.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>

static std::atomic<bool> g_stop = false;

static void SetStopTrue(int)
{
	g_stop = true;
}

int main(const int argc, const char* argv[])
{
	try
	{
		const Args args = ParseArgs(argc, argv);
		std::signal(SIGINT, SetStopTrue);

		Economy economy(args.quiet);
		economy.Run(args.mode, std::chrono::seconds(args.seconds), g_stop);
		economy.PrintResult();
		economy.VerifyState();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
}
