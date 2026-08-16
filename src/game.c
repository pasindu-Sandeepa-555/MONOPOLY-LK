#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/game.h"
#include "../include/players.h"
#include "../include/board.h"
#include "../include/finance.h"
#include "../include/events.h"

static void runAuction(struct Game *game, int property_index);

int rollDie(void)
{
    return (rand() % DICE_SIDES) + 1;
}

int rollTwoDice(void)
{
    return rollDie() + rollDie();
}

void initializeGame(struct Game *game)
{
    srand((unsigned int)time(NULL));

    initializeBoard(&game->board);
    initializePlayers(game->players);
    initializeEconomy(&game->economy);

    game->current_player = 0;
    game->round = 1;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        game->turn_order[i] = i;
    }
}

void determineTurnOrder(struct Game *game)
{
    int rolls[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        game->turn_order[i] = i;
    }

    int order_complete = 0;

    while (!order_complete) {

        for (int i = 0; i < MAX_PLAYERS; i++) {
            rolls[i] = rollTwoDice();
            printf("%s rolls %d.\n", game->players[i].name, rolls[i]);
        }

        order_complete = 1;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            for (int j = i + 1; j < MAX_PLAYERS; j++) {
                if (rolls[i] == rolls[j]) {
                    order_complete = 0;
                }
            }
        }

        if (!order_complete) {
            printf("Tie detected. Rolling again...\n\n");
        }
    }

    for (int i = 0; i < MAX_PLAYERS - 1; i++) {
        for (int j = i + 1; j < MAX_PLAYERS; j++) {
            if (rolls[j] > rolls[i]) {
                int temp = rolls[i];
                rolls[i] = rolls[j];
                rolls[j] = temp;

                temp = game->turn_order[i];
                game->turn_order[i] = game->turn_order[j];
                game->turn_order[j] = temp;
            }
        }
    }

    printf("\nTurn order:\n");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("%d. %s\n", i + 1, game->players[game->turn_order[i]].name);
    }

    game->current_player = 0;
}

void movePlayer(struct Game *game, int player_index, int steps)
{
    struct Player *player = &game->players[player_index];

    int old_position = player->position;
    int new_position = (old_position + steps) % BOARD_SIZE;

    if (old_position + steps >= BOARD_SIZE) {
        player->cash += GO_REWARD;
        printf("%s passed GO and collected LKR %d.\n", player->name, GO_REWARD);
    }

    player->position = new_position;

    printf("%s moves from Square %d to Square %d.\n",
           player->name, old_position, new_position);
}

static int countSolventPlayers(const struct Game *game)
{
    int count = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].status == SOLVENT) count++;
    }
    return count;
}

void playTurn(struct Game *game)
{
    int player_index = game->turn_order[game->current_player];
    struct Player *player = &game->players[player_index];

    if (player->status == BANKRUPT) {
        game->current_player++;
        if (game->current_player >= MAX_PLAYERS) game->current_player = 0;
        return;
    }

    printf("\n=================================\n");
    printf("Round %d - %s's turn\n", game->round, player->name);
    printf("=================================\n");

    printf("Current position: %d\n", player->position);
    printf("Cash: LKR %d\n", player->cash);

    int dice = rollTwoDice();
    game->last_dice_roll = dice;

    printf("Rolled: %d\n", dice);

    movePlayer(game, player_index, dice);
    resolveLanding(game, player_index);
    developProperties(game, player_index);

    if (player->cash < 0 && player->status == SOLVENT) {
        player->status = BANKRUPT;
        printf("%s has been declared bankrupt.\n\nRemaining assets transferred to the Bank.\n\n",
               player->name);
    }

    game->current_player++;

    if (game->current_player >= MAX_PLAYERS) {

        game->current_player = 0;

        printf("\n===== ROUND %d COMPLETE =====\n", game->round);

        updateLoans(game);
        updateInsurance(game);
        runEconomicSystems(game);

        printf("=============================================\n");
        printf("Round %d Summary\n", game->round);
        printf("=============================================\n\n");

        for (int i = 0; i < MAX_PLAYERS; i++) {
            struct Player *p = &game->players[i];
            printf("%s\n\nCash : LKR %d\n\nProperties : %d\n\nOutstanding Loan : %s\n",
                   p->name, p->cash, p->property_count,
                   p->loan.active ? "LKR (see above)" : "None");
            printf("---------------------------------------------\n\n");
        }

        game->round++;
    }
}

