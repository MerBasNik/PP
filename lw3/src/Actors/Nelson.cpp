//
// Created by Вадим Патрушев on 31.03.2026.
//

#include "Nelson.h"
#include <iostream>

Nelson::Nelson(const Money stealMoney)
	: Actor("Nelson")
	, m_stealMoney(stealMoney)
{
}

void Nelson::SetApu(Apu* apu)
{
	m_apu = apu;
}
void Nelson::SetBart(Bart* bart)
{
	m_bart = bart;
}

void Nelson::DoAction(Bank& bank)
{
	if (rand() % 100 < 50)
	{
		StealMoney();
		BuyPurchase();
	}
}

void Nelson::StealMoney()
{
	if (!m_bart)
	{
		return;
	}
	const Money bartCash = m_bart->GetCash();
	const Money stealAmount = std::min(m_stealMoney, bartCash);
	if (stealAmount > 0)
	{
		if (m_bart->RemoveCash(stealAmount))
		{
			AddCash(stealAmount);
			std::cout << GetName() << ": steal money from Bart" << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": Bart hasn't enough money to steal" << std::endl;
	}
}

void Nelson::BuyPurchase()
{
	if (!m_apu)
	{
		return;
	}
	const Money nelsonCash = GetCash();
	if (nelsonCash > 0)
	{
		if (RemoveCash(nelsonCash))
		{
			m_apu->AddCash(nelsonCash);
			std::cout << GetName() << ": bought cigarettes in the Apu's store" << std::endl;
			return;
		}
	}
	std::cout << GetName() << ": didn't buy nothing" << std::endl;
}