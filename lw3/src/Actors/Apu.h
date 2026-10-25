//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_APU_H
#define ECONOMY_APU_H
#include "Actor.h"

class Apu : public Actor
{
public:
	Apu(Money electricityBill);
	void DoAction(Bank& bank) override;
	void SetMrBurns(Actor* mrBurns);

private:
	Money m_electricityBill;
	Actor* m_mrBurns = nullptr;
	void DepositMoney(Bank& bank);
	void PayElectricityBill(Bank& bank) const;
};

#endif // ECONOMY_APU_H
