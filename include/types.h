#ifndef TYPES_H
#define TYPES_H

#define MAX_PLAYERS 4
#define BOARD_SIZE 40
#define MAX_NAME_LENGTH 50
#define MAX_PROPERTIES 22

#define MAX_RAILWAYS 4
#define MAX_UTILITIES 2

#define DICE_SIDES 6
#define GO_REWARD 2000

enum Strategy {
    AGGRESSIVE,
    CONSERVATIVE,
    RISK_TAKER,
    TRADER
};

enum PlayerStatus {
    SOLVENT,
    BANKRUPT
};

enum SpaceType {
    GO,
    PROPERTY,
    RAILWAY,
    UTILITY,
    TAX,
    EVENT,
    JAIL,
    GO_TO_JAIL,
    FREE_PARKING,
    BANK,
    INSURANCE
};

struct Loan {
    int active;
    int principal;
    int interest;
    int rounds_remaining;
};

struct Insurance {
    int active;
    int type;
    int premium;
    int claim;
    int rounds_remaining;
};

struct Property {
    char name[MAX_NAME_LENGTH];

    int price;
    int mortgage_value;
    int rent;

    int owner;
    int group;

    int houses;
    int hotel;

    int mortgaged;
    int loan_locked;
};

struct Railway {
    char name[MAX_NAME_LENGTH];

    int price;
    int mortgage_value;
    int rent;

    int owner;
    int mortgaged;
    int loan_locked;
};

struct Utility {
    char name[MAX_NAME_LENGTH];

    int price;
    int mortgage_value;

    int owner;
    int mortgaged;
    int loan_locked;
};

struct Space {
    char name[MAX_NAME_LENGTH];

    enum SpaceType type;

    int property_index;
    int railway_index;
    int utility_index;
};

struct Player {
    char name[MAX_NAME_LENGTH];

    int cash;
    int position;

    enum Strategy strategy;
    enum PlayerStatus status;

    int property_count;
    int railway_count;
    int utility_count;

    int properties[MAX_PROPERTIES];
    int railways[MAX_RAILWAYS];
    int utilities[MAX_UTILITIES];

    struct Loan loan;
    struct Insurance insurance;

    int taxes_due;
    int accrued_interest;
    int insurance_claims;
};

struct Board {
    struct Space spaces[BOARD_SIZE];

    struct Property properties[MAX_PROPERTIES];
    struct Railway railways[MAX_RAILWAYS];
    struct Utility utilities[MAX_UTILITIES];
};

struct Game {
    struct Board board;
    struct Player players[MAX_PLAYERS];

    int turn_order[MAX_PLAYERS];
    int current_player;

    int round;
};

#endif
