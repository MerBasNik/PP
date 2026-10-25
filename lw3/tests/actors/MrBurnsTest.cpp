//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Homer.h"
#include "src/Actors/MrBurns.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class MrBurnsTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_mrBurns.SetAccountId(m_bank.OpenAccount());
		m_homer.SetAccountId(m_bank.OpenAccount());
		m_mrBurns.AddEmployee(&m_homer);
		m_bank.DepositMoney(*m_mrBurns.GetAccountId(), 5000);
	}

	Bank m_bank{ 1000000 };
	MrBurns m_mrBurns{ 5000 };
	Homer m_homer{ 1000, 200, 300 };
};

TEST_F(MrBurnsTest, PaySalarySuccess)
{
	const AccountId mrBurnsId = *m_mrBurns.GetAccountId();
	const AccountId homerId = *m_homer.GetAccountId();
	const Money initialCash = m_bank.GetCash();
	m_mrBurns.DoAction(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(mrBurnsId), 0);
	EXPECT_EQ(m_bank.GetAccountBalance(homerId), 5000);
	EXPECT_EQ(m_mrBurns.GetCash(), 0);
	EXPECT_EQ(m_bank.GetCash(), initialCash);
}

TEST_F(MrBurnsTest, PaySalaryUsesTryWithdrawAndDeposit)
{
	const auto operationsBefore = m_bank.GetOperationsCount();
	m_mrBurns.DoAction(m_bank);
	EXPECT_EQ(m_bank.GetOperationsCount() - operationsBefore, 2u);
}

TEST_F(MrBurnsTest, PaySalaryDidntEnoughMoney)
{
	const AccountId mrBurnsId = *m_mrBurns.GetAccountId();
	const AccountId homerId = *m_homer.GetAccountId();
	EXPECT_NO_THROW(m_mrBurns.DoAction(m_bank));
	EXPECT_NO_THROW(m_mrBurns.DoAction(m_bank));
	EXPECT_EQ(m_bank.GetAccountBalance(mrBurnsId), 0);
	EXPECT_EQ(m_bank.GetAccountBalance(homerId), 5000);
}

TEST_F(MrBurnsTest, EmployeeWithoutAccount)
{
	Homer homerWithoutAccount(1000, 200, 300);
	MrBurns mrBurns(100);
	mrBurns.SetAccountId(*m_mrBurns.GetAccountId());
	mrBurns.AddEmployee(&homerWithoutAccount);
	EXPECT_NO_THROW(mrBurns.DoAction(m_bank));
	EXPECT_EQ(m_bank.GetAccountBalance(*m_mrBurns.GetAccountId()), 5000);
}
