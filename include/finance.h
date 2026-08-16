#ifndef FINANCE_H
#define FINANCE_H

#include "types.h"

#define INCOME_TAX 2000
#define LOAN_INTEREST_RATE 8
#define LOAN_DURATION 20

int takeLoan(struct Player *player, int amount);
int repayLoan(struct Player *player, int amount);

void updateLoans(struct Game *game);

int payTax(struct Player *player, int amount);

int buyInsurance(struct Player *player,
                 enum InsuranceType type,
                 int property_value);

void updateInsurance(struct Game *game);

void processInsuranceClaim(struct Player *player,
                           enum InsuranceType type);

#endif
