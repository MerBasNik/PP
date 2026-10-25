//
// Created by Вадим Патрушев on 23.03.2026.
//

#ifndef ECONOMY_BANK_H
#define ECONOMY_BANK_H
#include <unordered_map>

using AccountId = unsigned long long;
using Money = long long;

class Bank
{
public:
	// Инициализирует монетарную систему. cash — количество денег в наличном обороте
	// При отрицательном количестве денег, выбрасывается BankOperationError
	explicit Bank(Money cash);
	Bank(const Bank&) = delete;
	Bank& operator=(const Bank&) = delete;

	// Перевести деньги с исходного счёта (srcAccountId) на целевой (dstAccountId)
	// Нельзя перевести больше, чем есть на исходном счёте
	// Нельзя перевести отрицательное количество денег
	// Исключение BankOperationError выбрасывается, при отсутствии счетов или
	// недостатке денег на исходном счёте
	// При отрицательном количестве переводимых денег выбрасывается std::out_of_range
	void SendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount);

	// Перевести деньги с исходного счёта (srcAccountId) на целевой (dstAccountId)
	// Нельзя перевести больше, чем есть на исходном счёте
	// Нельзя перевести отрицательное количество денег
	// При нехватке денег на исходном счёте возвращается false
	// Если номера счетов невалидны, выбрасывается BankOperationError
	// При отрицательном количестве денег выбрасывается std::out_of_range
	[[nodiscard]] bool TrySendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount);

	// Возвращает количество наличных денег в обороте
	[[nodiscard]] Money GetCash() const;

	// Сообщает о количестве денег на указанном счёте
	// Если указанный счёт отсутствует, выбрасывается исключение BankOperationError
	[[nodiscard]] Money GetAccountBalance(AccountId accountId) const;

	// Снимает деньги со счёта. Нельзя снять больше, чем есть на счете
	// Нельзя снять отрицательное количество денег
	// Снятые деньги переходят добавляются к массе наличных денег
	// При невалидном номере счёта или отсутствии денег, выбрасывается исключение BankOperationError
	// При отрицательном количестве денег выбрасывается std::out_of_range
	void WithdrawMoney(AccountId accountId, Money amount);

	// Попытаться снять деньги в размере amount со счёта account.
	// Объем денег в наличном обороте увеличивается на величину amount
	// При нехватке денег на счёте возвращается false, а количество наличных денег остаётся неизменным
	// При невалидном номере аккаунта выбрасывается BankOperationError.
	// При отрицательном количестве денег выбрасывается std::out_of_range
	[[nodiscard]] bool TryWithdrawMoney(AccountId account, Money amount);

	// Поместить наличные деньги на счёт. Количество денег в наличном обороте
	// уменьшается на величину amount.
	// Нельзя поместить больше, чем имеется денег в наличном обороте
	// Нельзя поместить на счёт отрицательное количество денег
	// Нельзя поместить деньги на отсутствующий счёт
	// При невалидном номере аккаунта или нехватке наличных денег в обороте выбрасывается BankOperationError.
	// При отрицательном количестве денег выбрасывается std::out_of_range
	void DepositMoney(AccountId account, Money amount);

	// Открывает счёт в банке. После открытия счёта на нём нулевой баланс.
	// Каждый открытый счёт имеет уникальный номер.
	// Возвращает номер счёта
	[[nodiscard]] AccountId OpenAccount();

	// Закрывает указанный счёт.
	// Возвращает количество денег, которые были на счёте в момент закрытия
	// Эти деньги переходят в наличный оборот
	[[nodiscard]] Money CloseAccount(AccountId accountId);

	Money& GetPointerToAccountBalance(AccountId accountId);

private:
	AccountId m_currentAccountId = 0;
	Money m_cash = 0;
	std::unordered_map<AccountId, Money> m_accountBalances;
};

#endif // ECONOMY_BANK_H
