//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_HOMER_H
#define ECONOMY_HOMER_H
#include "Actor.h"

class Homer : public Actor
{
public:
	Homer(Money moneyForMarge, Money electricityBill, Money moneyForChildren);
	void DoAction(Bank& bank) override;
	void SetMarge(Actor* marge);
	void SetBart(Actor* bart);
	void SetLisa(Actor* lisa);
	void SetMrBurns(Actor* mrBurns);

	void PayElectricityBill(Bank& bank);
	void WithdrawFamilyCash(Bank& bank);
	void GiveMoneyToFamily();

private:
	Money m_moneyForChildren;
	Money m_electricityBill;
	Money m_moneyForMarge;

	Actor* m_marge = nullptr;
	Actor* m_bart = nullptr;
	Actor* m_lisa = nullptr;
	Actor* m_mrBurns = nullptr;

	void GiveMoneyTo(Actor* recipient, Money amount);
};

#endif // ECONOMY_HOMER_H
