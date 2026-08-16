#include <stdio.h>

#include "../include/finance.h"

int takeLoan(struct Player *player, int amount, int rate)
{
    if (player->loan.active) {
        printf("%s already has an active loan.\n", player->name);
        return 0;
    }

    if (amount <= 0) {
        return 0;
    }

    player->loan.active = 1;
    player->loan.principal = amount;
    player->loan.interest = 0;
    player->loan.rate = rate;
    player->loan.rounds_remaining = LOAN_DURATION;

    player->cash += amount;

    printf("%s obtained a secured loan.\n\nLoan Amount : LKR %d.\n\nInterest Rate : %d%%\nDuration : %d Rounds\n\n",
           player->name, amount, rate, LOAN_DURATION);

    return 1;
}

int calculateMaxLoan(const struct Board *board, const struct Player *player)
{
    int total_collateral = 0;

    for (int i = 0; i < player->property_count; i++) {
        int idx = player->properties[i];
        if (idx < 0) continue;

        const struct Property *p = &board->properties[idx];
        if (!p->mortgaged) {
            total_collateral += p->mortgage_value;
        }
    }

    for (int i = 0; i < player->railway_count; i++) {
        int idx = player->railways[i];
        if (idx < 0) continue;

        const struct Railway *r = &board->railways[idx];
        if (!r->mortgaged) {
            total_collateral += r->mortgage_value;
        }
    }

    for (int i = 0; i < player->utility_count; i++) {
        int idx = player->utilities[i];
        if (idx < 0) continue;

        const struct Utility *u = &board->utilities[idx];
        if (!u->mortgaged) {
            total_collateral += u->mortgage_value;
        }
    }

    return (total_collateral * 75) / 100;
}

int repayLoan(struct Player *player, int amount)
{
    if (!player->loan.active) {
        printf("%s has no active loan.\n", player->name);
        return 0;
    }

    if (amount <= 0) {
        return 0;
    }

    int total = player->loan.principal + player->loan.interest;

    if (amount > total) {
        amount = total;
    }

    if (player->cash < amount) {
        printf("%s does not have enough cash to repay the loan.\n", player->name);
        return 0;
    }

    player->cash -= amount;

    if (amount >= player->loan.interest) {
        amount -= player->loan.interest;
        player->loan.interest = 0;
        player->loan.principal -= amount;
    }
    else {
        player->loan.interest -= amount;
    }

    if (player->loan.principal <= 0 && player->loan.interest <= 0) {
        player->loan.active = 0;
        player->loan.rounds_remaining = 0;
        player->loan.principal = 0;
        player->loan.interest = 0;

        printf("%s repaid LKR %d.\n\n%s fully repaid the loan.\n\n", player->name, amount, player->name);
    }
    else {
        printf("%s repaid LKR %d.\n\nOutstanding Balance :\nLKR %d.\n\n",
               player->name, amount, player->loan.principal + player->loan.interest);
    }

    return 1;
}

void updateLoans(struct Game *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        struct Player *player = &game->players[i];

        if (!player->loan.active) {
            continue;
        }

        int outstanding = player->loan.principal + player->loan.interest;
        int interest = (outstanding * player->loan.rate) / 100;

        player->loan.interest += interest;
        player->loan.rounds_remaining--;

        printf("%s loan interest: LKR %d\n", player->name, interest);
        printf("Outstanding loan: LKR %d\n",
               player->loan.principal + player->loan.interest);

        if (player->loan.rounds_remaining <= 0) {

            printf("%s has defaulted.\n\nCollateral has been foreclosed.\n\nOutstanding debt cleared.\n\n",
                   player->name);

            /* Foreclose: return pledged collateral to bank, demolish buildings,
               cancel insurance, clear debt. Buildings/insurance already tracked
               on the property/player structs, so reset the relevant fields. */
            for (int p = 0; p < player->property_count; p++) {
                int idx = player->properties[p];
                if (idx < 0) continue;

                struct Property *prop = &game->board.properties[idx];
                if (prop->loan_locked) {
                    prop->owner = -1;
                    prop->houses = 0;
                    prop->hotel = 0;
                    prop->loan_locked = 0;
                    prop->mortgaged = 0;
                }
            }

            player->insurance.active = 0;

            player->loan.active = 0;
            player->loan.principal = 0;
            player->loan.interest = 0;
            player->loan.rounds_remaining = 0;
        }
    }
}

int payTax(struct Player *player, int amount)
{
    if (amount <= 0) {
        return 0;
    }

    if (player->cash >= amount) {
        printf("%s pays LKR %d in tax.\n", player->name, amount);
        player->cash -= amount;
        return 1;
    }

    player->taxes_due += amount;

    printf("%s cannot pay LKR %d in tax.\n", player->name, amount);

    return 0;
}

int buyInsurance(struct Player *player,
                 enum InsuranceType type,
                 int property_value,
                 int property_index)
{
    if (player->insurance.active) {
        printf("%s already has insurance.\n", player->name);
        return 0;
    }

    int percentage;

    switch (type) {
        case BASIC_INSURANCE:          percentage = 5;  break;
        case COMPREHENSIVE_INSURANCE:  percentage = 10; break;
        case BUSINESS_INTERRUPTION:    percentage = 15; break;
        default: return 0;
    }

    int premium = (property_value * percentage) / 100;

    if (player->cash < premium) {
        printf("%s cannot afford insurance premium.\n", player->name);
        return 0;
    }

    player->cash -= premium;

    player->insurance.active = 1;
    player->insurance.type = type;
    player->insurance.premium = premium;
    player->insurance.claim = 0;
    player->insurance.property_index = property_index;
    player->insurance.rounds_remaining = 20;

    printf("Insurance purchased.\n\nProperty : %s\n\nPremium : LKR %d.\n\n",
           player->name, premium);

    return 1;
}

void updateInsurance(struct Game *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        struct Player *player = &game->players[i];

        if (!player->insurance.active) {
            continue;
        }

        player->insurance.rounds_remaining--;

        if (player->insurance.rounds_remaining <= 3 &&
            player->insurance.rounds_remaining > 0) {

            printf("Insurance reminder: %s's policy expires in %d rounds.\n",
                   player->name, player->insurance.rounds_remaining);
        }

        if (player->insurance.rounds_remaining <= 0) {

            printf("%s's insurance policy has expired.\n", player->name);

            player->insurance.active = 0;
            player->insurance.type = NO_INSURANCE;
            player->insurance.premium = 0;
            player->insurance.claim = 0;
            player->insurance.property_index = -1;
            player->insurance.rounds_remaining = 0;
        }
    }
}

void processInsuranceClaim(struct Player *player, enum InsuranceType type)
{
    if (!player->insurance.active) {
        return;
    }

    if (player->insurance.type != type) {
        return;
    }

    player->insurance.claim = 1;
    player->insurance_claims++;

    printf("%s insurance claim activated.\n", player->name);
}
