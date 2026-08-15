#ifndef BOARD_H
#define BOARD_H

#include "types.h"

void initializeBoard(struct Board *board);

void printBoard(const struct Board *board);

int calculateRailwayRent(const struct Board *board, int player_index);

int calculateUtilityRent(
		const struct Board *board,
		int player_index,
		int dice_value
		);


#endif

