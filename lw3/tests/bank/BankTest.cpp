//
// Created by Вадим Патрушев on 23.03.2026.
//

#include "src/Bank/Bank.h"
#include "src/Bank/BankOperationError.h"
#include <gtest/gtest.h>

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
	EXPECT_THROW(Bank bank(-100), BankOperationError);
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
	auto _ = bank.CloseAccount(accountId);
	EXPECT_THROW(bank.GetAccountBalance(accountId), BankOperationError);
}
TEST_F(BankTest, CloseAccountAddsMoneyToCash)
{
	const Money initialCash = bank.GetCash();
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	const auto cash = bank.CloseAccount(accountId);
	EXPECT_EQ(bank.GetCash(), initialCash);
}

TEST_F(BankTest, CloseNotExistentAccount)
{
	EXPECT_THROW(bank.CloseAccount(100), BankOperationError);
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
TEST_F(BankTest, DepositNegativeAmount)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_THROW(bank.DepositMoney(accountId, -100), std::out_of_range);
}
TEST_F(BankTest, DepositMoreThanCash)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_THROW(bank.DepositMoney(accountId, 1500), BankOperationError);
}
TEST_F(BankTest, DepositToNotExistentAccount)
{
	EXPECT_THROW(bank.DepositMoney(100, 100), BankOperationError);
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
TEST_F(BankTest, WithdrawMoreThanBalance)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 100);
	EXPECT_THROW(bank.WithdrawMoney(accountId, 200), BankOperationError);
}
TEST_F(BankTest, WithdrawNegativeAmount)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	EXPECT_THROW(bank.WithdrawMoney(accountId, -100), std::out_of_range);
}
TEST_F(BankTest, WithdrawFromNotExistentAccount)
{
	EXPECT_THROW(bank.WithdrawMoney(100, 100), BankOperationError);
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
	EXPECT_THROW(bank.TryWithdrawMoney(accountId, -100), std::out_of_range);
}
TEST_F(BankTest, TryWithdrawFromNotExistentAccount)
{
	EXPECT_THROW(bank.TryWithdrawMoney(999, 100), BankOperationError);
}

TEST_F(BankTest, SendMoneySuccess)
{
	const AccountId srcId = bank.OpenAccount();
	const AccountId dstId = bank.OpenAccount();
	bank.DepositMoney(srcId, 500);
	bank.SendMoney(srcId, dstId, 300);
	EXPECT_EQ(bank.GetAccountBalance(srcId), 200);
	EXPECT_EQ(bank.GetAccountBalance(dstId), 300);
	EXPECT_EQ(bank.GetCash(), 500);
}
TEST_F(BankTest, SendMoneyInsufficientFunds)
{
	const AccountId srcId = bank.OpenAccount();
	const AccountId dstId = bank.OpenAccount();
	bank.DepositMoney(srcId, 100);
	EXPECT_THROW(bank.SendMoney(srcId, dstId, 200), BankOperationError);
	EXPECT_EQ(bank.GetAccountBalance(srcId), 100);
	EXPECT_EQ(bank.GetAccountBalance(dstId), 0);
}
TEST_F(BankTest, SendMoneyNegativeAmount)
{
	const AccountId srcId = bank.OpenAccount();
	const AccountId dstId = bank.OpenAccount();
	bank.DepositMoney(srcId, 500);
	EXPECT_THROW(bank.SendMoney(srcId, dstId, -100), std::out_of_range);
}
TEST_F(BankTest, SendMoneyFromNotExistentAccount)
{
	const AccountId dstId = bank.OpenAccount();
	EXPECT_THROW(bank.SendMoney(999, dstId, 100), BankOperationError);
}
TEST_F(BankTest, SendMoneyToNotExistentAccount)
{
	const AccountId srcId = bank.OpenAccount();
	bank.DepositMoney(srcId, 500);
	EXPECT_THROW(bank.SendMoney(srcId, 999, 100), BankOperationError);
}
TEST_F(BankTest, TrySendMoneySuccess)
{
	const AccountId srcId = bank.OpenAccount();
	const AccountId dstId = bank.OpenAccount();
	bank.DepositMoney(srcId, 500);
	const bool result = bank.TrySendMoney(srcId, dstId, 300);
	EXPECT_TRUE(result);
	EXPECT_EQ(bank.GetAccountBalance(srcId), 200);
	EXPECT_EQ(bank.GetAccountBalance(dstId), 300);
}
TEST_F(BankTest, TrySendMoneyBalanceLessAmount)
{
	const AccountId srcId = bank.OpenAccount();
	const AccountId dstId = bank.OpenAccount();
	bank.DepositMoney(srcId, 100);
	const bool result = bank.TrySendMoney(srcId, dstId, 200);
	EXPECT_FALSE(result);
	EXPECT_EQ(bank.GetAccountBalance(srcId), 100);
	EXPECT_EQ(bank.GetAccountBalance(dstId), 0);
}
TEST_F(BankTest, TrySendMoneyNegativeAmount)
{
	const AccountId srcId = bank.OpenAccount();
	const AccountId dstId = bank.OpenAccount();
	bank.DepositMoney(srcId, 500);
	EXPECT_THROW(bank.TrySendMoney(srcId, dstId, -100), std::out_of_range);
}
TEST_F(BankTest, TrySendMoneyFromNotExistentAccount)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_THROW(bank.TrySendMoney(100, accountId, 100), BankOperationError);
}
TEST_F(BankTest, TrySendMoneyToNotExistentAccount)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	EXPECT_THROW(bank.TrySendMoney(accountId, 100, 100), BankOperationError);
}

