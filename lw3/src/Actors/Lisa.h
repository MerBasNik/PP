//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_LISA_H
#define ECONOMY_LISA_H
#include "Actor.h"

class Lisa : public Actor
{
public:
	Lisa(Money initMoney, Money purchase);
	void DoAction(Bank& bank) override;
	void SetApu(Actor* apu);

private:
	Money m_purchase;
	Actor* m_apu = nullptr;
};

#endif // ECONOMY_LISA_H
