//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_APU_H
#define ECONOMY_APU_H
#include "Actor.h"

class Apu : public Actor
{
public:
	explicit Apu(Money electricityBill);
	void DoAction(Bank& bank) override;
	void SetMrBurns(Actor* mrBurns);
	void DepositMoney(Bank& bank);
	void PayElectricityBill(Bank& bank);

private:
	Money m_electricityBill;
	Actor* m_mrBurns = nullptr;
};

#endif // ECONOMY_APU_H