TEST_F(BankTest, GetAccountBalanceSuccess)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	EXPECT_EQ(bank.GetAccountBalance(accountId), 500);
}
TEST_F(BankTest, GetAccountBalanceForEmptyAccount)
{
	const AccountId accountId = bank.OpenAccount();
	EXPECT_EQ(bank.GetAccountBalance(accountId), 0);
}
TEST_F(BankTest, GetAccountBalanceForNotExistent)
{
	EXPECT_THROW(bank.GetAccountBalance(999), BankOperationError);
}

TEST_F(BankTest, GetCashInitialValue)
{
	EXPECT_EQ(bank.GetCash(), 1000);
}
TEST_F(BankTest, GetCashAfterDeposit)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 300);
	EXPECT_EQ(bank.GetCash(), 700);
}
TEST_F(BankTest, GetCashAfterWithdraw)
{
	const AccountId accountId = bank.OpenAccount();
	bank.DepositMoney(accountId, 500);
	bank.WithdrawMoney(accountId, 200);
	EXPECT_EQ(bank.GetCash(), 700);
}

TEST_F(BankTest, DoSomeActions)
{
	const AccountId accountId1 = bank.OpenAccount();
	const AccountId accountId2 = bank.OpenAccount();
	const AccountId accountId3 = bank.OpenAccount();
	bank.DepositMoney(accountId1, 500);
	bank.DepositMoney(accountId2, 300);
	bank.SendMoney(accountId1, accountId2, 200);
	bank.WithdrawMoney(accountId2, 100);
	bank.SendMoney(accountId2, accountId3, 300);
	EXPECT_EQ(bank.GetAccountBalance(accountId1), 300);
	EXPECT_EQ(bank.GetAccountBalance(accountId2), 100);
	EXPECT_EQ(bank.GetAccountBalance(accountId3), 300);
	EXPECT_EQ(bank.GetCash(), 1000 - 500 - 300 + 100);
}
TEST_F(BankTest, DoSomeOperations)
{
	const AccountId accountId1 = bank.OpenAccount();
	const AccountId accountId2 = bank.OpenAccount();
	bank.DepositMoney(accountId1, 500);
	const bool result1 = bank.TrySendMoney(accountId1, accountId2, 200);
	const bool result2 = bank.TrySendMoney(accountId1, accountId2, 400);
	EXPECT_TRUE(result1);
	EXPECT_FALSE(result2);
	EXPECT_EQ(bank.GetAccountBalance(accountId1), 300);
	EXPECT_EQ(bank.GetAccountBalance(accountId2), 200);
}