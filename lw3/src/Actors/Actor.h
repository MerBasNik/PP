//
// Created by Вадим Патрушев on 23.03.2026.
//

#ifndef ECONOMY_ACTOR_H
#define ECONOMY_ACTOR_H
#include "src/Bank/Bank.h"
#include "src/Logger/Logger.h"
#include "src/Wallet/Wallet.h"
#include <format>
#include <optional>
#include <string>

class Actor
{
public:
	explicit Actor(std::string name);
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;
	virtual ~Actor() = default;
	virtual void DoAction(Bank& bank) = 0;
	[[nodiscard]] const std::string& GetName() const;
	[[nodiscard]] Money GetCash() const;
	[[nodiscard]] Wallet& GetWallet();
	[[nodiscard]] std::optional<AccountId> GetAccountId() const;
	void SetAccountId(AccountId accountId);
	void SetLogger(Logger* logger);

protected:
	bool WithdrawToWallet(Bank& bank, Money amount);
	bool DepositFromWallet(Bank& bank, AccountId dst, Money amount);
	bool PayFromAccount(Bank& bank, AccountId dst, Money amount);
	bool GiveCash(Actor& recipient, Money amount);
	template <typename... Args>
	void Log(std::format_string<Args...> fmt, Args&&... args) const
	{
		if (m_logger)
		{
			m_logger->Print(m_name + ": " + std::format(fmt, std::forward<Args>(args)...));
		}
	}

private:
	std::string m_name;
	Wallet m_wallet;
	std::optional<AccountId> m_accountId;
	Logger* m_logger = nullptr;
};

#endif // ECONOMY_ACTOR_H
