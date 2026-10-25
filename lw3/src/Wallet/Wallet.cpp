#include "Wallet.h"
#include <stdexcept>

static void ThrowIfNegative(const Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("amount is negative");
	}
}

void Wallet::Add(const Money amount)
{
	ThrowIfNegative(amount);
	std::lock_guard lock(m_mutex);
	m_money += amount;
}

bool Wallet::TrySpend(const Money amount)
{
	ThrowIfNegative(amount);
	std::lock_guard lock(m_mutex);
	if (m_money < amount)
	{
		return false;
	}
	m_money -= amount;
	return true;
}

Money Wallet::GetBalance() const
{
	std::lock_guard lock(m_mutex);
	return m_money;
}
