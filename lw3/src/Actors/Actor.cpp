//
// Created by Вадим Патрушев on 23.03.2026.
//

#include "Actor.h"
#include <iostream>
#include <mutex>

Actor::Actor(const std::string& name)
	: m_name(std::move(name))
{
}

const std::string& Actor::GetName() const
{
	return m_name;
}

Money Actor::GetCash() const
{
	return m_wallet.GetBalance();
}

Wallet& Actor::GetWallet()
{
	return m_wallet;
}

std::optional<AccountId> Actor::GetAccountId() const
{
	return m_accountId;
}

void Actor::SetAccountId(const AccountId accountId)
{
	m_accountId = accountId;
}

bool Actor::WithdrawToWallet(Bank& bank, const Money amount)
{
	if (!m_accountId.has_value())
	{
		return false;
	}
	if (!bank.TryWithdrawMoney(*m_accountId, amount))
	{
		return false;
	}
	m_wallet.Add(amount);
	return true;
}

bool Actor::DepositFromWallet(Bank& bank, const AccountId dst, const Money amount)
{
	if (!m_wallet.TrySpend(amount))
	{
		return false;
	}
	bank.DepositMoney(dst, amount);
	return true;
}

bool Actor::PayFromAccount(Bank& bank, const AccountId dst, const Money amount)
{
	return WithdrawToWallet(bank, amount) && DepositFromWallet(bank, dst, amount);
}

bool Actor::GiveCash(Actor& recipient, const Money amount)
{
	if (!m_wallet.TrySpend(amount))
	{
		return false;
	}
	recipient.m_wallet.Add(amount);
	return true;
}

void Actor::SetQuiet(const bool quiet)
{
	m_quiet = quiet;
}

void Actor::Print(const std::string& message) const
{
	if (m_quiet)
	{
		return;
	}
	std::string text = m_name + ": " + message + '\n';
	std::cout << text;
}
