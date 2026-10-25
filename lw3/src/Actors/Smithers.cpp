//
// Created by Вадим Патрушев on 31.03.2026.
//

#include "Smithers.h"
#include <iostream>

Smithers::Smithers(const Money purchase)
	: Actor("Smithers")
	, m_purchase(purchase)
{
}

void Smithers::SetApu(Apu* apu)
{
	m_apu = apu;
}

void Smithers::DoAction(Bank& bank)
{
	if (rand() % 100 < 50)
	{
		ReopenAccount(bank);
	}
	BuyPurchase(bank);
}

void Smithers::BuyPurchase(Bank& bank) const
{
	if (!m_apu)
	{
		return;
	}
	const auto smithersAccountId = GetAccountId();
	const auto apuAccountId = m_apu->GetAccountId();
	if (smithersAccountId.has_value() && apuAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*smithersAccountId, *apuAccountId, m_purchase);
			std::cout << GetName() << ": bought products in the Apu's store" << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": did not buy nothing in the Apu' store" << std::endl;
	}
}

void Smithers::ReopenAccount(Bank& bank)
{
	const auto smithersAccountId = GetAccountId();
	if (smithersAccountId.has_value())
	{
		try
		{
			const Money oldBalance = bank.CloseAccount(GetAccountId().value());
			std::cout << GetName() << ": closed account with balance " << oldBalance << std::endl;
			const auto newAccountId = bank.OpenAccount();
			SetAccountId(newAccountId);
			if (oldBalance > 0)
			{
				bank.DepositMoney(newAccountId, oldBalance);
				std::cout << GetName() << ": opened new account" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": account reopening failed - " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": did not open new account" << std::endl;
	}
}