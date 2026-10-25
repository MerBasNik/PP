//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_MARGE_H
#define ECONOMY_MARGE_H
#include "Customer.h"

class Marge : public Customer
{
public:
	explicit Marge(Money purchase);
};

#endif // ECONOMY_MARGE_H
