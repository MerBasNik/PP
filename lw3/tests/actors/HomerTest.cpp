//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Bart.h"
#include "src/Actors/Homer.h"
#include "src/Actors/Lisa.h"
#include "src/Actors/Marge.h"
#include "src/Actors/MrBurns.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class HomerTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(10000);
		m_homer = new Homer(1200, 1200, 300);
		m_marge = new Marge(1500);
		m_bart = new Bart(100, 400);
		m_lisa = new Lisa(100, 400);
		m_mrBurns = new MrBurns(5000);

		m_homer->SetAccountId(m_bank->OpenAccount());
		m_marge->SetAccountId(m_bank->OpenAccount());
		m_mrBurns->SetAccountId(m_bank->OpenAccount());

		m_homer->SetMarge(m_marge);
		m_homer->SetBart(m_bart);
		m_homer->SetLisa(m_lisa);
		m_homer->SetMrBurns(m_mrBurns);

		m_bank->DepositMoney(m_homer->GetAccountId().value(), 1000);
	}
	void TearDown() override
	{
		delete m_bank;
		delete m_homer;
		delete m_marge;
		delete m_bart;
		delete m_lisa;
		delete m_mrBurns;
	}
	Bank* m_bank = nullptr;
	Homer* m_homer = nullptr;
	Marge* m_marge = nullptr;
	Bart* m_bart = nullptr;
	Lisa* m_lisa = nullptr;
	MrBurns* m_mrBurns = nullptr;
};

TEST_F(HomerTest, SendMoneyToMarge)
{
	m_bank->DepositMoney(m_homer->GetAccountId().value(), 200);
	// m_homer->SendMargeMoney(*m_bank);
	m_homer->DoAction(*m_bank);
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId margeId = m_marge->GetAccountId().value();
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), 0);
	EXPECT_EQ(m_bank->GetAccountBalance(margeId), 1200);
}
TEST_F(HomerTest, SendMoneyToMargeDidntEnoughMoney)
{
	// m_homer->SendMargeMoney(*m_bank);
	m_homer->DoAction(*m_bank);
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId margeId = m_marge->GetAccountId().value();
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), 700);
	EXPECT_EQ(m_bank->GetAccountBalance(margeId), 0);
}

TEST_F(HomerTest, WithdrawForChildren)
{
	const Money bartCash = m_bart->GetCash();
	const Money lisaCash = m_lisa->GetCash();
	// m_homer->GetChildrenMoney(*m_bank);
	m_homer->DoAction(*m_bank);
	EXPECT_EQ(m_bart->GetCash(), bartCash + 150);
	EXPECT_EQ(m_lisa->GetCash(), lisaCash + 150);
}

TEST_F(HomerTest, PayElectricityBill)
{
	m_bank->DepositMoney(m_homer->GetAccountId().value(), 1400);
	// m_homer->PayElectricityBill(*m_bank);
	m_homer->DoAction(*m_bank);
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId burnsId = m_mrBurns->GetAccountId().value();
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), 0);
	EXPECT_EQ(m_bank->GetAccountBalance(burnsId), 1200);
}

TEST_F(HomerTest, PayElectricityBillDidntEnoughMoney)
{
	m_homer->DoAction(*m_bank);
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId burnsId = m_mrBurns->GetAccountId().value();
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), 700);
	EXPECT_EQ(m_bank->GetAccountBalance(burnsId), 0);
}
