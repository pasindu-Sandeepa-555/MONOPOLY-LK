#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/game.h"
#include "../include/players.h"
#include "../include/board.h"

#include "../include/finance.h"

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

    game->last_dice_roll = dice;

    printf("Rolled: %d\n", dice);

    movePlayer(game, player_index, dice);

    resolveLanding(game, player_index);

    game->current_player++;

    if (game->current_player >= MAX_PLAYERS) {
        
	game->current_player = 0;
	
	printf("\n===== ROUND %d COMPLETE =====\n" , game->round);

	updateLoans(game);

        game->round++;
    }
}

void resolveLanding(struct Game *game, int player_index)
{
    struct Player *player = &game->players[player_index];

    int position = player->position;

    struct Space *space = &game->board.spaces[position];

    printf("\n%s landed on Square %d: %s\n",
           player->name,
           position,
           space->name);

    switch (space->type) {

        case GO:
            printf("This is GO.\n");
            break;

        case PROPERTY:

	    int property_index = space->property_index;

	    struct Property *property =
   	    &game->board.properties[property_index];

	    if (property->owner == -1) {

   	    printf("Property is unowned.\n");
   	    printf("Price: LKR %d\n", property->price);

   	    if (player->cash >= property->price) {

 	    printf("%s buys %s for LKR %d.\n",
               player->name,
               property->name,
               property->price);

            buyProperty(&game->board,
                    player,
                    player_index,
                    property_index);
            }
   	    else {
       		 printf("%s cannot afford this property.\n",
                 	 player->name);
            }
      }
       else if (property->owner == player_index) {

   		 printf("%s already owns this property.\n",
       		         player->name);
	}
	else {

	    int rent =
	        calculatePropertyRent(property);

	    int owner_index = property->owner;

	    printf("%s pays LKR %d rent to %s.\n",
        	   player->name,
        	   rent,
        	   game->players[owner_index].name);

	    player->cash -= rent;

	    game->players[owner_index].cash += rent;
	}
	break;

	case RAILWAY:
{
    int railway_index = space->railway_index;

    struct Railway *railway =
        &game->board.railways[railway_index];

    if (railway->owner == -1) {

        printf("Railway is unowned.\n");
        printf("Price: LKR %d\n", railway->price);

        if (player->cash >= railway->price) {

            printf("%s buys %s for LKR %d.\n",
                   player->name,
                   railway->name,
                   railway->price);

            buyRailway(&game->board,
                       player,
                       player_index,
                       railway_index);
        }
        else {
            printf("%s cannot afford this railway.\n",
                   player->name);
        }
    }
    else if (railway->owner == player_index) {

        printf("%s already owns this railway.\n",
               player->name);
    }
    else {

        int owner_index = railway->owner;

        int rent =
            calculateRailwayRent(
                railway,
                game->players[owner_index].railway_count
            );

        printf("%s pays LKR %d railway rent to %s.\n",
               player->name,
               rent,
               game->players[owner_index].name);

        player->cash -= rent;
        game->players[owner_index].cash += rent;
    }

    break;
}

      
        case UTILITY:
	{
	    int utility_index = space->utility_index;
	
	    struct Utility *utility =
	        &game->board.utilities[utility_index];

	    if (utility->owner == -1) {

	        printf("Utility is unowned.\n");
	        printf("Price: LKR %d\n", utility->price);

	        if (player->cash >= utility->price) {

	            printf("%s buys %s for LKR %d.\n",
	                   player->name,
	                   utility->name,
	                   utility->price);

	            buyUtility(&game->board,
	                       player,
	                       player_index,
	                       utility_index);
	        }
	        else {
	            printf("%s cannot afford this utility.\n",
	                   player->name);
	        }
	    }
	    else if (utility->owner == player_index) {

		    printf("%s already owns this utility.\n",
		               player->name);
	    }
	    else {

	        int owner_index = utility->owner;
	
	        int utility_count =
	            game->players[owner_index].utility_count;

	        int rent =
	            calculateUtilityRent(
	                utility,
	                utility_count,
	                game->last_dice_roll
	            );

	        printf("%s pays LKR %d utility rent to %s.\n",
	               player->name,
	               rent,
	               game->players[owner_index].name);

	        player->cash -= rent;
	        game->players[owner_index].cash += rent;
	    }
	
	    break;
	}
	

	
	case TAX:
	{
	    int tax = 2000;

	    payTax(player, tax);

	    break;
	}
	

        case JAIL:
            printf("This is Jail / Just Visiting.\n");
            break;

        case GO_TO_JAIL:
            printf("Go directly to Jail.\n");
            player->position = 10;
            break;

        case FREE_PARKING:
            printf("Free Parking.\n");
            break;

        case BANK:
            printf("Bank space.\n");
            break;

        case INSURANCE:
            printf("Insurance space.\n");
            break;

        default:
            printf("Unknown space type.\n");
            break;
    }
}

