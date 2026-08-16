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

#define NUM_NATIONAL_EVENTS 20
#define NUM_REGULATIONS 8
#define NUM_ECONOMIC_EVENTS 8
#define NUM_REGIONAL_CARDS 12

#define BASE_LOAN_INTEREST 8

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

enum InsuranceType {
    NO_INSURANCE,
    BASIC_INSURANCE,
    COMPREHENSIVE_INSURANCE,
    BUSINESS_INTERRUPTION
};

enum NationalEvent {
    TOURISM_HYPE,
    FUEL_SHORTAGE,
    HEAVY_FLOODS,
    POLITICAL_RALLY,
    STOCK_MARKET_RISE,
    ECONOMIC_DOWNTURN,
    HOUSING_SUBSIDY_EVENT,
    INTEREST_RATE_CUT,
    INTEREST_RATE_INCREASE,
    TAX_AMNESTY,
    POWER_FAILURE,
    FOREIGN_FUNDING,
    PORT_EXPANSION,
    FESTIVAL_SEASON,
    LABOUR_STRIKE,
    INSURANCE_DISCOUNT,
    PROPERTY_REVALUATION,
    CURRENCY_DEPRECIATION,
    GOVERNMENT_GRANT,
    NATIONAL_DISASTER
};

enum Regulation {
    REG_INCREASE_PROPERTY_TAX,
    REG_REDUCE_LOAN_INTEREST,
    REG_HOUSING_SUBSIDY,
    REG_LUXURY_PROPERTY_TAX,
    REG_RAILWAY_MODERNIZATION,
    REG_ELECTRICITY_TARIFF_REVISION,
    REG_INSURANCE_REGULATION,
    REG_ANTI_SPECULATION_ACT
};

enum EconomicEvent {
    ECO_TOURISM_BOOM,
    ECO_FUEL_CRISIS,
    ECO_HEAVY_MONSOON,
    ECO_ECONOMIC_RECESSION,
    ECO_STOCK_MARKET_BOOM,
    ECO_GOVERNMENT_HOUSING_PROGRAMME,
    ECO_FOREIGN_INVESTMENT,
    ECO_POLITICAL_UNREST
};

enum RegionalCard {
    REG_CARD_SOUTHERN_TOURISM_BOOM,
    REG_CARD_PORT_CITY_EXPANSION,
    REG_CARD_IT_INDUSTRY_GROWTH,
    REG_CARD_NORTHERN_DEVELOPMENT,
    REG_CARD_TEA_EXPORT_BOOM,
    REG_CARD_AIRPORT_EXPANSION,
    REG_CARD_UNIVERSITY_CITY_GROWTH,
    REG_CARD_BEACH_POLLUTION,
    REG_CARD_FLOOD_DAMAGE,
    REG_CARD_TRANSPORT_STRIKE,
    REG_CARD_ELECTRICITY_TARIFF_INCREASE,
    REG_CARD_WATER_SHORTAGE
};

struct Loan {
    int active;
    int principal;
    int interest;
    int rate;                 
    int rounds_remaining;
};

struct Insurance {
    int active;
    enum InsuranceType type;
    int premium;
    int claim;
    int property_index;       
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

    int age;                   
    int depreciation;         
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

    int has_active_event;
    enum NationalEvent active_event;
    int event_rounds_remaining;
};

struct Board {
    struct Space spaces[BOARD_SIZE];

    struct Property properties[MAX_PROPERTIES];
    struct Railway railways[MAX_RAILWAYS];
    struct Utility utilities[MAX_UTILITIES];
};

struct Economy {
    int inflation_rate;             
    int loan_interest_rate;         

    int boom_group;                 
    int boom_rounds_remaining;
    int boom_snapshot_price[MAX_PROPERTIES];
    int boom_snapshot_mortgage[MAX_PROPERTIES];
    int boom_snapshot_rent[MAX_PROPERTIES];

    int decline_group;             
    int decline_rounds_remaining;
    int decline_snapshot_price[MAX_PROPERTIES];
    int decline_snapshot_mortgage[MAX_PROPERTIES];
    int decline_snapshot_rent[MAX_PROPERTIES];

    int has_regional_card;
    enum RegionalCard regional_card;
    int regional_rounds_remaining;
    int regional_snapshot_price[MAX_PROPERTIES];
    int regional_snapshot_rent[MAX_PROPERTIES];
    int regional_snapshot_touched[MAX_PROPERTIES];

    int event_deck[NUM_NATIONAL_EVENTS];
    int event_deck_pos;
};
struct Game {
    struct Board board;
    struct Player players[MAX_PLAYERS];

    int turn_order[MAX_PLAYERS];
    int current_player;

    int round;

    int last_dice_roll;

    struct Economy economy;
};

#endif
