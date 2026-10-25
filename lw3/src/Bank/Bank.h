//
// Created by Вадим Патрушев on 23.03.2026.
//

#ifndef ECONOMY_BANK_H
#define ECONOMY_BANK_H
#include <atomic>
#include <mutex>
#include <unordered_map>

using AccountId = unsigned long long;
using Money = long long;

class Bank
{
public:
	explicit Bank(Money cash);
	Bank(const Bank&) = delete;
	Bank& operator=(const Bank&) = delete;

	AccountId OpenAccount();
	Money CloseAccount(AccountId account);
	void DepositMoney(AccountId account, Money amount);
	void WithdrawMoney(AccountId account, Money amount);
	[[nodiscard]] bool TryWithdrawMoney(AccountId account, Money amount);
	[[nodiscard]] Money GetAccountBalance(AccountId account) const;
	[[nodiscard]] Money GetCash() const;
	[[nodiscard]] unsigned long long GetOperationsCount() const;

private:
	mutable std::mutex m_mutex;
	Money m_cash = 0;
	AccountId m_nextAccountId = 0;
	std::unordered_map<AccountId, Money> m_accountBalances;
	mutable std::atomic<unsigned long long> m_operationsCount{ 0 };

	Money& GetBalanceRef(AccountId account);
	const Money& GetBalanceRef(AccountId account) const;
	void CountOperation() const;
};

#endif // ECONOMY_BANK_H
