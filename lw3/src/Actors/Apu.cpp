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
	DepositMoney(bank);
	PayElectricityBill(bank);
}

void Apu::DepositMoney(Bank& bank)
{
	const auto accountId = GetAccountId();
	const Money money = GetCash();
	if (accountId.has_value() && money > 0 && DepositFromWallet(bank, *accountId, money))
	{
		const std::string message = std::format("deposited revenue {}", money);
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
	const auto accountId = m_mrBurns->GetAccountId();
	if (accountId.has_value() && PayFromAccount(bank, *accountId, m_electricityBill))
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
