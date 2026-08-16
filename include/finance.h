#ifndef FINANCE_H
#define FINANCE_H

#include "types.h"

#define INCOME_TAX 2000
#define LOAN_DURATION 20

int takeLoan(struct Player *player, int amount, int rate);
int repayLoan(struct Player *player, int amount);

int calculateMaxLoan(const struct Board *board, const struct Player *player);

void updateLoans(struct Game *game);

int payTax(struct Player *player, int amount);

int buyInsurance(struct Player *player,
                 enum InsuranceType type,
                 int property_value,
                 int property_index);

void updateInsurance(struct Game *game);

void processInsuranceClaim(struct Player *player,
                           enum InsuranceType type);

#endif
