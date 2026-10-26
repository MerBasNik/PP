//
// Created by Вадим Патрушев on 23.03.2026.
//

#include "Bank.h"
#include "BankOperationError.h"
#include <stdexcept>
#include <string>

void Bank::VerifyBank(const Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("amount is negative");
	}
}

Bank::Bank(const Money cash)
	: m_cash(cash)
{
	VerifyBank(cash);
}

AccountId Bank::OpenAccount()
{
	CountOperation();
	std::lock_guard lock(m_mutex);

	const AccountId accountId = m_nextAccountId;
	m_accountBalances.emplace(accountId, 0);
	++m_nextAccountId;
	return accountId;
}

Money Bank::CloseAccount(const AccountId account)
{
	CountOperation();
	std::lock_guard lock(m_mutex);

	const Money balance = GetBalanceRef(account);
	m_accountBalances.erase(account);
	m_cash += balance;
	return balance;
}

void Bank::DepositMoney(const AccountId account, const Money amount)
{
	CountOperation();
	VerifyBank(amount);
	std::lock_guard lock(m_mutex);

	Money& balance = GetBalanceRef(account);
	if (m_cash < amount)
	{
		throw BankOperationError("not enough cash to deposit");
	}
	m_cash -= amount;
	balance += amount;
}

void Bank::WithdrawMoney(const AccountId account, const Money amount)
{
	CountOperation();
	VerifyBank(amount);
	std::lock_guard lock(m_mutex);

	Money& balance = GetBalanceRef(account);
	if (balance < amount)
	{
		throw BankOperationError("amount greater than account balance");
	}
	balance -= amount;
	m_cash += amount;
}

bool Bank::TryWithdrawMoney(const AccountId account, const Money amount)
{
	CountOperation();
	VerifyBank(amount);
	std::lock_guard lock(m_mutex);

	Money& balance = GetBalanceRef(account);
	if (balance < amount)
	{
		return false;
	}
	balance -= amount;
	m_cash += amount;
	return true;
}

Money Bank::GetAccountBalance(const AccountId account) const
{
	CountOperation();
	std::lock_guard lock(m_mutex);
	return GetBalanceRef(account);
}

Money Bank::GetCash() const
{
	CountOperation();
	std::lock_guard lock(m_mutex);
	return m_cash;
}

unsigned long long Bank::GetOperationsCount() const
{
	return m_operationsCount.load();
}

Money& Bank::GetBalanceRef(const AccountId account)
{
	const auto it = m_accountBalances.find(account);
	if (it == m_accountBalances.end())
	{
		throw BankOperationError("account " + std::to_string(account) + " not found");
	}
	return it->second;
}

const Money& Bank::GetBalanceRef(const AccountId account) const
{
	return const_cast<Bank*>(this)->GetBalanceRef(account);
}

void Bank::CountOperation() const
{
	++m_operationsCount;
}
