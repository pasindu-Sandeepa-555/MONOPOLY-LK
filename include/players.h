#ifndef PLAYERS_H
#define PLAYERS_H

#include "types.h"

void initializePlayers(struct Player players[MAX_PLAYERS]);

void printPlayer(const struct Player *player);

int playerWantsToBuy(const struct Player *player, int price);
int playerWantsLoan(const struct Player *player);
int playerWantsToRepayLoan(const struct Player *player);
int playerWantsInsurance(const struct Player *player, int property_value);
int playerWantsToBuildHouse(const struct Player *player);
int playerAuctionBid(const struct Player *player, int current_bid,
                     int market_value, int increment);


#endif
