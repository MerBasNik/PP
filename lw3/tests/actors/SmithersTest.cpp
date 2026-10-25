//
// Created by Вадим Патрушев on 31.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Smithers.h"
#include "src/Bank/Bank.h"
#include "src/Bank/BankOperationError.h"
#include <gtest/gtest.h>

class SmithersTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(10000);
		m_apu = new Apu(200);
		m_smithers = new Smithers(100);
		const AccountId apuId = m_bank->OpenAccount();
		const AccountId smithersId = m_bank->OpenAccount();
		m_apu->SetAccountId(apuId);
		m_smithers->SetAccountId(smithersId);
		m_smithers->SetApu(m_apu);
		m_bank->DepositMoney(smithersId, 500);
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_apu;
		delete m_smithers;
	}

	Bank* m_bank = nullptr;
	Apu* m_apu = nullptr;
	Smithers* m_smithers = nullptr;
};

TEST_F(SmithersTest, BuyPurchase)
{
	const AccountId smithersId = m_smithers->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initSmithersBalance = m_bank->GetAccountBalance(smithersId);
	const Money initApuBalance = m_bank->GetAccountBalance(apuId);
	m_smithers->BuyPurchase(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(smithersId), initSmithersBalance - 100);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initApuBalance + 100);
}

TEST_F(SmithersTest, BuyPurchaseInsufficientFunds)
{
	const AccountId smithersId = m_smithers->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	m_bank->WithdrawMoney(smithersId, 450);
	const Money initSmithersBalance = m_bank->GetAccountBalance(smithersId);
	const Money initApuBalance = m_bank->GetAccountBalance(apuId);
	m_smithers->BuyPurchase(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(smithersId), initSmithersBalance);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initApuBalance);
}

TEST_F(SmithersTest, BuyPurchaseWithoutApu)
{
	const auto smithersAlone = new Smithers(100);
	const AccountId smithersId = m_bank->OpenAccount();
	smithersAlone->SetAccountId(smithersId);
	m_bank->DepositMoney(smithersId, 500);
	const Money initSmithersBalance = m_bank->GetAccountBalance(smithersId);
	smithersAlone->BuyPurchase(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(smithersId), initSmithersBalance);
	delete smithersAlone;
}

TEST_F(SmithersTest, ReopenAccount)
{
	const AccountId oldId = m_smithers->GetAccountId().value();
	const Money oldBalance = m_bank->GetAccountBalance(oldId);
	m_smithers->ReopenAccount(*m_bank);
	const AccountId newId = m_smithers->GetAccountId().value();
	EXPECT_NE(oldId, newId);
	EXPECT_EQ(m_bank->GetAccountBalance(newId), oldBalance);
	EXPECT_THROW(m_bank->GetAccountBalance(oldId), BankOperationError);
}

TEST_F(SmithersTest, ReopenAccountWithZeroBalance)
{
	const AccountId smithersId = m_smithers->GetAccountId().value();
	m_bank->WithdrawMoney(smithersId, m_bank->GetAccountBalance(smithersId));
	const AccountId oldId = m_smithers->GetAccountId().value();
	m_smithers->ReopenAccount(*m_bank);
	const AccountId newId = m_smithers->GetAccountId().value();
	EXPECT_NE(oldId, newId);
	EXPECT_EQ(m_bank->GetAccountBalance(newId), 0);
}

TEST_F(SmithersTest, ReopenAccountWithoutAccount)
{
	const auto smithersAlone = new Smithers(100);
	smithersAlone->ReopenAccount(*m_bank);
	EXPECT_FALSE(smithersAlone->GetAccountId().has_value());
	delete smithersAlone;
}

TEST_F(SmithersTest, DoAction)
{
	AccountId smithersId = m_smithers->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initSmithersBalance = m_bank->GetAccountBalance(smithersId);
	const Money initApuBalance = m_bank->GetAccountBalance(apuId);
	m_smithers->DoAction(*m_bank);
	smithersId = m_smithers->GetAccountId().value();
	const Money totalInitial = initSmithersBalance + initApuBalance;
	const Money totalFinal = m_bank->GetAccountBalance(smithersId) + m_bank->GetAccountBalance(apuId);
	EXPECT_EQ(totalInitial, totalFinal);
}