//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Lisa.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class LisaTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(1000000);
		m_lisa = new Lisa(70, 50);
		m_apu = new Apu(200);
		m_lisa->SetApu(m_apu);
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_lisa;
		delete m_apu;
	}

	Bank* m_bank = nullptr;
	Lisa* m_lisa = nullptr;
	Apu* m_apu = nullptr;
};

TEST_F(LisaTest, SpendMoneyAtApu)
{
	const Money initialLisaCash = m_lisa->GetCash();
	const Money initialApuCash = m_apu->GetCash();
	m_lisa->DoAction(*m_bank);
	EXPECT_EQ(m_lisa->GetCash(), initialLisaCash - 50);
	EXPECT_EQ(m_apu->GetCash(), initialApuCash + 50);
}

TEST_F(LisaTest, NotEnoughMoney)
{
	const Money initialLisaCash = m_lisa->GetCash();
	const Money initialApuCash = m_apu->GetCash();
	m_lisa->DoAction(*m_bank);
	m_lisa->DoAction(*m_bank);
	EXPECT_EQ(m_lisa->GetCash(), initialLisaCash - 50);
	EXPECT_EQ(m_apu->GetCash(), initialApuCash + 50);
}