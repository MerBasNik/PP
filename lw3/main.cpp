#include "src/Args/Args.h"
#include "src/Economy/Economy.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>

static_assert(std::atomic<bool>::is_always_lock_free, "the flag is written from a signal handler");
static std::atomic<bool> g_stopRequested = false;

static void OnSigInt(int)
{
	g_stopRequested = true;
}

int main(const int argc, const char* argv[])
{
	try
	{
		const Args args = ParseArgs(argc, argv);
		std::signal(SIGINT, OnSigInt);

		Economy economy(args.quiet);
		economy.Run(args.mode, std::chrono::seconds(args.seconds), g_stopRequested);
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
