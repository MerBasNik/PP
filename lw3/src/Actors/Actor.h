//
// Created by Вадим Патрушев on 23.03.2026.
//

#ifndef ECONOMY_ACTOR_H
#define ECONOMY_ACTOR_H
#include "src/Bank/Bank.h"
#include "src/Wallet/Wallet.h"
#include <optional>
#include <string>

class Actor
{
public:
	explicit Actor(const std::string& name);
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;
	virtual ~Actor() = default;
	virtual void DoAction(Bank& bank) = 0;
	[[nodiscard]] const std::string& GetName() const;
	[[nodiscard]] Money GetCash() const;
	[[nodiscard]] Wallet& GetWallet();
	[[nodiscard]] std::optional<AccountId> GetAccountId() const;
	void SetAccountId(AccountId accountId);
	void SetQuiet(bool quiet);

protected:
	bool WithdrawToWallet(Bank& bank, Money amount);
	bool DepositFromWallet(Bank& bank, AccountId dst, Money amount);
	bool PayFromAccount(Bank& bank, AccountId dst, Money amount);
	bool GiveCash(Actor& recipient, Money amount);
	void Print(const std::string& message) const;

private:
	std::string m_name;
	Wallet m_wallet;
	std::optional<AccountId> m_accountId;
	bool m_quiet = true;
};

#endif // ECONOMY_ACTOR_H
