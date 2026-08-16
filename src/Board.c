#include <stdio.h>
#include <string.h>

#include "../include/board.h"

static void initializeSpaces(struct Board *board);
static void initializeProperties(struct Board *board);
static void initializeRailways(struct Board *board);
static void initializeUtilities(struct Board *board);
static int canBuildEvenly(const struct Board *board, int property_index);

static void initializeProperties(struct Board *board)
{
    const char *names[MAX_PROPERTIES] = {
        "Pettah", "Maradana", "Bambalapitiya", "Wellawatte", "Mount Lavinia",
        "Nugegoda", "Maharagama", "Kottawa", "Negombo", "Katunayake",
        "Ja-Ela", "Kandy City", "Peradeniya", "Katugastota", "Galle Fort",
        "Unawatuna", "Hikkaduwa", "Jaffna Town", "Nallur", "Trincomalee",
        "Nuwara Eliya", "Galle Face"
    };

    int groups[MAX_PROPERTIES] = {
        1, 1,          /* Pettah, Maradana */
        2, 2, 2,       /* Bambalapitiya, Wellawatte, Mount Lavinia */
        3, 3, 3,       /* Nugegoda, Maharagama, Kottawa */
        4, 4, 4,       /* Negombo, Katunayake, Ja-Ela */
        5, 5, 5,       /* Kandy City, Peradeniya, Katugastota */
        6, 6, 6,       /* Galle Fort, Unawatuna, Hikkaduwa */
        7, 7, 7,       /* Jaffna Town, Nallur, Trincomalee */
        8, 8           /* Nuwara Eliya, Galle Face */
    };

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        strcpy(board->properties[i].name, names[i]);

        board->properties[i].price = 1000 + (i * 200);

        board->properties[i].mortgage_value =
            board->properties[i].price / 2;

        board->properties[i].rent =
            board->properties[i].price / 10;

        board->properties[i].owner = -1;
        board->properties[i].group = groups[i];

        board->properties[i].houses = 0;
        board->properties[i].hotel = 0;

        board->properties[i].mortgaged = 0;
        board->properties[i].loan_locked = 0;
    }
}

