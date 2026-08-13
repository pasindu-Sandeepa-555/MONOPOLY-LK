#include <stdio.h>
#include "../include/players.h"
#include "../include/board.h"


int main(void)
{
	struct Player players[MAX_PLAYERS];
	struct Board board;

	initializePlayers(players);
	initializeBoard(&board);

	printf("MONOPOLY-LK\n");
	printf("====================\n");

	for (int i = 0; i < MAX_PLAYERS; i++) {
		printPlayer(&players[i]);

	}
	
	printBoard(&board);;

	return 0;

}



