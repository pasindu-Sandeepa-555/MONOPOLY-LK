#ifndef EVENTS_H
#define EVENTS_H

#include "types.h"

void initializeEconomy(struct Economy *economy);

void drawNationalEventCard(struct Game *game, int player_index);

void runEconomicSystems(struct Game *game);

void printCurrentMarketConditions(const struct Game *game);

#endif
