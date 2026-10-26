//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Apu.h"

#include <format>

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
	DepositRevenue(bank);
	PayElectricityBill(bank);
}

void Apu::DepositRevenue(Bank& bank)
{
	const auto apuAccountId = GetAccountId();
	const Money revenue = GetCash();
	if (apuAccountId.has_value() && revenue > 0 && DepositFromWallet(bank, *apuAccountId, revenue))
	{
		const std::string message = std::format("deposited revenue {}", revenue);
		Print(message);
	}
	else
	{
		const std::string message = "skips turn: no revenue to deposit";
		Print(message);
	}
}

void Apu::PayElectricityBill(Bank& bank)
{
	if (!m_mrBurns)
	{
		return;
	}
	const auto mrBurnsAccountId = m_mrBurns->GetAccountId();
	if (mrBurnsAccountId.has_value() && PayFromAccount(bank, *mrBurnsAccountId, m_electricityBill))
	{
		const std::string message = std::format("paid electricity bill {}", m_electricityBill);
		Print(message);
	}
	else
	{
		const std::string message = std::format("skips turn: can't pay electricity bill {}", m_electricityBill);
		Print(message);
	}
}
