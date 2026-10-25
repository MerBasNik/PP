//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Marge.h"

#include "src/Bank/BankOperationError.h"

#include <iostream>
#include <ostream>
Marge::Marge(const Money purchase)
	: Actor("Marge")
	, m_purchase(purchase)
{
}

void Marge::SetApu(Actor* apu)
{
	m_apu = apu;
}

void Marge::DoAction(Bank& bank)
{
	if (!m_apu)
	{
		return;
	}
	const auto margeAccountId = GetAccountId();
	const auto apuAccountId = m_apu->GetAccountId();
	if (margeAccountId.has_value() && apuAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*margeAccountId, *apuAccountId, m_purchase);
			std::cout << GetName() << ": bought products in the Apu's store" << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": didn't buy products in the Apu's store" << std::endl;
	}
}