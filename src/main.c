#include <stdio.h>
#include "../include/game.h"
#include "../include/players.h"
#include "../include/board.h"
#include "../include/finance.h"

int main(void)
{
    struct Game game;
    initializeGame(&game);

    printf("MONOPOLY-LK Simulation\n\n");
    printf("Player 1 : Aggressive Investor\n");
    printf("Player 2 : Conservative Banker\n");
    printf("Player 3 : Risk Taker\n");
    printf("Player 4 : Opportunistic Trader\n\n");
    printf("Each player begins with LKR 30,000.\n");

    determineTurnOrder(&game);

    printf("\n===== GAME START =====\n");

    while (!gameHasEnded(&game)) {
        playTurn(&game);
    }

    printf("\n===== GAME OVER =====\n");

    int winner = -1;
    int best_net_worth = -1000000;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        struct Player *p = &game.players[i];

        if (p->status != SOLVENT) continue;

        int net_worth = p->cash; /* simplified: extend with property/building value */

        printf("%s | Cash: LKR %d | Position: %d\n", p->name, p->cash, p->position);

        if (net_worth > best_net_worth) {
            best_net_worth = net_worth;
            winner = i;
        }
    }

    if (winner != -1) {
        printf("\nWinner: %s\n", game.players[winner].name);
    }

    return 0;
}
