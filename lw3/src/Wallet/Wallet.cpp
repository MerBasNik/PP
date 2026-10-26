#include "Wallet.h"
#include <stdexcept>

void Wallet::ValidateWallet(const Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("amount is negative");
	}
}

void Wallet::Add(const Money amount)
{
	ValidateWallet(amount);
	std::lock_guard lock(m_mutex);
	m_money += amount;
}

bool Wallet::TrySpend(const Money amount)
{
	ValidateWallet(amount);
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