static void runAuction(struct Game *game, int property_index)
{
    struct Property *property = &game->board.properties[property_index];

    int active[MAX_PLAYERS];
    int active_count = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        active[i] = (game->players[i].status == SOLVENT);
        if (active[i]) active_count++;
    }

    int current_bid = property->price / 2;
    int winner = -1;

    printf("Auction Started.\n\nProperty :\n%s\n\nOpening Bid :\nLKR %d.\n\n",
           property->name, current_bid);

    int changed = 1;

    while (active_count > 1 && changed) {
        changed = 0;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (!active[i]) continue;

            struct Player *bidder = &game->players[i];

            if (playerAuctionBid(bidder, current_bid, property->price, 250)) {
                current_bid += 250;
                winner = i;
                changed = 1;
                printf("%s bids LKR %d.\n", bidder->name, current_bid);
            }
            else {
                active[i] = 0;
                active_count--;
                printf("%s withdraws.\n", bidder->name);
            }
        }
    }

    if (winner != -1 && game->players[winner].cash >= current_bid) {
        struct Player *w = &game->players[winner];

        w->cash -= current_bid;
        property->owner = winner;

        w->properties[w->property_count] = property_index;
        w->property_count++;

        printf("%s wins the auction.\n\n", w->name);
    }
    else {
        printf("No bids received. Property remains with the Bank.\n\n");
    }
}

