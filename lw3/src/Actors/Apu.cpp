//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Apu.h"
#include <iostream>

Apu::Apu(const Money electricityBill)
	: Actor("Apu")
	, m_electricityBill(electricityBill)
{
}

void Apu::SetMrBurns(Actor* mrBurns)
{
	m_mrBurns = mrBurns;
}

void Apu::DoAction(Bank& bank)
{
	DepositMoney(bank);
	PayElectricityBill(bank);
}

void Apu::DepositMoney(Bank& bank)
{
	const Money apuCash = GetCash();
	const auto apuAccountId = GetAccountId();
	if (apuCash > 0 && apuAccountId.has_value())
	{
		try
		{
			if (RemoveCash(apuCash))
			{
				bank.DepositMoney(*apuAccountId, apuCash);
				std::cout << GetName() << ": successfully deposit money" << std::endl;
			}
			else
			{
				std::cout << GetName() << ": didn't deposit money" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": didn't deposit money" << std::endl;
	}
}

void Apu::PayElectricityBill(Bank& bank) const
{
	const auto apuAccountId = GetAccountId();
	const auto mrBurnsAccountId = m_mrBurns->GetAccountId();
	if (mrBurnsAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*apuAccountId, *mrBurnsAccountId, m_electricityBill);
			std::cout << GetName() << ": successfully pay electricity bill" << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
}