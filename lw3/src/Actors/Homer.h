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

private:
	Money m_moneyForChildren;
	Money m_electricityBill;
	Money m_moneyForMarge;

	Actor* m_marge = nullptr;
	Actor* m_bart = nullptr;
	Actor* m_lisa = nullptr;
	Actor* m_mrBurns = nullptr;

	void PayElectricityBill(Bank& bank) const;
	void SendMargeMoney(Bank& bank) const;
	void GetChildrenMoney(Bank& bank) const;
};

#endif // ECONOMY_HOMER_H
