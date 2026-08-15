#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/game.h"
#include "../include/players.h"
#include "../include/board.h"

int rollDie(void)
{
    return (rand() % DICE_SIDES) + 1;
}

int rollTwoDice(void)
{
    return rollDie() + rollDie();
}

void initializeGame(struct Game *game)
{
    srand((unsigned int)time(NULL));

    initializeBoard(&game->board);
    initializePlayers(game->players);

    game->current_player = 0;
    game->round = 1;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        game->turn_order[i] = i;
    }
}

void determineTurnOrder(struct Game *game)
{
    int rolls[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        game->turn_order[i] = i;
    }

    int order_complete = 0;

    while (!order_complete) {

        for (int i = 0; i < MAX_PLAYERS; i++) {
            rolls[i] = rollTwoDice();

            printf("%s rolls %d.\n",
                   game->players[i].name,
                   rolls[i]);
        }

        order_complete = 1;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            for (int j = i + 1; j < MAX_PLAYERS; j++) {

                if (rolls[i] == rolls[j]) {
                    order_complete = 0;
                }
            }
        }

        if (!order_complete) {
            printf("Tie detected. Rolling again...\n\n");
        }
    }

    for (int i = 0; i < MAX_PLAYERS - 1; i++) {
        for (int j = i + 1; j < MAX_PLAYERS; j++) {

            if (rolls[j] > rolls[i]) {

                int temp = rolls[i];
                rolls[i] = rolls[j];
                rolls[j] = temp;

                temp = game->turn_order[i];
                game->turn_order[i] = game->turn_order[j];
                game->turn_order[j] = temp;
            }
        }
    }

    printf("\nTurn order:\n");

    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("%d. %s\n",
               i + 1,
               game->players[game->turn_order[i]].name);
    }

    game->current_player = 0;
}

void movePlayer(struct Game *game, int player_index, int steps)
{
    struct Player *player = &game->players[player_index];

    int old_position = player->position;

    int new_position =
        (old_position + steps) % BOARD_SIZE;

    if (old_position + steps >= BOARD_SIZE) {

        player->cash += GO_REWARD;

        printf("%s passed GO and collected LKR %d.\n",
               player->name,
               GO_REWARD);
    }

    player->position = new_position;

    printf("%s moves from Square %d to Square %d.\n",
           player->name,
           old_position,
           new_position);
}

void playTurn(struct Game *game)
{
    int player_index = game->turn_order[game->current_player];

    struct Player *player = &game->players[player_index];

    printf("\n=================================\n");
    printf("Round %d - %s's turn\n", game->round, player->name);
    printf("=================================\n");

    printf("Current position: %d\n", player->position);
    printf("Cash: LKR %d\n", player->cash);

    int dice = rollTwoDice();

    printf("Rolled: %d\n", dice);

    movePlayer(game, player_index, dice);

    game->current_player++;

    if (game->current_player >= MAX_PLAYERS) {
        game->current_player = 0;
        game->round++;
    }
}
