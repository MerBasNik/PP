//
// Created by Вадим Патрушев on 31.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Bart.h"
#include "src/Actors/Nelson.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class NelsonTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(10000);
		m_bart = new Bart(100, 50);
		m_apu = new Apu(200);
		m_nelson = new Nelson(50);
		m_nelson->SetBart(m_bart);
		m_nelson->SetApu(m_apu);
		m_apu->SetAccountId(m_bank->OpenAccount());
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_bart;
		delete m_apu;
		delete m_nelson;
	}

	Bank* m_bank = nullptr;
	Bart* m_bart = nullptr;
	Apu* m_apu = nullptr;
	Nelson* m_nelson = nullptr;
};

TEST_F(NelsonTest, StealMoney)
{
	const Money initBartCash = m_bart->GetCash();
	const Money initNelsonCash = m_nelson->GetCash();
	m_nelson->StealMoney();
	EXPECT_EQ(m_bart->GetCash(), initBartCash - 50);
	EXPECT_EQ(m_nelson->GetCash(), initNelsonCash + 50);
}

TEST_F(NelsonTest, StealMoreDidnotEnoughMoney)
{
	const Money initBartCash = m_bart->GetCash();
	m_bart->RemoveCash(80);
	const Money initNelsonCash = m_nelson->GetCash();
	m_nelson->StealMoney();
	EXPECT_EQ(m_bart->GetCash(), 0);
	EXPECT_EQ(m_nelson->GetCash(), initNelsonCash + 20);
}

TEST_F(NelsonTest, BuyCigarettes)
{
	const Money initNelsonCash = m_nelson->GetCash();
	const Money initApuCash = m_apu->GetCash();
	m_nelson->BuyPurchase();
	EXPECT_EQ(m_nelson->GetCash(), 0);
	EXPECT_EQ(m_apu->GetCash(), initApuCash + initNelsonCash);
}

TEST_F(NelsonTest, BuyCigarettesDidnotEnoughMoney)
{
	m_nelson->RemoveCash(m_nelson->GetCash());
	const Money initApuCash = m_apu->GetCash();
	m_nelson->BuyPurchase();
	EXPECT_EQ(m_nelson->GetCash(), 0);
	EXPECT_EQ(m_apu->GetCash(), initApuCash);
}

TEST_F(NelsonTest, DoAction)
{
	const Money initBartCash = m_bart->GetCash();
	const Money initNelsonCash = m_nelson->GetCash();
	const Money initApuCash = m_apu->GetCash();
	m_nelson->DoAction(*m_bank);
	const Money finalBartCash = m_bart->GetCash();
	const Money finalNelsonCash = m_nelson->GetCash();
	const Money finalApuCash = m_apu->GetCash();
	const Money totalInitial = initBartCash + initNelsonCash + initApuCash;
	const Money totalFinal = finalBartCash + finalNelsonCash + finalApuCash;
	EXPECT_EQ(totalInitial, totalFinal);
}