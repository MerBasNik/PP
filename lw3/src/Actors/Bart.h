//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_BART_H
#define ECONOMY_BART_H
#include "Actor.h"

class Bart : public Actor
{
public:
	Bart(Money initMoney, Money purchase);
	void DoAction(Bank& bank) override;
	void SetApu(Actor* apu);

private:
	Money m_purchase;
	Actor* m_apu = nullptr;
};

#endif // ECONOMY_BART_H
