#include <stdio.h>
#include <string.h>

#include "../include/board.h"

static void initializeProperties(struct Board *board)
{
    const char *names[MAX_PROPERTIES] = {
        "Pettah",
        "Maradana",
        "Bambalapitiya",
        "Wellawatte",
        "Mount Lavinia",
        "Nugegoda",
        "Maharagama",
        "Kottawa",
        "Negombo",
        "Katunayake",
        "Ja-Ela",
        "Kandy City",
        "Peradeniya",
        "Katugastota",
        "Galle Fort",
        "Unawatuna",
        "Hikkaduwa",
        "Jaffna Town",
        "Nallur",
        "Trincomalee",
        "Nuwara Eliya",
        "Galle Face"
    };

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        strcpy(board->properties[i].name, names[i]);

        board->properties[i].price = 1000 + (i * 200);

        board->properties[i].mortgage_value =
            board->properties[i].price / 2;

        board->properties[i].rent =
            board->properties[i].price / 10;

        board->properties[i].owner = -1;

        board->properties[i].group = i / 2;

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

        snprintf(
            board->spaces[i].name,
            MAX_NAME_LENGTH,
            "Space %d",
            i
        );
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
    initializeSpaces(board);
}

void printBoard(const struct Board *board)
{
    printf("\n===== BOARD =====\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d : %-35s\n",
               i,
               board->spaces[i].name);
    }

    printf("=================\n");
}

