#ifndef GAME_H
#define GAME_H

#include "types.h"

void initializeGame(struct Game *game);

void determineTurnOrder(struct Game *game);

int rollDie(void);

int rollTwoDice(void);

void movePlayer(struct Game *game, int player_index, int steps);

void playTurn(struct Game *game);



#endif
