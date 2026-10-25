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
		m_lisa.SetApu(&m_apu);
		m_lisa.GetWallet().Add(70);
	}

	Bank m_bank{ 1000000 };
	Lisa m_lisa{ 50 };
	Apu m_apu{ 200 };
};

TEST_F(LisaTest, SpendMoneyAtApu)
{
	const Money initialLisaCash = m_lisa.GetCash();
	const Money initialApuCash = m_apu.GetCash();
	m_lisa.DoAction(m_bank);
	EXPECT_EQ(m_lisa.GetCash(), initialLisaCash - 50);
	EXPECT_EQ(m_apu.GetCash(), initialApuCash + 50);
}

TEST_F(LisaTest, NotEnoughMoney)
{
	const Money initialLisaCash = m_lisa.GetCash();
	const Money initialApuCash = m_apu.GetCash();
	m_lisa.DoAction(m_bank);
	m_lisa.DoAction(m_bank);
	EXPECT_EQ(m_lisa.GetCash(), initialLisaCash - 50);
	EXPECT_EQ(m_apu.GetCash(), initialApuCash + 50);
}
