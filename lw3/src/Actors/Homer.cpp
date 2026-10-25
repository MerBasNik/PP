//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Homer.h"

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
	PayElectricityBill(bank);
	WithdrawFamilyCash(bank);
	GiveMoneyToFamily();
}

void Homer::PayElectricityBill(Bank& bank)
{
	if (!m_mrBurns)
	{
		return;
	}
	const auto mrBurnsAccountId = m_mrBurns->GetAccountId();
	if (mrBurnsAccountId.has_value() && PayFromAccount(bank, *mrBurnsAccountId, m_electricityBill))
	{
		Log("paid electricity bill {}", m_electricityBill);
	}
	else
	{
		Log("skips turn: can't pay electricity bill {}", m_electricityBill);
	}
}

void Homer::WithdrawFamilyCash(Bank& bank)
{
	const Money amount = m_moneyForMarge + m_moneyForChildren;
	if (WithdrawToWallet(bank, amount))
	{
		Log("withdrew {} from account", amount);
	}
	else
	{
		Log("skips turn: not enough money on account to withdraw {}", amount);
	}
}

void Homer::GiveMoneyToFamily()
{
	GiveMoneyTo(m_marge, m_moneyForMarge);
	GiveMoneyTo(m_bart, m_moneyForChildren / 2);
	GiveMoneyTo(m_lisa, m_moneyForChildren / 2);
}

void Homer::GiveMoneyTo(Actor* recipient, const Money amount)
{
	if (!recipient)
	{
		return;
	}
	if (GiveCash(*recipient, amount))
	{
		Log("gave {} to {}", amount, recipient->GetName());
	}
	else
	{
		Log("skips turn: not enough cash to give {} to {}", amount, recipient->GetName());
	}
}
