#ifndef ECONOMY_WALLET_H
#define ECONOMY_WALLET_H
#include "src/Bank/Bank.h"
#include <mutex>

class Wallet
{
public:
	void Add(Money amount);
	[[nodiscard]] bool TrySpend(Money amount);
	[[nodiscard]] Money GetBalance() const;

private:
	mutable std::mutex m_mutex;
	Money m_money = 0;
};

#endif // ECONOMY_WALLET_H
