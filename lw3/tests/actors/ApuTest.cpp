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
		m_bank = new Bank(1000000);
		m_apu = new Apu(1000);
		m_mrBurns = new MrBurns(5000);

		const AccountId apuId = m_bank->OpenAccount();
		const AccountId burnsId = m_bank->OpenAccount();

		m_apu->SetAccountId(apuId);
		m_mrBurns->SetAccountId(burnsId);

		m_apu->SetMrBurns(m_mrBurns);
		m_apu->AddCash(1000);
		m_bank->DepositMoney(apuId, 1000);
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_apu;
		delete m_mrBurns;
	}

	Bank* m_bank = nullptr;
	Apu* m_apu = nullptr;
	MrBurns* m_mrBurns = nullptr;
};

TEST_F(ApuTest, DepositCash)
{
	const Money initialBankCash = m_bank->GetCash();
	const Money initialApuCash = m_apu->GetCash();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initialBalance = m_bank->GetAccountBalance(apuId);
	m_apu->DoAction(*m_bank);
	const Money finishBankCash = m_bank->GetCash();
	EXPECT_EQ(m_apu->GetCash(), 0);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initialBalance + initialApuCash - 1000);
	EXPECT_EQ(initialBankCash - initialApuCash, finishBankCash);
}

TEST_F(ApuTest, PayElectricityBill)
{
	const AccountId apuId = m_apu->GetAccountId().value();
	const AccountId mrBurnsId = m_mrBurns->GetAccountId().value();
	const Money cash = m_apu->GetCash();
	const Money initialApuBalance = m_bank->GetAccountBalance(apuId);
	const Money initialMrBurnsBalance = m_bank->GetAccountBalance(mrBurnsId);
	m_apu->DoAction(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initialApuBalance - 1000 + cash);
	EXPECT_EQ(m_bank->GetAccountBalance(mrBurnsId), initialMrBurnsBalance + 1000);
}

TEST_F(ApuTest, DidtPayElectricity)
{
	const AccountId apuId = m_apu->GetAccountId().value();
	const AccountId mrBurnsId = m_mrBurns->GetAccountId().value();
	m_bank->SendMoney(apuId, mrBurnsId, 500);
	m_apu->DoAction(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(mrBurnsId), 1500);
}