static void initializeSpaces(struct Board *board)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        board->spaces[i].property_index = -1;
        board->spaces[i].railway_index = -1;
        board->spaces[i].utility_index = -1;

        board->spaces[i].type = FREE_PARKING;

        snprintf(board->spaces[i].name, MAX_NAME_LENGTH, "Space %d", i);
    }

    strcpy(board->spaces[0].name, "GO");
    board->spaces[0].type = GO;

    strcpy(board->spaces[1].name, "Pettah");
    board->spaces[1].type = PROPERTY;
    board->spaces[1].property_index = 0;

    strcpy(board->spaces[2].name, "Community Development Fund");
    board->spaces[2].type = EVENT;

    strcpy(board->spaces[3].name, "Maradana");
    board->spaces[3].type = PROPERTY;
    board->spaces[3].property_index = 1;

    strcpy(board->spaces[4].name, "Income Tax");
    board->spaces[4].type = TAX;

    strcpy(board->spaces[5].name, "Colombo Fort Railway Station");
    board->spaces[5].type = RAILWAY;
    board->spaces[5].railway_index = 0;

    strcpy(board->spaces[6].name, "Bambalapitiya");
    board->spaces[6].type = PROPERTY;
    board->spaces[6].property_index = 2;

    strcpy(board->spaces[7].name, "National Event Card");
    board->spaces[7].type = EVENT;

    strcpy(board->spaces[8].name, "Wellawatte");
    board->spaces[8].type = PROPERTY;
    board->spaces[8].property_index = 3;

    strcpy(board->spaces[9].name, "Mount Lavinia");
    board->spaces[9].type = PROPERTY;
    board->spaces[9].property_index = 4;

    strcpy(board->spaces[10].name, "Jail / Just Visiting");
    board->spaces[10].type = JAIL;

    strcpy(board->spaces[11].name, "Nugegoda");
    board->spaces[11].type = PROPERTY;
    board->spaces[11].property_index = 5;

    strcpy(board->spaces[12].name, "Ceylon Electricity Board");
    board->spaces[12].type = UTILITY;
    board->spaces[12].utility_index = 0;

    strcpy(board->spaces[13].name, "Maharagama");
    board->spaces[13].type = PROPERTY;
    board->spaces[13].property_index = 6;

    strcpy(board->spaces[14].name, "Kottawa");
    board->spaces[14].type = PROPERTY;
    board->spaces[14].property_index = 7;

    strcpy(board->spaces[15].name, "Kandy Railway Station");
    board->spaces[15].type = RAILWAY;
    board->spaces[15].railway_index = 1;

    strcpy(board->spaces[16].name, "Negombo");
    board->spaces[16].type = PROPERTY;
    board->spaces[16].property_index = 8;

    strcpy(board->spaces[17].name, "Sri Lanka Insurance");
    board->spaces[17].type = INSURANCE;

    strcpy(board->spaces[18].name, "Katunayake");
    board->spaces[18].type = PROPERTY;
    board->spaces[18].property_index = 9;

    strcpy(board->spaces[19].name, "Ja-Ela");
    board->spaces[19].type = PROPERTY;
    board->spaces[19].property_index = 10;

    strcpy(board->spaces[20].name, "Free Parking");
    board->spaces[20].type = FREE_PARKING;

    strcpy(board->spaces[21].name, "Kandy City");
    board->spaces[21].type = PROPERTY;
    board->spaces[21].property_index = 11;

    strcpy(board->spaces[22].name, "National Event Card");
    board->spaces[22].type = EVENT;

    strcpy(board->spaces[23].name, "Peradeniya");
    board->spaces[23].type = PROPERTY;
    board->spaces[23].property_index = 12;

    strcpy(board->spaces[24].name, "Katugastota");
    board->spaces[24].type = PROPERTY;
    board->spaces[24].property_index = 13;

    strcpy(board->spaces[25].name, "Galle Railway Station");
    board->spaces[25].type = RAILWAY;
    board->spaces[25].railway_index = 2;

    strcpy(board->spaces[26].name, "Galle Fort");
    board->spaces[26].type = PROPERTY;
    board->spaces[26].property_index = 14;

    strcpy(board->spaces[27].name, "Unawatuna");
    board->spaces[27].type = PROPERTY;
    board->spaces[27].property_index = 15;

    strcpy(board->spaces[28].name, "NWSDB");
    board->spaces[28].type = UTILITY;
    board->spaces[28].utility_index = 1;

    strcpy(board->spaces[29].name, "Hikkaduwa");
    board->spaces[29].type = PROPERTY;
    board->spaces[29].property_index = 16;

    strcpy(board->spaces[30].name, "Go To Jail");
    board->spaces[30].type = GO_TO_JAIL;

    strcpy(board->spaces[31].name, "Jaffna Town");
    board->spaces[31].type = PROPERTY;
    board->spaces[31].property_index = 17;

    strcpy(board->spaces[32].name, "Nallur");
    board->spaces[32].type = PROPERTY;
    board->spaces[32].property_index = 18;

    strcpy(board->spaces[33].name, "Ceylinco Insurance");
    board->spaces[33].type = INSURANCE;

    strcpy(board->spaces[34].name, "Trincomalee");
    board->spaces[34].type = PROPERTY;
    board->spaces[34].property_index = 19;

    strcpy(board->spaces[35].name, "Jaffna Railway Station");
    board->spaces[35].type = RAILWAY;
    board->spaces[35].railway_index = 3;

    strcpy(board->spaces[36].name, "National Event Card");
    board->spaces[36].type = EVENT;

    strcpy(board->spaces[37].name, "Nuwara Eliya");
    board->spaces[37].type = PROPERTY;
    board->spaces[37].property_index = 20;

    strcpy(board->spaces[38].name, "Bank of Ceylon");
    board->spaces[38].type = BANK;

    strcpy(board->spaces[39].name, "Galle Face");
    board->spaces[39].type = PROPERTY;
    board->spaces[39].property_index = 21;
}

