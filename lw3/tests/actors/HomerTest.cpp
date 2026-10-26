//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Customer.h"
#include "src/Actors/Homer.h"
#include "src/Actors/MrBurns.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class HomerTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_homer.SetAccountId(m_bank.OpenAccount());
		m_mrBurns.SetAccountId(m_bank.OpenAccount());

		m_homer.SetMarge(&m_marge);
		m_homer.SetBart(&m_bart);
		m_homer.SetLisa(&m_lisa);
		m_homer.SetMrBurns(&m_mrBurns);

		m_bank.DepositMoney(*m_homer.GetAccountId(), 1000);
	}

	Bank m_bank{ 10000 };
	Homer m_homer{ 500, 200, 300 };
	Customer m_marge{ "Marge", 1500 };
	Customer m_bart{ "Bart", 400 };
	Customer m_lisa{ "Lisa", 400 };
	MrBurns m_mrBurns{ 5000 };
};

TEST_F(HomerTest, PayElectricityBill)
{
	m_homer.PayElectricityBill(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_homer.GetAccountId()), 800);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_mrBurns.GetAccountId()), 200);
	EXPECT_EQ(m_homer.GetCash(), 0);
}

TEST_F(HomerTest, PayElectricityBillDidntEnoughMoney)
{
	m_bank.WithdrawMoney(*m_homer.GetAccountId(), 900);
	m_homer.PayElectricityBill(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_homer.GetAccountId()), 100);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_mrBurns.GetAccountId()), 0);
}

TEST_F(HomerTest, WithdrawFamilyCash)
{
	m_homer.WithdrawFamilyCash(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_homer.GetAccountId()), 200);
	EXPECT_EQ(m_homer.GetCash(), 800);
}

TEST_F(HomerTest, WithdrawFamilyCashDidntEnoughMoney)
{
	m_bank.WithdrawMoney(*m_homer.GetAccountId(), 500);
	m_homer.WithdrawFamilyCash(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_homer.GetAccountId()), 500);
	EXPECT_EQ(m_homer.GetCash(), 0);
}

TEST_F(HomerTest, GiveMoneyToFamily)
{
	m_homer.GetWallet().Add(800);
	m_homer.GiveMoneyToFamily();
	EXPECT_EQ(m_homer.GetCash(), 0);
	EXPECT_EQ(m_marge.GetCash(), 500);
	EXPECT_EQ(m_bart.GetCash(), 150);
	EXPECT_EQ(m_lisa.GetCash(), 150);
}

TEST_F(HomerTest, GiveMoneyToFamilyDidntEnoughCash)
{
	m_homer.GetWallet().Add(600);
	m_homer.GiveMoneyToFamily();
	EXPECT_EQ(m_homer.GetCash(), 100);
	EXPECT_EQ(m_marge.GetCash(), 500);
	EXPECT_EQ(m_bart.GetCash(), 0);
	EXPECT_EQ(m_lisa.GetCash(), 0);
}

TEST_F(HomerTest, DoAction)
{
	m_homer.DoAction(m_bank);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_homer.GetAccountId()), 0);
	EXPECT_EQ(m_bank.GetAccountBalance(*m_mrBurns.GetAccountId()), 200);
	EXPECT_EQ(m_homer.GetCash(), 0);
	EXPECT_EQ(m_marge.GetCash(), 500);
	EXPECT_EQ(m_bart.GetCash(), 150);
	EXPECT_EQ(m_lisa.GetCash(), 150);
}
