#include <stdio.h>

#include "../include/game.h"

int main(void)
{
    struct Game game;

    initializeGame(&game);

    printf("MONOPOLY-LK\n");
    printf("====================\n");

    determineTurnOrder(&game);

    printf("\n===== GAME START =====\n");

    for (int i = 0; i < 8; i++) {
        playTurn(&game);
    }

    printf("\n===== GAME STATE =====\n");

    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("%s | Cash: LKR %d | Position: %d\n",
               game.players[i].name,
               game.players[i].cash,
               game.players[i].position);
    }

    return 0;
}