void initializeBoard(struct Board *board)
{
    initializeProperties(board);
    initializeRailways(board);
    initializeUtilities(board);
    initializeSpaces(board);
}

void printBoard(const struct Board *board)
{
    printf("\n===== BOARD =====\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d : %-35s\n", i, board->spaces[i].name);
    }

    printf("=================\n");
}

static void initializeRailways(struct Board *board)
{
    const char *names[MAX_RAILWAYS] = {
        "Colombo Fort Railway Station",
        "Kandy Railway Station",
        "Galle Railway Station",
        "Jaffna Railway Station"
    };

    for (int i = 0; i < MAX_RAILWAYS; i++) {
        strcpy(board->railways[i].name, names[i]);

        board->railways[i].price = 1500;
        board->railways[i].mortgage_value = 750;
        board->railways[i].rent = 250;

        board->railways[i].owner = -1;
        board->railways[i].mortgaged = 0;
        board->railways[i].loan_locked = 0;
    }
}

static void initializeUtilities(struct Board *board)
{
    const char *names[MAX_UTILITIES] = {
        "Ceylon Electricity Board",
        "National Water Supply and Drainage Board"
    };

    for (int i = 0; i < MAX_UTILITIES; i++) {
        strcpy(board->utilities[i].name, names[i]);

        board->utilities[i].price = 1500;
        board->utilities[i].mortgage_value = 750;

        board->utilities[i].owner = -1;
        board->utilities[i].mortgaged = 0;
        board->utilities[i].loan_locked = 0;
    }
}

int buyProperty(struct Board *board,
                struct Player *player,
                int player_index,
                int property_index)
{
    struct Property *property = &board->properties[property_index];

    if (property->owner != -1) {
        return 0;
    }

    if (player->cash < property->price) {
        return 0;
    }

    if (player->property_count >= MAX_PROPERTIES) {
        return 0;
    }

    player->cash -= property->price;
    property->owner = player_index;

    player->properties[player->property_count] = property_index;
    player->property_count++;

    return 1;
}

int calculatePropertyRent(const struct Property *property)
{
    if (property->mortgaged) {
        return 0;
    }

    int multiplier = 1;

    if (property->hotel > 0) {
        multiplier = 10;
    }
    else {
        switch (property->houses) {
            case 0: multiplier = 1; break;
            case 1: multiplier = 2; break;
            case 2: multiplier = 3; break;
            case 3: multiplier = 5; break;
            case 4: multiplier = 7; break;
        }
    }

    return property->rent * multiplier;
}

int ownsCompleteGroup(const struct Board *board,
                      int player_index,
                      int group)
{
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        if (board->properties[i].group == group &&
            board->properties[i].owner != player_index) {
            return 0;
        }
    }

    return 1;
}

static int canBuildEvenly(const struct Board *board, int property_index)
{
    const struct Property *property = &board->properties[property_index];
    int group = property->group;
    int minimum = 999;

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        if (board->properties[i].group == group) {
            if (board->properties[i].houses < minimum) {
                minimum = board->properties[i].houses;
            }
        }
    }

    return property->houses <= minimum;
}

int canBuildHouse(const struct Board *board,
                  int player_index,
                  int property_index)
{
    const struct Property *property = &board->properties[property_index];

    /* Must own the property. */
    if (property->owner != player_index) {
        return 0;
    }

    /* Must own the complete colour group (Rule 8). */
    if (!ownsCompleteGroup(board, player_index, property->group)) {
        return 0;
    }

    /* Already has a hotel. */
    if (property->hotel > 0) {
        return 0;
    }

    /* Maximum four houses. */
    if (property->houses >= 4) {
        return 0;
    }

    /* Must build evenly across the group (Rule 9). */
    if (!canBuildEvenly(board, property_index)) {
        return 0;
    }

    return 1;
}

