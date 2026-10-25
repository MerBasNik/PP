//
// Created by Вадим Патрушев on 31.03.2026.
//

#include "Chester.h"
#include <iostream>

Chester::Chester(const Money hackMoney, const Money purchaseMoney)
	: Actor("Chester")
	, m_stealMoney(hackMoney)
	, m_purchase(purchaseMoney)
{
}

void Chester::SetApu(Apu* apu)
{
	m_apu = apu;
}
void Chester::SetHomer(Homer* homer)
{
	m_homer = homer;
}

void Chester::DoAction(Bank& bank)
{
	if (rand() % 100 < 50)
	{
		HackHomer(bank);
	}
	BuyPurchase(bank);
}

void Chester::HackHomer(Bank& bank) const
{
	if (!m_homer)
	{
		return;
	}
	const auto chesterAccountId = GetAccountId();
	const auto homerAccountId = m_homer->GetAccountId();
	if (chesterAccountId.has_value() && homerAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*homerAccountId, *chesterAccountId, m_stealMoney);
			std::cout << GetName() << ": hacked and steal money from " << m_homer->GetName() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": did not hack " << m_homer->GetName() << std::endl;
	}
}

void Chester::BuyPurchase(Bank& bank) const
{
	if (!m_apu)
	{
		return;
	}
	const auto chesterAccountId = GetAccountId();
	const auto apuAccountId = m_apu->GetAccountId();
	if (apuAccountId.has_value() && chesterAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*chesterAccountId, *apuAccountId, m_purchase);
			std::cout << GetName() << ": bought products in the Apu's store" << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
}