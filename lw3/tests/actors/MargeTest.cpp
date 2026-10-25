//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Marge.h"
#include "src/Bank/Bank.h"
#include <gtest/gtest.h>

class MargeTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_marge.SetApu(&m_apu);
		m_marge.GetWallet().Add(700);
	}

	Bank m_bank{ 1000000 };
	Marge m_marge{ 500 };
	Apu m_apu{ 200 };
};

TEST_F(MargeTest, BuyProducts)
{
	const auto operationsBefore = m_bank.GetOperationsCount();
	m_marge.DoAction(m_bank);
	EXPECT_EQ(m_marge.GetCash(), 200);
	EXPECT_EQ(m_apu.GetCash(), 500);
	EXPECT_EQ(m_bank.GetOperationsCount(), operationsBefore);
}

TEST_F(MargeTest, BuyProductsDidntEnoughMoney)
{
	m_marge.DoAction(m_bank);
	EXPECT_NO_THROW(m_marge.DoAction(m_bank));
	EXPECT_EQ(m_marge.GetCash(), 200);
	EXPECT_EQ(m_apu.GetCash(), 500);
}

TEST_F(MargeTest, WithoutApu)
{
	Marge marge(500);
	marge.GetWallet().Add(700);
	EXPECT_NO_THROW(marge.DoAction(m_bank));
	EXPECT_EQ(marge.GetCash(), 700);
}