void resolveLanding(struct Game *game, int player_index)
{
    struct Player *player = &game->players[player_index];
    int position = player->position;
    struct Space *space = &game->board.spaces[position];

    printf("\n%s landed on Square %d: %s\n", player->name, position, space->name);

    switch (space->type) {

        case GO:
            printf("This is GO.\n");
            break;

        case PROPERTY: {
            int property_index = space->property_index;
            struct Property *property = &game->board.properties[property_index];

            if (property->owner == -1) {
                printf("Property is unowned.\n");
                printf("Price: LKR %d\n", property->price);

                if (playerWantsToBuy(player, property->price)) {
                    printf("%s purchased %s for LKR %d.\n",
                           player->name, property->name, property->price);

                    buyProperty(&game->board, player, player_index, property_index);
                }
                else {
                    printf("%s declined to purchase. Property enters auction.\n", player->name);
                    runAuction(game, property_index);
                }
            }
            else if (property->owner == player_index) {
                printf("%s already owns this property.\n", player->name);
            }
            else {
                int rent = calculatePropertyRent(property);
                int owner_index = property->owner;

                printf("%s pays LKR %d rent to %s.\n",
                       player->name, rent, game->players[owner_index].name);

                player->cash -= rent;
                game->players[owner_index].cash += rent;
            }
            break;
        }

        case RAILWAY: {
            int railway_index = space->railway_index;
            struct Railway *railway = &game->board.railways[railway_index];

            if (railway->owner == -1) {
                printf("Railway is unowned.\n");
                printf("Price: LKR %d\n", railway->price);

                if (playerWantsToBuy(player, railway->price)) {
                    printf("%s buys %s for LKR %d.\n",
                           player->name, railway->name, railway->price);

                    buyRailway(&game->board, player, player_index, railway_index);
                }
                else {
                    printf("%s cannot afford or declined this railway.\n", player->name);
                }
            }
            else if (railway->owner == player_index) {
                printf("%s already owns this railway.\n", player->name);
            }
            else {
                int owner_index = railway->owner;
                int rent = calculateRailwayRent(railway,
                    game->players[owner_index].railway_count);

                printf("%s pays LKR %d railway rent to %s.\n",
                       player->name, rent, game->players[owner_index].name);

                player->cash -= rent;
                game->players[owner_index].cash += rent;
            }
            break;
        }

        case UTILITY: {
            int utility_index = space->utility_index;
            struct Utility *utility = &game->board.utilities[utility_index];

            if (utility->owner == -1) {
                printf("Utility is unowned.\n");
                printf("Price: LKR %d\n", utility->price);

                if (playerWantsToBuy(player, utility->price)) {
                    printf("%s buys %s for LKR %d.\n",
                           player->name, utility->name, utility->price);

                    buyUtility(&game->board, player, player_index, utility_index);
                }
                else {
                    printf("%s cannot afford or declined this utility.\n", player->name);
                }
            }
            else if (utility->owner == player_index) {
                printf("%s already owns this utility.\n", player->name);
            }
            else {
                int owner_index = utility->owner;
                int utility_count = game->players[owner_index].utility_count;

                int rent = calculateUtilityRent(utility, utility_count, game->last_dice_roll);

                printf("%s pays LKR %d utility rent to %s.\n",
                       player->name, rent, game->players[owner_index].name);

                player->cash -= rent;
                game->players[owner_index].cash += rent;
            }
            break;
        }

        case TAX:
            payTax(player, INCOME_TAX);
            break;

        case EVENT:
            drawNationalEventCard(game, player_index);
            break;

        case JAIL:
            printf("This is Jail / Just Visiting.\n");
            break;

        case GO_TO_JAIL:
            printf("Go directly to Jail.\n");
            player->position = 10;
            break;

        case FREE_PARKING:
            printf("Free Parking.\n");
            break;

        case BANK: {
            printf("Bank of Ceylon.\n");

            if (player->loan.active) {
                printf("%s has an active loan.\n", player->name);
                printf("Outstanding loan: LKR %d\n",
                       player->loan.principal + player->loan.interest);

                if (playerWantsToRepayLoan(player)) {
                    repayLoan(player, player->loan.principal + player->loan.interest);
                }
            }
            else if (playerWantsLoan(player)) {
                int max_loan = calculateMaxLoan(&game->board, player);

                if (max_loan > 0) {
                    takeLoan(player, max_loan, game->economy.loan_interest_rate);
                }
                else {
                    printf("%s has no eligible collateral for a loan.\n", player->name);
                }
            }
            else {
                printf("%s has no active loan.\n", player->name);
            }
            break;
        }

        case INSURANCE: {
            printf("This is an insurance space.\n");

            if (player->insurance.active) {
                printf("%s already has active insurance.\n", player->name);
            }
            else {
                int target = -1;
                int best_value = -1;

                for (int i = 0; i < MAX_PROPERTIES; i++) {
                    struct Property *p = &game->board.properties[i];

                    if (p->owner == player_index && (p->houses > 0 || p->hotel > 0)) {
                        if (p->price > best_value) {
                            best_value = p->price;
                            target = i;
                        }
                    }
                }

                if (target != -1 && playerWantsInsurance(player, best_value)) {
                    enum InsuranceType type =
                        (player->strategy == CONSERVATIVE ||
                         game->board.properties[target].hotel > 0)
                            ? COMPREHENSIVE_INSURANCE
                            : BASIC_INSURANCE;

                    buyInsurance(player, type, best_value, target);
                }
                else {
                    printf("%s can purchase insurance.\n", player->name);
                }
            }
            break;
        }

        default:
            printf("Unknown space type.\n");
            break;
    }
}

void developProperties(struct Game *game, int player_index)
{
    struct Player *player = &game->players[player_index];

    if (!playerWantsToBuildHouse(player)) {
        return;
    }

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        struct Property *property = &game->board.properties[i];

        if (property->owner != player_index) {
            continue;
        }

        if (property->houses == 4 && property->hotel == 0) {
            buildHotel(&game->board, player, player_index, i);
            continue;
        }

        if (canBuildHouse(&game->board, player_index, i)) {
            buildHouse(&game->board, player, player_index, i);
        }
    }
}

int gameHasEnded(const struct Game *game)
{
    return (countSolventPlayers(game) <= 1) || (game->round > 500);
}
