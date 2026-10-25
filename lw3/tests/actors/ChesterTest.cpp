//
// Created by Вадим Патрушев on 31.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Chester.h"
#include "src/Actors/Homer.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class ChesterTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(10000);
		m_homer = new Homer(500, 200, 100);
		m_apu = new Apu(200);
		m_chester = new Chester(800, 500);

		const AccountId homerId = m_bank->OpenAccount();
		const AccountId apuId = m_bank->OpenAccount();
		const AccountId chesterId = m_bank->OpenAccount();

		m_homer->SetAccountId(homerId);
		m_apu->SetAccountId(apuId);
		m_chester->SetAccountId(chesterId);
		m_chester->SetHomer(m_homer);
		m_chester->SetApu(m_apu);
		m_bank->DepositMoney(homerId, 1000);
		m_bank->DepositMoney(chesterId, 500);
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_homer;
		delete m_apu;
		delete m_chester;
	}

	Bank* m_bank = nullptr;
	Homer* m_homer = nullptr;
	Apu* m_apu = nullptr;
	Chester* m_chester = nullptr;
};

TEST_F(ChesterTest, HackHomer)
{
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId chesterId = m_chester->GetAccountId().value();
	const Money initHomerBalance = m_bank->GetAccountBalance(homerId);
	const Money initChesterBalance = m_bank->GetAccountBalance(chesterId);
	m_chester->HackHomer(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), initHomerBalance - 800);
	EXPECT_EQ(m_bank->GetAccountBalance(chesterId), initChesterBalance + 800);
}

TEST_F(ChesterTest, HackHomerDidnotEnoughMoney)
{
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId chesterId = m_chester->GetAccountId().value();
	m_bank->WithdrawMoney(homerId, 900);
	const Money initHomerBalance = m_bank->GetAccountBalance(homerId);
	const Money initChesterBalance = m_bank->GetAccountBalance(chesterId);
	m_chester->HackHomer(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), initHomerBalance);
	EXPECT_EQ(m_bank->GetAccountBalance(chesterId), initChesterBalance);
}

TEST_F(ChesterTest, BuyPurchase)
{
	const AccountId chesterId = m_chester->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initChesterBalance = m_bank->GetAccountBalance(chesterId);
	const Money initApuBalance = m_bank->GetAccountBalance(apuId);
	m_chester->BuyPurchase(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(chesterId), initChesterBalance - 500);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initApuBalance + 500);
}

TEST_F(ChesterTest, BuyPurchaseDidnotEnoughMoney)
{
	const AccountId chesterId = m_chester->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	m_bank->WithdrawMoney(chesterId, 150);
	const Money initChesterBalance = m_bank->GetAccountBalance(chesterId);
	const Money initApuBalance = m_bank->GetAccountBalance(apuId);
	m_chester->BuyPurchase(*m_bank);
	EXPECT_EQ(m_bank->GetAccountBalance(chesterId), initChesterBalance);
	EXPECT_EQ(m_bank->GetAccountBalance(apuId), initApuBalance);
}

TEST_F(ChesterTest, DoAction)
{
	const AccountId homerId = m_homer->GetAccountId().value();
	const AccountId chesterId = m_chester->GetAccountId().value();
	const AccountId apuId = m_apu->GetAccountId().value();
	const Money initHomerBalance = m_bank->GetAccountBalance(homerId);
	const Money initChesterBalance = m_bank->GetAccountBalance(chesterId);
	const Money initApuBalance = m_bank->GetAccountBalance(apuId);
	m_chester->DoAction(*m_bank);
	const Money totalInitial = initHomerBalance + initChesterBalance + initApuBalance;
	const Money totalFinal = m_bank->GetAccountBalance(homerId) + m_bank->GetAccountBalance(chesterId) + m_bank->GetAccountBalance(apuId);
	EXPECT_EQ(totalInitial, totalFinal);
}