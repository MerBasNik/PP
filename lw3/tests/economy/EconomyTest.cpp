//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Economy/Economy.h"
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

struct EconomyTest : public ::testing::Test
{
	Economy economy;
	std::atomic<bool> stopRequested = false;
};

TEST_F(EconomyTest, ConsistentAfterCreation)
{
	EXPECT_NO_THROW(economy.VerifyState());
}

TEST_F(EconomyTest, SingleModeKeepsMoney)
{
	economy.Run(Mode::Single, 200ms, stopRequested);
	EXPECT_GT(economy.GetRunOperationsCount(), 0u);
	EXPECT_NO_THROW(economy.VerifyState());
}

TEST_F(EconomyTest, ParallelModeKeepsMoney)
{
	economy.Run(Mode::Parallel, 200ms, stopRequested);
	EXPECT_GT(economy.GetRunOperationsCount(), 0u);
	EXPECT_NO_THROW(economy.VerifyState());
}

TEST_F(EconomyTest, StopsImmediatelyWhenStopRequested)
{
	stopRequested = true;
	economy.Run(Mode::Parallel, 1h, stopRequested);
	EXPECT_LT(economy.GetRunDuration(), 1s);
	EXPECT_EQ(economy.GetRunOperationsCount(), 0u);
	EXPECT_NO_THROW(economy.VerifyState());
}

TEST_F(EconomyTest, StopsWhenStopRequestedDuringRun)
{
	std::jthread stopper([this] {
		std::this_thread::sleep_for(100ms);
		stopRequested = true;
	});
	economy.Run(Mode::Parallel, 1h, stopRequested);
	EXPECT_LT(economy.GetRunDuration(), 10s);
	EXPECT_NO_THROW(economy.VerifyState());
}

TEST_F(EconomyTest, PrintResultDoesNotThrow)
{
	economy.Run(Mode::Single, 50ms, stopRequested);
	EXPECT_NO_THROW(economy.PrintResult());
}
