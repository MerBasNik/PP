//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Marge.h"
#include "src/Bank/Bank.h"
#include "src/Bank/BankOperationError.h"

#include <gtest/gtest.h>

class MargeTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(1000000);
		m_marge = new Marge(500);
		m_apu = new Apu(200);

		const AccountId margeId = m_bank->OpenAccount();
		const AccountId apuId = m_bank->OpenAccount();

		m_marge->SetAccountId(margeId);
		m_apu->SetAccountId(apuId);
		m_marge->SetApu(m_apu);
		m_bank->DepositMoney(margeId, 500);
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_marge;
		delete m_apu;
	}

	Bank* m_bank = nullptr;
	Marge* m_marge = nullptr;
	Apu* m_apu = nullptr;
};

TEST_F(MargeTest, BuyProducts)
{
	const AccountId margeId = m_marge->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initialMargeBalance = m_bank->GetAccountBalance(margeId);
	const Money initialApuBalance = m_bank->GetAccountBalance(apuId);
	m_marge->DoAction(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(margeId), initialMargeBalance - 500);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initialApuBalance + 500);
}

TEST_F(MargeTest, BuyProductsNotExistSrcAccount)
{
	const AccountId margeId = 100;
	EXPECT_THROW(m_bank->GetAccountBalance(margeId), BankOperationError);
	EXPECT_NO_THROW(m_marge->DoAction(*m_bank));
}

TEST_F(MargeTest, BuyProductsNotExistDstAccount)
{
	const AccountId apuId = 100;
	EXPECT_THROW(m_bank->GetAccountBalance(apuId), BankOperationError);
	EXPECT_NO_THROW(m_marge->DoAction(*m_bank));
}

TEST_F(MargeTest, BuyProductsDidntInoughMoney)
{
	const AccountId margeId = m_marge->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initialApuBalance = m_bank->GetAccountBalance(apuId);
	EXPECT_NO_THROW(m_marge->DoAction(*m_bank));
	EXPECT_NO_THROW(m_marge->DoAction(*m_bank));
	EXPECT_EQ(m_bank->GetAccountBalance(margeId), 0);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initialApuBalance + 500);
}
