//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "src/Actors/Homer.h"
#include "src/Actors/MrBurns.h"
#include "src/Bank/Bank.h"
#include "src/Bank/BankOperationError.h"

#include <gtest/gtest.h>

class MrBurnsTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_bank = new Bank(1000000);
		m_mrBurns = new MrBurns(5000);
		m_homer = new Homer(1000, 200, 300);
		const AccountId mrBurnsId = m_bank->OpenAccount();
		const AccountId homerId = m_bank->OpenAccount();

		m_mrBurns->SetAccountId(mrBurnsId);
		m_homer->SetAccountId(homerId);
		m_mrBurns->AddEmployee(m_homer);
		m_bank->DepositMoney(mrBurnsId, 5000);
	}

	void TearDown() override
	{
		delete m_bank;
		delete m_mrBurns;
		delete m_homer;
	}

	Bank* m_bank = nullptr;
	MrBurns* m_mrBurns = nullptr;
	Homer* m_homer = nullptr;
};

TEST_F(MrBurnsTest, PaySalarySuccess)
{
	const AccountId mrBurnsId = m_mrBurns->GetAccountId().value();
	const AccountId homerId = m_homer->GetAccountId().value();
	const Money initialMrBurnsBalance = m_bank->GetAccountBalance(mrBurnsId);
	const Money initialHomerBalance = m_bank->GetAccountBalance(homerId);
	EXPECT_NO_THROW(m_mrBurns->DoAction(*m_bank));
	EXPECT_EQ(m_bank->GetAccountBalance(mrBurnsId), initialMrBurnsBalance - 5000);
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), initialHomerBalance + 5000);
}

TEST_F(MrBurnsTest, PaySalaryNotExistSrcAccount)
{
	const AccountId mrBurnsId = 100;
	EXPECT_THROW(m_bank->GetAccountBalance(mrBurnsId), BankOperationError);
	EXPECT_NO_THROW(m_mrBurns->DoAction(*m_bank));
}

TEST_F(MrBurnsTest, PaySalaryNotExistDstAccount)
{
	const AccountId homerId = 100;
	EXPECT_THROW(m_bank->GetAccountBalance(homerId), BankOperationError);
	EXPECT_NO_THROW(m_mrBurns->DoAction(*m_bank));
}

TEST_F(MrBurnsTest, PaySalaryDidntEnoughMoney)
{
	const AccountId mrBurnsId = m_mrBurns->GetAccountId().value();
	const AccountId homerId = m_homer->GetAccountId().value();
	const Money initialMrBurnsBalance = m_bank->GetAccountBalance(mrBurnsId);
	EXPECT_NO_THROW(m_mrBurns->DoAction(*m_bank));
	EXPECT_NO_THROW(m_mrBurns->DoAction(*m_bank));
	EXPECT_EQ(m_bank->GetAccountBalance(mrBurnsId), 0);
	EXPECT_EQ(m_bank->GetAccountBalance(homerId), initialMrBurnsBalance);
}