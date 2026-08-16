#include <stdio.h>

#include "../include/finance.h"

int takeLoan(struct Player *player, int amount)
{
    if (player->loan.active) {
        printf("%s already has an active loan.\n",
               player->name);
        return 0;
    }

    if (amount <= 0) {
        return 0;
    }

    player->loan.active = 1;
    player->loan.principal = amount;
    player->loan.interest = 0;
    player->loan.rounds_remaining = LOAN_DURATION;

    player->cash += amount;

    printf("%s takes a loan of LKR %d.\n",
           player->name,
           amount);

    printf("Loan duration: %d rounds\n",
           LOAN_DURATION);

    return 1;
}


int repayLoan(struct Player *player, int amount)
{
    if (!player->loan.active) {
        printf("%s has no active loan.\n",
               player->name);
        return 0;
    }

    if (amount <= 0) {
        return 0;
    }

    int total =
        player->loan.principal +
        player->loan.interest;

    if (amount > total) {
        amount = total;
    }

    if (player->cash < amount) {
        printf("%s does not have enough cash to repay the loan.\n",
               player->name);
        return 0;
    }

    player->cash -= amount;

    /* Interest is paid before principal. */
    if (amount >= player->loan.interest) {

        amount -= player->loan.interest;
        player->loan.interest = 0;

        player->loan.principal -= amount;
    }
    else {
        player->loan.interest -= amount;
    }

    if (player->loan.principal == 0 &&
        player->loan.interest == 0) {

        player->loan.active = 0;
        player->loan.rounds_remaining = 0;

        printf("%s fully repaid the loan.\n",
               player->name);
    }
    else {
        printf("%s made a loan repayment.\n",
               player->name);

        printf("Outstanding loan: LKR %d\n",
               player->loan.principal +
               player->loan.interest);
    }

    return 1;
}


void updateLoans(struct Game *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {

        struct Player *player =
            &game->players[i];

        if (!player->loan.active) {
            continue;
        }

        int outstanding =
            player->loan.principal +
            player->loan.interest;

        /*
         * Current loan interest rate = 8%.
         * Interest is compounded after every
         * complete round.
         */
        int interest =
            (outstanding * LOAN_INTEREST_RATE) / 100;

        player->loan.interest += interest;

        player->loan.rounds_remaining--;

        printf("%s loan interest: LKR %d\n",
               player->name,
               interest);

        printf("Outstanding loan: LKR %d\n",
               player->loan.principal +
               player->loan.interest);

        if (player->loan.rounds_remaining <= 0) {

            printf("%s's loan has reached maturity.\n",
                   player->name);

            /*
             * Full foreclosure system can be
             * implemented later.
             */
            printf("Loan default/foreclosure must be processed.\n");
        }
    }
}


int payTax(struct Player *player, int amount)
{
    if (amount <= 0) {
        return 0;
    }

    if (player->cash >= amount) {

        printf("%s pays LKR %d in tax.\n",
               player->name,
               amount);

        player->cash -= amount;

        return 1;
    }

    /*
     * Player cannot immediately pay.
     * Record the unpaid tax.
     */
    player->taxes_due += amount;

    printf("%s cannot pay LKR %d in tax.\n",
           player->name,
           amount);

    return 0;
}


int buyInsurance(struct Player *player,
                 enum InsuranceType type,
                 int property_value)
{
    if (player->insurance.active) {

        printf("%s already has insurance.\n",
               player->name);

        return 0;
    }

    int percentage;

    switch (type) {

        case BASIC_INSURANCE:
            percentage = 5;
            break;

        case COMPREHENSIVE_INSURANCE:
            percentage = 10;
            break;

        case BUSINESS_INTERRUPTION:
            percentage = 15;
            break;

        default:
            return 0;
    }

    int premium =
        (property_value * percentage) / 100;

    if (player->cash < premium) {

        printf("%s cannot afford insurance premium.\n",
               player->name);

        return 0;
    }

    player->cash -= premium;

    player->insurance.active = 1;
    player->insurance.type = type;
    player->insurance.premium = premium;
    player->insurance.claim = 0;
    player->insurance.rounds_remaining = 20;

    printf("%s purchased insurance.\n",
           player->name);

    printf("Premium: LKR %d\n",
           premium);

    printf("Coverage duration: 20 rounds\n");

    return 1;
}


void updateInsurance(struct Game *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {

        struct Player *player =
            &game->players[i];

        if (!player->insurance.active) {
            continue;
        }

        player->insurance.rounds_remaining--;

        if (player->insurance.rounds_remaining <= 3 &&
            player->insurance.rounds_remaining > 0) {

            printf(
                "Insurance reminder: %s's policy expires in %d rounds.\n",
                player->name,
                player->insurance.rounds_remaining
            );
        }

        if (player->insurance.rounds_remaining <= 0) {

            printf("%s's insurance policy has expired.\n",
                   player->name);

            player->insurance.active = 0;
            player->insurance.type = NO_INSURANCE;
            player->insurance.premium = 0;
            player->insurance.claim = 0;
            player->insurance.rounds_remaining = 0;
        }
    }
}


void processInsuranceClaim(struct Player *player,
                           enum InsuranceType type)
{
    if (!player->insurance.active) {
        return;
    }

    if (player->insurance.type != type) {
        return;
    }

    player->insurance.claim = 1;
    player->insurance_claims++;

    printf("%s insurance claim activated.\n",
           player->name);
}
