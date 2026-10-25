//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_MARGE_H
#define ECONOMY_MARGE_H
#include "Actor.h"
#include "Apu.h"

class Marge : public Actor
{
public:
	explicit Marge(Money purchase);
	void DoAction(Bank& bank) override;
	void SetApu(Actor* apu);

private:
	Money m_purchase;
	Actor* m_apu = nullptr;
};

#endif // ECONOMY_MARGE_H
