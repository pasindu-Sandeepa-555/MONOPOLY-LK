#ifndef BOARD_H
#define BOARD_H

#include "types.h"

void initializeBoard(struct Board *board);

void printBoard(const struct Board *board);

int buyProperty(struct Board *board, struct Player *player,int player_index, int property_index);

int calculatePropertyRent(const struct Property *property);

int buyRailway(struct Board *board, struct Player *player, int player_index, int railway_index);

int calculateRailwayRent(const struct Railway *railway, int railway_count);

int buyUtility(struct Board *board, struct Player *player, int player_index, int utility_index);

int calculateUtilityRent(const struct Utility *utility, int utility_count, int dice_roll);

int canBuildHouse(const struct Board *board, int player_index, int property_index);

int buildHouse(struct Board *board, struct Player *player, int player_index, int property_index);

int buildHotel(struct Board *board, struct Player *player, int player_index, int property_index);

int ownsCompleteGroup(const struct Board *board, int player_index, int group);

int canBuildHouse(const struct Board *board, int player_index, int property_index);


#endif

