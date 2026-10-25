//
// Created by Вадим Патрушев on 31.03.2026.
//

#ifndef ECONOMY_SMITHERS_H
#define ECONOMY_SMITHERS_H
#include "Actor.h"
#include "Apu.h"

class Smithers : public Actor
{
public:
	explicit Smithers(Money purchase);
	void DoAction(Bank& bank) override;
	void SetApu(Apu* apu);
	void ReopenAccount(Bank& bank);
	void BuyPurchase(Bank& bank) const;

private:
	Money m_purchase;
	Apu* m_apu = nullptr;
};

#endif // ECONOMY_SMITHERS_H
