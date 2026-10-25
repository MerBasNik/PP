//
// Created by Вадим Патрушев on 23.03.2026.
//

#include "Bank.h"
#include "BankOperationError.h"

#include <iostream>

Bank::Bank(const Money cash)
{
	if (cash < 0)
	{
		throw BankOperationError("Cash cannot be negative");
	}
	m_cash = cash;
}

AccountId Bank::OpenAccount()
{
	AccountId accountId = m_currentAccountId++;
	m_accountBalances.emplace(accountId, 0);
	return accountId;
}

Money Bank::CloseAccount(const AccountId accountId)
{
	const Money money = GetAccountBalance(accountId);
	m_cash += money;
	m_accountBalances.erase(accountId);
	return money;
}

void Bank::DepositMoney(const AccountId accountId, const Money amount)
{
	if (m_cash < amount)
	{
		throw BankOperationError("amount greater than account balance");
	}
	if (amount < 0)
	{
		throw std::out_of_range("amount is negative");
	}
	Money& money = GetPointerToAccountBalance(accountId);
	m_cash -= amount;
	money += amount;
}

void Bank::WithdrawMoney(const AccountId accountId, const Money amount)
{
	const bool isWithdraw = TryWithdrawMoney(accountId, amount);
	if (!isWithdraw)
	{
		throw BankOperationError("Amount greater than account balance");
	}
}

bool Bank::TryWithdrawMoney(const AccountId accountId, const Money amount)
{
	Money& money = GetPointerToAccountBalance(accountId);
	if (money < amount)
	{
		return false;
	}
	if (amount < 0)
	{
		throw std::out_of_range("amount is negative");
	}
	money -= amount;
	m_cash += amount;
	return true;
}

Money Bank::GetAccountBalance(const AccountId accountId) const
{
	const auto account = m_accountBalances.find(accountId);
	if (account == m_accountBalances.end())
	{
		throw BankOperationError("account not found");
	}
	const Money money = account->second;
	return money;
}

Money& Bank::GetPointerToAccountBalance(const AccountId accountId)
{
	const auto account = m_accountBalances.find(accountId);
	if (account == m_accountBalances.end())
	{
		throw BankOperationError("account not found");
	}
	return account->second;
}

Money Bank::GetCash() const
{
	return m_cash;
}

// void Bank::SendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
// {
// 	Money& srcMoney = GetPointerToAccountBalance(srcAccountId);
// 	Money& dstMoney = GetPointerToAccountBalance(dstAccountId);
// 	if (srcMoney < amount)
// 	{
// 		throw BankOperationError("Amount greater than account balance");
// 	}
// 	if (amount < 0)
// 	{
// 		throw std::out_of_range("amount greater than account balance");
// 	}
// 	srcMoney -= amount;
// 	dstMoney += amount;
// }
//
// bool Bank::TrySendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
// {
// 	try
// 	{
// 		SendMoney(srcAccountId, dstAccountId, amount);
// 		return true;
// 	}
// 	catch (const std::out_of_range& e)
// 	{
// 		std::cout << e.what() << std::endl;
// 		return false;
// 	}
// 	catch (const BankOperationError& e)
// 	{
// 		std::cout << e.what() << std::endl;
// 		return false;
// 	}
// }

bool Bank::TrySendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
{
	Money& srcMoney = GetPointerToAccountBalance(srcAccountId);
	Money& dstMoney = GetPointerToAccountBalance(dstAccountId);
	if (srcMoney < amount)
	{
		return false;
	}
	if (amount < 0)
	{
		throw std::out_of_range("amount greater than account balance");
	}
	srcMoney -= amount;
	dstMoney += amount;
	return true;
}

void Bank::SendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
{
	const bool isSend = TrySendMoney(srcAccountId, dstAccountId, amount);
	if (!isSend)
	{
		throw BankOperationError("Amount greater than account balance");
	}
}
