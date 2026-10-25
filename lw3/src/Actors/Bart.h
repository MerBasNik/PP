//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_BART_H
#define ECONOMY_BART_H
#include "Customer.h"

class Bart : public Customer
{
public:
	explicit Bart(Money purchase);
};

#endif // ECONOMY_BART_H
