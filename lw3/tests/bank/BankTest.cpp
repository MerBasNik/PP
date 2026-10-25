//
// Created by Вадим Патрушев on 23.03.2026.
//

#include "src/Bank/Bank.h"
#include "src/Bank/BankOperationError.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

struct BankTest : public ::testing::Test
{
	BankTest()
		: bank(1000)
	{
	}
	Bank bank;
};

TEST_F(BankTest, PositiveCash)
{
	EXPECT_NO_THROW(Bank bank(1000));
}
TEST_F(BankTest, ZeroCash)
{
	EXPECT_NO_THROW(Bank bank(0));
}
TEST_F(BankTest, NegativeCash)
{
	EXPECT_THROW(Bank bank(-100), std::out_of_range);
}

TEST_F(BankTest, OpenAccountReturnsUniqueId)
{
	const AccountId accountId1 = bank.OpenAccount();
	const AccountId accountId2 = bank.OpenAccount();
	EXPECT_NE(accountId1, accountId2);
}
TEST_F(BankTest, CreatesAccountWithZeroBalance)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_EQ(bank.GetAccountBalance(accountId), 0);
	EXPECT_EQ(bank.GetCash(), 1000);
}

TEST_F(BankTest, CloseAccount)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	const Money balance = bank.CloseAccount(accountId);
	EXPECT_EQ(balance, 500);
}
TEST_F(BankTest, RemovesAccount)
{
	const AccountId accountId = bank.OpenAccount();
	bank.CloseAccount(accountId);
	EXPECT_THROW((void)bank.GetAccountBalance(accountId), BankOperationError);
	EXPECT_THROW(bank.DepositMoney(accountId, 1), BankOperationError);
	EXPECT_THROW(bank.CloseAccount(accountId), BankOperationError);
}
TEST_F(BankTest, CloseAccountAddsMoneyToCash)
{
	const Money initialCash = bank.GetCash();
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	bank.CloseAccount(accountId);
	EXPECT_EQ(bank.GetCash(), initialCash);
}
TEST_F(BankTest, ClosedAccountIdIsNotReused)
{
	const AccountId accountId = bank.OpenAccount();
	bank.CloseAccount(accountId);
	EXPECT_NE(bank.OpenAccount(), accountId);
}
TEST_F(BankTest, CloseNotExistentAccount)
{
	EXPECT_THROW(bank.CloseAccount(100), BankOperationError);
	EXPECT_EQ(bank.GetCash(), 1000);
}

TEST_F(BankTest, DepositMoneyIncreasesBalance)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 300);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 300);
}
TEST_F(BankTest, DepositMoney)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 300);
	EXPECT_EQ(bank.GetCash(), 700);
}
TEST_F(BankTest, DepositAllCash)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 1000);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 1000);
	EXPECT_EQ(bank.GetCash(), 0);
}
TEST_F(BankTest, DepositNegativeAmount)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_THROW(bank.DepositMoney(accountId, -100), std::out_of_range);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 0);
	EXPECT_EQ(bank.GetCash(), 1000);
}
TEST_F(BankTest, DepositMoreThanCash)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_THROW(bank.DepositMoney(accountId, 1500), BankOperationError);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 0);
	EXPECT_EQ(bank.GetCash(), 1000);
}
TEST_F(BankTest, DepositToNotExistentAccount)
{
	EXPECT_THROW(bank.DepositMoney(100, 100), BankOperationError);
	EXPECT_EQ(bank.GetCash(), 1000);
}

TEST_F(BankTest, WithdrawMoney)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	bank.WithdrawMoney(accountId, 200);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 300);
}
TEST_F(BankTest, WithdrawMoneyIncreasesCash)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	bank.WithdrawMoney(accountId, 200);
	EXPECT_EQ(bank.GetCash(), 700);
}
TEST_F(BankTest, WithdrawWholeBalance)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	bank.WithdrawMoney(accountId, 500);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 0);
	EXPECT_EQ(bank.GetCash(), 1000);
}
TEST_F(BankTest, WithdrawMoreThanBalance)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 100);
	EXPECT_THROW(bank.WithdrawMoney(accountId, 200), BankOperationError);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 100);
	EXPECT_EQ(bank.GetCash(), 900);
}
TEST_F(BankTest, WithdrawNegativeAmount)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	EXPECT_THROW(bank.WithdrawMoney(accountId, -100), std::out_of_range);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 500);
	EXPECT_EQ(bank.GetCash(), 500);
}
TEST_F(BankTest, WithdrawFromNotExistentAccount)
{
	EXPECT_THROW(bank.WithdrawMoney(100, 100), BankOperationError);
	EXPECT_EQ(bank.GetCash(), 1000);
}

