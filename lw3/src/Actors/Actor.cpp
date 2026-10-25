//
// Created by Вадим Патрушев on 23.03.2026.
//

#include "Actor.h"

Actor::Actor(std::string name)
	: m_name(std::move(name))
{
}

Money Actor::GetCash() const
{
	return m_cash;
}

std::optional<AccountId> Actor::GetAccountId() const
{
	return m_accountId;
}

std::string Actor::GetName() const
{
	return m_name;
}

void Actor::SetAccountId(const AccountId accountId)
{
	m_accountId = accountId;
}

void Actor::AddCash(const Money amount)
{
	if (amount >= 0)
	{
		m_cash += amount;
	}
}

bool Actor::RemoveCash(const Money amount)
{
	if (m_cash < amount)
	{
		return false;
	}
	m_cash -= amount;
	return true;
}
