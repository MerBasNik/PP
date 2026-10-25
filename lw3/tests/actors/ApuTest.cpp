//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/MrBurns.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class ApuTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_apu.SetAccountId(m_bank.OpenAccount());
		m_mrBurns.SetAccountId(m_bank.OpenAccount());
		m_apu.SetMrBurns(&m_mrBurns);

		m_bank.DepositMoney(*m_apu.GetAccountId(), 1000);
		m_bank.WithdrawMoney(*m_apu.GetAccountId(), 1000);
		m_apu.GetWallet().Add(1000);
	}

	Bank m_bank{ 1000000 };
	Apu m_apu{ 1000 };
	MrBurns m_mrBurns{ 5000 };
};

TEST_F(ApuTest, DepositRevenue)
{
	const Money initialBankCash = m_bank.GetCash();
	m_apu.DepositRevenue(m_bank);
	EXPECT_EQ(m_apu.GetCash(), 0);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_apu.GetAccountId()), 1000);
	EXPECT_EQ(m_bank.GetCash(), initialBankCash - 1000);
}

TEST_F(ApuTest, DepositWithoutRevenue)
{
	m_apu.DepositRevenue(m_bank);
	const auto operationsBefore = m_bank.GetOperationsCount();
	m_apu.DepositRevenue(m_bank);
	EXPECT_EQ(m_bank.GetOperationsCount(), operationsBefore);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_apu.GetAccountId()), 1000);
}

TEST_F(ApuTest, PayElectricityBill)
{
	m_apu.DoAction(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_apu.GetAccountId()), 0);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_mrBurns.GetAccountId()), 1000);
	EXPECT_EQ(m_apu.GetCash(), 0);
}

TEST_F(ApuTest, DidntPayElectricity)
{
	ASSERT_TRUE(m_apu.GetWallet().TrySpend(500));
	m_apu.DoAction(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_apu.GetAccountId()), 500);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_mrBurns.GetAccountId()), 0);
}