int buildHouse(struct Board *board,
               struct Player *player,
               int player_index,
               int property_index)
{
    if (!canBuildHouse(board, player_index, property_index)) {
        return 0;
    }

    struct Property *property = &board->properties[property_index];

    int cost;

    switch (property->group) {
        case 1: cost = 500;  break;
        case 2: cost = 750;  break;
        case 3: cost = 1000; break;
        case 4: cost = 1250; break;
        case 5: cost = 1500; break;
        case 6: cost = 2000; break;
        case 7: cost = 2500; break;
        case 8: cost = 3000; break;
        default: return 0;
    }

    if (player->cash < cost) {
        return 0;
    }

    player->cash -= cost;
    property->houses++;

    printf("%s constructed a house on %s.\n", player->name, property->name);
    printf("Construction Cost: LKR %d\n", cost);

    return 1;
}

int buildHotel(struct Board *board,
               struct Player *player,
               int player_index,
               int property_index)
{
    struct Property *property = &board->properties[property_index];

    if (property->owner != player_index) {
        return 0;
    }

    if (property->houses != 4) {
        return 0;
    }

    if (property->hotel > 0) {
        return 0;
    }

    int cost;

    switch (property->group) {
        case 1: cost = 2000;  break;
        case 2: cost = 3000;  break;
        case 3: cost = 4000;  break;
        case 4: cost = 5000;  break;
        case 5: cost = 6000;  break;
        case 6: cost = 8000;  break;
        case 7: cost = 10000; break;
        case 8: cost = 12000; break;
        default: return 0;
    }

    if (player->cash < cost) {
        return 0;
    }

    player->cash -= cost;

    property->houses = 0;
    property->hotel = 1;

    printf("%s upgraded %s to a Hotel.\n", player->name, property->name);
    printf("Hotel Construction Cost: LKR %d\n", cost);

    return 1;
}

int buyRailway(struct Board *board,
               struct Player *player,
               int player_index,
               int railway_index)
{
    struct Railway *railway = &board->railways[railway_index];

    if (railway->owner != -1) {
        return 0;
    }

    if (player->cash < railway->price) {
        return 0;
    }

    if (player->railway_count >= MAX_RAILWAYS) {
        return 0;
    }

    player->cash -= railway->price;
    railway->owner = player_index;

    player->railways[player->railway_count] = railway_index;
    player->railway_count++;

    return 1;
}

int calculateRailwayRent(const struct Railway *railway,
                         int railway_count)
{
    if (railway->mortgaged) {
        return 0;
    }

    switch (railway_count) {
        case 1: return 250;
        case 2: return 500;
        case 3: return 1000;
        case 4: return 2000;
        default: return 0;
    }
}

int buyUtility(struct Board *board,
               struct Player *player,
               int player_index,
               int utility_index)
{
    struct Utility *utility = &board->utilities[utility_index];

    if (utility->owner != -1) {
        return 0;
    }

    if (player->cash < utility->price) {
        return 0;
    }

    if (player->utility_count >= MAX_UTILITIES) {
        return 0;
    }

    player->cash -= utility->price;
    utility->owner = player_index;

    player->utilities[player->utility_count] = utility_index;
    player->utility_count++;

    return 1;
}

int calculateUtilityRent(const struct Utility *utility,
                         int utility_count,
                         int dice_roll)
{
    if (utility->mortgaged) {
        return 0;
    }

    if (utility_count == 1) {
        return dice_roll * 4;
    }

    if (utility_count >= 2) {
        return dice_roll * 10;
    }

    return 0;
}
