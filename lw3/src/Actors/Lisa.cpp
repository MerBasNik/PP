//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Lisa.h"
#include <iostream>

Lisa::Lisa(const Money initMoney, const Money purchase)
	: Actor("Lisa")
	, m_purchase(purchase)
{
	AddCash(initMoney);
}

void Lisa::SetApu(Actor* apu)
{
	m_apu = apu;
}

void Lisa::DoAction(Bank& bank)
{
	if (!m_apu)
	{
		return;
	}
	const Money lisaCash = GetCash();
	if (lisaCash > 0)
	{
		if (RemoveCash(m_purchase))
		{
			m_apu->AddCash(m_purchase);
			std::cout << GetName() << ": bought something in the Apu's store" << std::endl;
			return;
		};
	}
	std::cout << GetName() << ": didn't buy nothing" << std::endl;
}