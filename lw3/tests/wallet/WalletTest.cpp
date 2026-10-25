#include "src/Wallet/Wallet.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(WalletTest, EmptyByDefault)
{
	const Wallet wallet;
	EXPECT_EQ(wallet.GetBalance(), 0);
}

TEST(WalletTest, AddAndSpend)
{
	Wallet wallet;
	wallet.Add(100);
	wallet.Add(50);
	EXPECT_TRUE(wallet.TrySpend(150));
	EXPECT_EQ(wallet.GetBalance(), 0);
}

TEST(WalletTest, NotEnoughMoney)
{
	Wallet wallet;
	wallet.Add(100);
	EXPECT_FALSE(wallet.TrySpend(101));
	EXPECT_EQ(wallet.GetBalance(), 100);
}

TEST(WalletTest, NegativeAmount)
{
	Wallet wallet;
	wallet.Add(100);
	EXPECT_THROW(wallet.Add(-1), std::out_of_range);
	EXPECT_THROW((void)wallet.TrySpend(-1), std::out_of_range);
	EXPECT_EQ(wallet.GetBalance(), 100);
}

TEST(WalletTest, ParallelAddAndSpendDoNotLoseMoney)
{
	constexpr int THREAD_COUNT = 8;
	constexpr int ITERATIONS = 100'000;

	Wallet wallet;
	{
		std::vector<std::jthread> threads;
		for (int t = 0; t < THREAD_COUNT; ++t)
		{
			threads.emplace_back([&wallet] {
				for (int i = 0; i < ITERATIONS; ++i)
				{
					wallet.Add(1);
					EXPECT_TRUE(wallet.TrySpend(1));
				}
			});
		}
	}
	EXPECT_EQ(wallet.GetBalance(), 0);
}