TEST_F(BankTest, TryWithdrawMoneySuccess)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	const bool result = bank.TryWithdrawMoney(accountId, 200);
	EXPECT_TRUE(result);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 300);
	EXPECT_EQ(bank.GetCash(), 700);
}
TEST_F(BankTest, TryWithdrawMoneyError)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 100);
	const bool result = bank.TryWithdrawMoney(accountId, 200);
	EXPECT_FALSE(result);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 100);
	EXPECT_EQ(bank.GetCash(), 1000 - 100);
}
TEST_F(BankTest, TryWithdrawMoneyNegativeAmount)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	EXPECT_THROW((void)bank.TryWithdrawMoney(accountId, -100), std::out_of_range);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 500);
	EXPECT_EQ(bank.GetCash(), 500);
}
TEST_F(BankTest, TryWithdrawFromNotExistentAccount)
{
	EXPECT_THROW((void)bank.TryWithdrawMoney(999, 100), BankOperationError);
	EXPECT_EQ(bank.GetCash(), 1000);
}

TEST_F(BankTest, GetAccountBalanceSuccess)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 500);
}
TEST_F(BankTest, GetAccountBalanceForNotExistent)
{
	EXPECT_THROW((void)bank.GetAccountBalance(999), BankOperationError);
}

TEST_F(BankTest, GetCashInitialValue)
{
	EXPECT_EQ(bank.GetCash(), 1000);
}

TEST_F(BankTest, DoSomeActions)
{
	const AccountId accountId1 = bank.OpenAccount();
	const AccountId accountId2 = bank.OpenAccount();
	bank.DepositMoney(accountId1, 500);
	bank.DepositMoney(accountId2, 300);
	bank.WithdrawMoney(accountId2, 100);
	EXPECT_TRUE(bank.TryWithdrawMoney(accountId1, 200));
	EXPECT_FALSE(bank.TryWithdrawMoney(accountId1, 400));
	EXPECT_EQ(bank.GetAccountBalance(accountId1), 300);
	EXPECT_EQ(bank.GetAccountBalance(accountId2), 200);
	EXPECT_EQ(bank.GetCash(), 1000 - 300 - 200);
}

TEST_F(BankTest, OperationsCountStartsWithZero)
{
	EXPECT_EQ(bank.GetOperationsCount(), 0u);
}
TEST_F(BankTest, OperationsCountCountsFailedCalls)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 100);
	bank.WithdrawMoney(accountId, 50);
	EXPECT_TRUE(bank.TryWithdrawMoney(accountId, 10));
	EXPECT_FALSE(bank.TryWithdrawMoney(accountId, 1000));
	(void)bank.GetAccountBalance(accountId);
	(void)bank.GetCash();
	EXPECT_THROW(bank.DepositMoney(accountId, -1), std::out_of_range);
	EXPECT_THROW(bank.WithdrawMoney(accountId, 1000), BankOperationError);
	EXPECT_THROW((void)bank.GetAccountBalance(999), BankOperationError);
	bank.CloseAccount(accountId);
	EXPECT_THROW(bank.CloseAccount(accountId), BankOperationError);

	EXPECT_EQ(bank.GetOperationsCount(), 12u);
	EXPECT_EQ(bank.GetOperationsCount(), 12u);
}

TEST(BankMultithreadingTest, ParallelDepositsAndWithdrawalsDoNotLoseMoney)
{
	constexpr int THREAD_COUNT = 8;
	constexpr int ITERATIONS = 100'000;

	Bank bank(1000);
	const AccountId accountId = bank.OpenAccount();
	const Money cashBefore = bank.GetCash();
	const Money balanceBefore = bank.GetAccountBalance(accountId);
	const auto operationsBefore = bank.GetOperationsCount();

	std::vector<std::thread> threads;
	threads.reserve(THREAD_COUNT);
	for (int t = 0; t < THREAD_COUNT; ++t)
	{
		threads.emplace_back([&bank, accountId] {
			for (int i = 0; i < ITERATIONS; ++i)
			{
				bank.DepositMoney(accountId, 1);
				bank.WithdrawMoney(accountId, 1);
			}
		});
	}
	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(bank.GetOperationsCount() - operationsBefore, 1'600'000u);
	EXPECT_EQ(bank.GetCash(), cashBefore);
	EXPECT_EQ(bank.GetAccountBalance(accountId), balanceBefore);
}
