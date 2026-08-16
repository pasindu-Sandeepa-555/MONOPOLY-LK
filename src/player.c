#include <stdio.h>
#include <string.h>
#include "../include/players.h"

void initializePlayers(struct Player players[MAX_PLAYERS])
{
    const char *names[MAX_PLAYERS] = {
        "Aggressive Investor",
        "Conservative Banker",
        "Risk-Taking Investor",
        "Opportunistic Trader"
    };
    enum Strategy strategies[MAX_PLAYERS] = {
        AGGRESSIVE,
        CONSERVATIVE,
        RISK_TAKER,
        TRADER
    };

    for (int i = 0; i < MAX_PLAYERS; i++) {
        strcpy(players[i].name, names[i]);
        players[i].cash = 30000;
        players[i].position = 0;
        players[i].strategy = strategies[i];
        players[i].status = SOLVENT;
        players[i].property_count = 0;
        players[i].railway_count = 0;
        players[i].utility_count = 0;

        for (int j = 0; j < MAX_PROPERTIES; j++) {
            players[i].properties[j] = -1;
        }
        for (int j = 0; j < MAX_RAILWAYS; j++) {
            players[i].railways[j] = -1;
        }
        for (int j = 0; j < MAX_UTILITIES; j++) {
            players[i].utilities[j] = -1;
        }

        players[i].loan.active = 0;
        players[i].loan.principal = 0;
        players[i].loan.interest = 0;
        players[i].loan.rate = 0;
        players[i].loan.rounds_remaining = 0;

        players[i].insurance.active = 0;
        players[i].insurance.property_index = -1;

        players[i].taxes_due = 0;
        players[i].accrued_interest = 0;
        players[i].insurance_claims = 0;

        players[i].has_active_event = 0;
        players[i].event_rounds_remaining = 0;
    }
}

void printPlayer(const struct Player *player)
{
    printf("Player: %s\n", player->name);
    printf("Cash: LKR %d\n", player->cash);
    printf("Position: %d\n", player->position);
    printf("Properties: %d\n", player->property_count);
    printf("\n");
}



int playerWantsToBuy(const struct Player *player, int price)
{
    switch (player->strategy) {

        case AGGRESSIVE:
                        return (player->cash - price) >= (price / 10);

        case CONSERVATIVE:
                       return (player->cash - price) >= (player->cash / 2);

        case RISK_TAKER:
            
            return player->cash >= price;

        case TRADER:
                        return player->cash >= (price + price / 2);

        default:
            return 0;
    }
}

int playerWantsLoan(const struct Player *player)
{
    if (player->loan.active) {
        return 0;
    }

    switch (player->strategy) {
        case AGGRESSIVE: return 1;
        case CONSERVATIVE: return player->cash < 2000;  
	case RISK_TAKER: return 1;                        
	case TRADER: return player->cash < 5000;
        default: return 0;
    }
}

int playerWantsToRepayLoan(const struct Player *player)
{
    if (!player->loan.active) {
        return 0;
    }

    int outstanding = player->loan.principal + player->loan.interest;

    switch (player->strategy) {
        case AGGRESSIVE:
            return player->cash > (2 * outstanding);

        case CONSERVATIVE:
            return player->cash >= outstanding;

        case RISK_TAKER:
            return 0; 
        case TRADER:
            return player->cash > (outstanding + 3000);

        default:
            return 0;
    }
}

int playerWantsInsurance(const struct Player *player, int property_value)
{
    switch (player->strategy) {
        case AGGRESSIVE:
            return 1;

        case CONSERVATIVE:
            return 1;

        case RISK_TAKER:
            /* only insures after experiencing a financial loss */
            return player->insurance_claims > 0;

        case TRADER:
            /* only for high-value developments */
            return property_value > 5000;

        default:
            return 0;
    }
}

int playerWantsToBuildHouse(const struct Player *player)
{
    switch (player->strategy) {
        case AGGRESSIVE:
            return 1;

        case CONSERVATIVE:
            
            return !player->loan.active;

        case RISK_TAKER:
            return 1;

        case TRADER:
            return player->cash > 5000;

        default:
            return 0;
    }
}

int playerAuctionBid(const struct Player *player, int current_bid,
                     int market_value, int increment)
{
    int next_bid = current_bid + increment;

    if (player->cash < next_bid) {
        return 0;
    }

    switch (player->strategy) {
        case AGGRESSIVE:
                        return next_bid <= (market_value * 120) / 100;

        case CONSERVATIVE:
            
            return next_bid < market_value;

        case RISK_TAKER:
                        return 1;

        case TRADER:
                        return next_bid <= (market_value * 80) / 100;

        default:
            return 0;
    }
}
