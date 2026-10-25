//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Bart.h"
#include <iostream>

Bart::Bart(const Money initMoney, const Money purchase)
	: Actor("Bart")
	, m_purchase(purchase)
{
	AddCash(initMoney);
}

void Bart::SetApu(Actor* apu)
{
	m_apu = apu;
}

void Bart::DoAction(Bank& bank)
{
	if (!m_apu)
	{
		return;
	}
	const Money bartCash = GetCash();
	if (bartCash > 0)
	{
		if (RemoveCash(m_purchase))
		{
			m_apu->AddCash(m_purchase);
			std::cout << GetName() << ": bought something in the Apu's store" << std::endl;
			return;
		}
	}
	std::cout << GetName() << ": didn't buy nothing" << std::endl;
}