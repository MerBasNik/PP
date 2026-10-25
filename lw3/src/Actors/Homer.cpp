//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Homer.h"
#include <iostream>

Homer::Homer(const Money moneyForMarge,
	const Money electricityBill, const Money moneyForChildren)
	: Actor("Homer")
	, m_moneyForChildren(moneyForChildren)
	, m_electricityBill(electricityBill)
	, m_moneyForMarge(moneyForMarge)
{
}

void Homer::SetBart(Actor* bart)
{
	m_bart = bart;
}

void Homer::SetLisa(Actor* lisa)
{
	m_lisa = lisa;
}

void Homer::SetMarge(Actor* marge)
{
	m_marge = marge;
}

void Homer::SetMrBurns(Actor* mrBurns)
{
	m_mrBurns = mrBurns;
}

void Homer::DoAction(Bank& bank)
{
	SendMargeMoney(bank);
	PayElectricityBill(bank);
	GetChildrenMoney(bank);
}

void Homer::PayElectricityBill(Bank& bank) const
{
	if (!m_mrBurns)
	{
		return;
	}
	const auto homerAccountId = GetAccountId();
	const auto mrBurnsAccountId = m_mrBurns->GetAccountId();
	if (mrBurnsAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*homerAccountId, *mrBurnsAccountId, m_electricityBill);
			std::cout << GetName() << ": pay electricity bill" << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": didn't pay electricity bill" << std::endl;
	}
}

void Homer::SendMargeMoney(Bank& bank) const
{
	if (!m_marge)
	{
		return;
	}
	const auto homerAccountId = GetAccountId();
	const auto margeAccountId = m_marge->GetAccountId();
	if (homerAccountId.has_value() && margeAccountId.has_value())
	{
		try
		{
			bank.SendMoney(*homerAccountId, *margeAccountId, m_moneyForMarge);
			std::cout << GetName() << ": sent money to " << m_marge->GetName() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": didn't send money to " << m_marge->GetName() << std::endl;
	}
}

void Homer::GetChildrenMoney(Bank& bank) const
{
	if (!m_bart || !m_lisa)
	{
		return;
	}
	const auto homerAccountId = GetAccountId();
	if (homerAccountId.has_value())
	{
		try
		{
			bank.WithdrawMoney(*homerAccountId, m_moneyForChildren);
			m_bart->AddCash(m_moneyForChildren / 2);
			m_lisa->AddCash(m_moneyForChildren / 2);
			std::cout << GetName() << ": got money for children" << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << GetName() << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << GetName() << ": didn't get money for children" << std::endl;
	}
}