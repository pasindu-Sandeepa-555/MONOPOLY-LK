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
		players[i].railways[j] = -1;
	}

        players[i].loan.active = 0;
        players[i].insurance.active = 0;

        players[i].taxes_due = 0;
        players[i].accrued_interest = 0;
        players[i].insurance_claims = 0;
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
