#include <stdio.h>
#include "../include/players.h"

int main(void)
{
	struct Player players[MAX_PLAYERS];

	initializePlayers(players);

	printf("MONOPOLY-LK\n");
	printf("====================\n");

	for (int i = 0; i < MAX_PLAYERS; i++) {
		printPlayer(&players[i]);

	}

	return 0;

}



