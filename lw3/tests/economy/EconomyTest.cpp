//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Apu.h"
#include "src/Actors/Bart.h"
#include "src/Actors/Homer.h"
#include "src/Economy/Economy.h"
#include <gtest/gtest.h>
#include <sstream>

struct EconomyTest : public ::testing::Test
{
	EconomyTest()
		: economy(5) {};
	Economy economy;
};

TEST_F(EconomyTest, CreateBank)
{
	EXPECT_EQ(economy.GetBank()->GetCash(), 3000);
}
TEST_F(EconomyTest, CreateActors)
{
	EXPECT_FALSE(economy.GetActors().empty());
}

TEST_F(EconomyTest, AddActor)
{
	auto* homer = new Homer(1000, 200, 300);
	const size_t initialSize = economy.GetActors().size();
	economy.AddActor(homer);
	EXPECT_EQ(economy.GetActors().size(), initialSize + 1);
	EXPECT_EQ(economy.GetActors().back(), homer);
}

TEST_F(EconomyTest, WithoutErrors)
{
	EXPECT_NO_THROW(economy.StartEconomy());
}
TEST_F(EconomyTest, ConsistencyAfterSimulation)
{
	EXPECT_NO_THROW(economy.StartEconomy());
	EXPECT_NO_THROW(economy.PrintResult());
}

TEST_F(EconomyTest, NegativeIterations)
{
	EXPECT_THROW(Economy economy(-1), std::invalid_argument);
}
TEST_F(EconomyTest, TotalMoney)
{
	const Bank* bank = economy.GetBank();
	const Money initialCash = bank->GetCash();
	Money initialTotal = initialCash;
	for (const auto* actor : economy.GetActors())
	{
		if (actor->GetAccountId().has_value())
		{
			initialTotal += bank->GetAccountBalance(*actor->GetAccountId());
		}
	}
	economy.StartEconomy();
	const Money finalCash = bank->GetCash();
	Money finalTotal = finalCash;
	for (const auto* actor : economy.GetActors())
	{
		if (actor->GetAccountId().has_value())
		{
			finalTotal += bank->GetAccountBalance(*actor->GetAccountId());
		}
	}
	EXPECT_EQ(initialTotal, finalTotal);
}
