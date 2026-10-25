//
// Created by Вадим Патрушев on 23.03.2026.
//

#ifndef ECONOMY_ACTOR_H
#define ECONOMY_ACTOR_H
#include "../Bank/Bank.h"
#include <string>

class Actor
{
public:
	explicit Actor(std::string name);
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;
	virtual ~Actor() = default;

	virtual void DoAction(Bank& bank) = 0;
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] Money GetCash() const;
	[[nodiscard]] std::optional<AccountId> GetAccountId() const;
	void SetAccountId(AccountId accountId);
	void AddCash(Money amount);
	[[nodiscard]] bool RemoveCash(Money amount);

private:
	std::string m_name;
	Money m_cash = 0;
	std::optional<AccountId> m_accountId;
};

#endif // ECONOMY_ACTOR_H
