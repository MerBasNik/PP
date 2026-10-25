//
// Created by Вадим Патрушев on 31.03.2026.
//

#ifndef ECONOMY_CHESTER_H
#define ECONOMY_CHESTER_H
#include "Actor.h"
#include "Apu.h"
#include "Homer.h"

class Chester : public Actor
{
public:
	Chester(Money hackMoney, Money purchaseMoney);
	void DoAction(Bank& bank) override;
	void SetHomer(Homer* homer);
	void SetApu(Apu* apu);
	void HackHomer(Bank& bank) const;
	void BuyPurchase(Bank& bank) const;

private:
	Homer* m_homer = nullptr;
	Apu* m_apu = nullptr;
	Money m_stealMoney;
	Money m_purchase;
};

#endif // ECONOMY_CHESTER_H
