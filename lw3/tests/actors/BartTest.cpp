//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Bart.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class BartTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(1000000);
		m_bart = new Bart(70, 50);
		m_apu = new Apu(200);
		m_bart->SetApu(m_apu);
	}
	void TearDown() override
	{
		delete m_bank;
		delete m_bart;
		delete m_apu;
	}
	Bank* m_bank = nullptr;
	Bart* m_bart = nullptr;
	Apu* m_apu = nullptr;
};

TEST_F(BartTest, SpendMoneyAtApu)
{
	const Money initialBartCash = m_bart->GetCash();
	const Money initialApuCash = m_apu->GetCash();
	m_bart->DoAction(*m_bank);
	EXPECT_EQ(m_bart->GetCash(), initialBartCash - 50);
	EXPECT_EQ(m_apu->GetCash(), initialApuCash + 50);
}

TEST_F(BartTest, NotEnoughMoney)
{
	const Money initialBartCash = m_bart->GetCash();
	const Money initialApuCash = m_apu->GetCash();
	m_bart->DoAction(*m_bank);
	m_bart->DoAction(*m_bank);
	EXPECT_EQ(m_bart->GetCash(), initialBartCash - 50);
	EXPECT_EQ(m_apu->GetCash(), initialApuCash + 50);
}
