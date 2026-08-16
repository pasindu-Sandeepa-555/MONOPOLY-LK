#include <stdio.h>
#include <stdlib.h>

#include "../include/events.h"

static const char *nationalEventName(enum NationalEvent e)
{
    switch (e) {
        case TOURISM_HYPE:          return "Tourism Hype";
        case FUEL_SHORTAGE:         return "Fuel Shortage";
        case HEAVY_FLOODS:          return "Heavy Floods";
        case POLITICAL_RALLY:       return "Political Rally";
        case STOCK_MARKET_RISE:     return "Stock Market Rise";
        case ECONOMIC_DOWNTURN:     return "Economic Downturn";
        case HOUSING_SUBSIDY_EVENT: return "Housing Subsidy";
        case INTEREST_RATE_CUT:     return "Interest Rate Cut";
        case INTEREST_RATE_INCREASE:return "Interest Rate Increase";
        case TAX_AMNESTY:           return "Tax Amnesty";
        case POWER_FAILURE:         return "Power Failure";
        case FOREIGN_FUNDING:       return "Foreign Funding";
        case PORT_EXPANSION:        return "Port Expansion";
        case FESTIVAL_SEASON:       return "Festival Season";
        case LABOUR_STRIKE:         return "Labour Strike";
        case INSURANCE_DISCOUNT:    return "Insurance Discount";
        case PROPERTY_REVALUATION:  return "Property Revaluation";
        case CURRENCY_DEPRECIATION: return "Currency Depreciation";
        case GOVERNMENT_GRANT:      return "Government Grant";
        case NATIONAL_DISASTER:     return "National Disaster";
        default:                    return "Unknown Event";
    }
}

static const char *regulationName(enum Regulation r)
{
    switch (r) {
        case REG_INCREASE_PROPERTY_TAX:        return "Increase Property Tax";
        case REG_REDUCE_LOAN_INTEREST:          return "Reduce Loan Interest";
        case REG_HOUSING_SUBSIDY:               return "Housing Subsidy";
        case REG_LUXURY_PROPERTY_TAX:           return "Luxury Property Tax";
        case REG_RAILWAY_MODERNIZATION:         return "Railway Modernization";
        case REG_ELECTRICITY_TARIFF_REVISION:   return "Electricity Tariff Revision";
        case REG_INSURANCE_REGULATION:          return "Insurance Regulation";
        case REG_ANTI_SPECULATION_ACT:          return "Anti-Speculation Act";
        default:                                return "Unknown Regulation";
    }
}

static const char *economicEventName(enum EconomicEvent e)
{
    switch (e) {
        case ECO_TOURISM_BOOM:                  return "Tourism Boom";
        case ECO_FUEL_CRISIS:                   return "Fuel Crisis";
        case ECO_HEAVY_MONSOON:                 return "Heavy Monsoon";
        case ECO_ECONOMIC_RECESSION:            return "Economic Recession";
        case ECO_STOCK_MARKET_BOOM:             return "Stock Market Boom";
        case ECO_GOVERNMENT_HOUSING_PROGRAMME:  return "Government Housing Programme";
        case ECO_FOREIGN_INVESTMENT:            return "Foreign Investment";
        case ECO_POLITICAL_UNREST:              return "Political Unrest";
        default:                                return "Unknown Economic Event";
    }
}

static const char *regionalCardName(enum RegionalCard c)
{
    switch (c) {
        case REG_CARD_SOUTHERN_TOURISM_BOOM:       return "Southern Tourism Boom";
        case REG_CARD_PORT_CITY_EXPANSION:         return "Port City Expansion";
        case REG_CARD_IT_INDUSTRY_GROWTH:          return "IT Industry Growth";
        case REG_CARD_NORTHERN_DEVELOPMENT:        return "Northern Development Programme";
        case REG_CARD_TEA_EXPORT_BOOM:             return "Tea Export Boom";
        case REG_CARD_AIRPORT_EXPANSION:           return "Airport Expansion";
        case REG_CARD_UNIVERSITY_CITY_GROWTH:      return "University City Growth";
        case REG_CARD_BEACH_POLLUTION:             return "Beach Pollution";
        case REG_CARD_FLOOD_DAMAGE:                return "Flood Damage";
        case REG_CARD_TRANSPORT_STRIKE:            return "Transport Strike";
        case REG_CARD_ELECTRICITY_TARIFF_INCREASE: return "Electricity Tariff Increase";
        case REG_CARD_WATER_SHORTAGE:               return "Water Shortage";
        default:                                    return "Unknown Regional Card";
    }
}

void initializeEconomy(struct Economy *economy)
{
    economy->inflation_rate = 0;
    economy->loan_interest_rate = BASE_LOAN_INTEREST;

    economy->boom_group = -1;
    economy->boom_rounds_remaining = 0;

    economy->decline_group = -1;
    economy->decline_rounds_remaining = 0;

    economy->has_regional_card = 0;
    economy->regional_card = REG_CARD_SOUTHERN_TOURISM_BOOM;
    economy->regional_rounds_remaining = 0;

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        economy->boom_snapshot_price[i] = 0;
        economy->boom_snapshot_mortgage[i] = 0;
        economy->boom_snapshot_rent[i] = 0;

        economy->decline_snapshot_price[i] = 0;
        economy->decline_snapshot_mortgage[i] = 0;
        economy->decline_snapshot_rent[i] = 0;

        economy->regional_snapshot_price[i] = 0;
        economy->regional_snapshot_rent[i] = 0;
        economy->regional_snapshot_touched[i] = 0;
    }

    for (int i = 0; i < NUM_NATIONAL_EVENTS; i++) {
        economy->event_deck[i] = i;
    }
    economy->event_deck_pos = 0;
}

static void adjustPropertyValue(struct Property *property, int percent)
{
    property->price += (property->price * percent) / 100;
    property->mortgage_value += (property->mortgage_value * percent) / 100;

    if (property->price < 100) property->price = 100;
    if (property->mortgage_value < 50) property->mortgage_value = 50;
}

static void adjustPropertyRent(struct Property *property, int percent)
{
    property->rent += (property->rent * percent) / 100;
    if (property->rent < 10) property->rent = 10;
}

static void snapshotForRegionalCard(struct Game *game, int idx)
{
    if (!game->economy.regional_snapshot_touched[idx]) {
        game->economy.regional_snapshot_price[idx] = game->board.properties[idx].price;
        game->economy.regional_snapshot_rent[idx] = game->board.properties[idx].rent;
        game->economy.regional_snapshot_touched[idx] = 1;
    }
}

static void applyNationalEventEffect(struct Game *game, int player_index, enum NationalEvent card)
{
    struct Player *player = &game->players[player_index];

    printf("National Event Card\n\n%s\n", nationalEventName(card));

   
    switch (card) {

        case TAX_AMNESTY:
            for (int i = 0; i < MAX_PLAYERS; i++) {
                game->players[i].cash += 2000;
            }
            printf("Each player receives LKR 2,000.\n");
            break;

        case GOVERNMENT_GRANT: {
            int lucky = rand() % MAX_PLAYERS;
            game->players[lucky].cash += 5000;
            printf("%s receives LKR 5,000.\n", game->players[lucky].name);
            break;
        }

        case INTEREST_RATE_CUT:
         
			       game->economy.loan_interest_rate -= 2;

            if (game->economy.loan_interest_rate < 1) game->economy.loan_interest_rate = 1;
            printf("Loan interest reduced by 2%%. New rate: %d%%\n",
                   game->economy.loan_interest_rate);
            break;

        case INTEREST_RATE_INCREASE:
            game->economy.loan_interest_rate += 2;
            printf("Loan interest increased by 2%%. New rate: %d%%\n",
                   game->economy.loan_interest_rate);
            break;

        case HEAVY_FLOODS:
        case NATIONAL_DISASTER: {
            int idx = rand() % MAX_PROPERTIES;
            struct Property *p = &game->board.properties[idx];
            if (p->houses > 0 || p->hotel > 0) {
                int repair_cost = p->price / 10;
                if (game->players[p->owner].cash >= repair_cost) {
                    game->players[p->owner].cash -= repair_cost;
                }
                printf("%s damaged. Repair cost LKR %d charged to owner.\n",
                       p->name, repair_cost);
            } else {
                printf("No developed property was affected.\n");
            }
            break;
        }

        case STOCK_MARKET_RISE:
            printf("All property values increase by 10%% for this player's holdings.\n");
            break;

        case ECONOMIC_DOWNTURN:
            printf("Property values decrease by 15%% for this player's holdings.\n");
            break;

        case FUEL_SHORTAGE:
            printf("Railway rent doubles for this player for 5 rounds.\n");
            break;

        case FOREIGN_FUNDING:
            printf("Commercial property values increase by 15%% for this player's holdings.\n");
            break;

        case PORT_EXPANSION:
            printf("Railway station values increase by 20%% for this player's holdings.\n");
            break;

        case PROPERTY_REVALUATION:
            printf("A random property group this player owns into appreciates by 15%%.\n");
            break;

        case HOUSING_SUBSIDY_EVENT:
            printf("House construction cost reduced by 30%% for this player for 15 rounds.\n");
            break;

        case FESTIVAL_SEASON:
            printf("Hotels receive 50%% additional rent for this player for 15 rounds.\n");
            break;

        case INSURANCE_DISCOUNT:
            printf("Insurance premiums reduced by 20%% for this player for 15 rounds.\n");
            break;

        case CURRENCY_DEPRECIATION:
            printf("Construction costs increase by 10%% for this player.\n");
            break;

        case POLITICAL_RALLY:
            printf("A random property is closed for 2 rounds. (Effect tracked qualitatively.)\n");
            break;

        case POWER_FAILURE:
            printf("Utility income halved for 3 rounds. (Effect tracked qualitatively.)\n");
            break;

        case LABOUR_STRIKE:
            printf("Construction suspended for 2 rounds. (Effect tracked qualitatively.)\n");
            break;

        default:
            break;
    }

    player->has_active_event = 1;
    player->active_event = card;
    player->event_rounds_remaining = 15;

    printf("\n");
}

void drawNationalEventCard(struct Game *game, int player_index)
{
    int card = game->economy.event_deck[game->economy.event_deck_pos];

    game->economy.event_deck_pos =
        (game->economy.event_deck_pos + 1) % NUM_NATIONAL_EVENTS;

    applyNationalEventEffect(game, player_index, (enum NationalEvent)card);
}

static void triggerInflation(struct Game *game)
{
    int rates[6] = {-3, 0, 2, 5, 8, 12};
    int rate = rates[rand() % 6];

    game->economy.inflation_rate = rate;

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        struct Property *p = &game->board.properties[i];
        p->price += (p->price * rate) / 100;
        p->mortgage_value += (p->mortgage_value * rate) / 100;
        p->rent += (p->rent * rate) / 100;

        if (p->price < 100) p->price = 100;
        if (p->mortgage_value < 50) p->mortgage_value = 50;
        if (p->rent < 10) p->rent = 10;
    }

    printf("Inflation\n------------\n%+d%%\n\n", rate);
}

static void triggerMarketReview(struct Game *game)
{
    int boom_group = 1 + (rand() % 8);
    int decline_group;

    do {
        decline_group = 1 + (rand() % 8);
    } while (decline_group == boom_group);

    /* Snapshot pre-boom / pre-decline values so they can be restored
       exactly when the 10-round window ends (Rule-LK 31/32). */
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        struct Property *p = &game->board.properties[i];

        if (p->group == boom_group) {
            game->economy.boom_snapshot_price[i] = p->price;
            game->economy.boom_snapshot_mortgage[i] = p->mortgage_value;
            game->economy.boom_snapshot_rent[i] = p->rent;
        }

        if (p->group == decline_group) {
            game->economy.decline_snapshot_price[i] = p->price;
            game->economy.decline_snapshot_mortgage[i] = p->mortgage_value;
            game->economy.decline_snapshot_rent[i] = p->rent;
        }
    }

    game->economy.boom_group = boom_group;
    game->economy.boom_rounds_remaining = 10;

    game->economy.decline_group = decline_group;
    game->economy.decline_rounds_remaining = 10;

    for (int i = 0; i < MAX_PROPERTIES; i++) {
        struct Property *p = &game->board.properties[i];

        if (p->group == boom_group) {
            adjustPropertyValue(p, 20);
            adjustPropertyRent(p, 25);
        }
        else if (p->group == decline_group) {
            adjustPropertyValue(p, -15);
            adjustPropertyRent(p, -20);
        }
    }

    printf("Property Market Review\nMarket Boom : Group %d\nMarket Decline : Group %d\n\n",
           boom_group, decline_group);
}

static void revertMarketBoom(struct Game *game, int group)
{
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        if (game->board.properties[i].group == group) {
            game->board.properties[i].price = game->economy.boom_snapshot_price[i];
            game->board.properties[i].mortgage_value = game->economy.boom_snapshot_mortgage[i];
            game->board.properties[i].rent = game->economy.boom_snapshot_rent[i];
        }
    }
    printf("Market Boom on Group %d has ended. Values reverted to pre-boom levels.\n\n", group);
}

static void revertMarketDecline(struct Game *game, int group)
{
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        if (game->board.properties[i].group == group) {
            game->board.properties[i].price = game->economy.decline_snapshot_price[i];
            game->board.properties[i].mortgage_value = game->economy.decline_snapshot_mortgage[i];
            game->board.properties[i].rent = game->economy.decline_snapshot_rent[i];
        }
    }
    printf("Market Decline on Group %d has ended. Values reverted to pre-decline levels.\n\n", group);
}

static void revertRegionalCard(struct Game *game)
{
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        if (game->economy.regional_snapshot_touched[i]) {
            game->board.properties[i].price = game->economy.regional_snapshot_price[i];
            game->board.properties[i].rent = game->economy.regional_snapshot_rent[i];
            game->economy.regional_snapshot_touched[i] = 0;
        }
    }
    printf("Regional Development period has ended. Affected property values reverted.\n\n");
}

static void triggerEconomicEvent(struct Game *game)
{
        enum EconomicEvent evt = (enum EconomicEvent)(rand() % NUM_ECONOMIC_EVENTS);

    printf("Economic Event\n\n%s\n", economicEventName(evt));

    switch (evt) {
        case ECO_STOCK_MARKET_BOOM:
            for (int i = 0; i < MAX_PROPERTIES; i++) {
                adjustPropertyValue(&game->board.properties[i], 10);
            }
            game->economy.loan_interest_rate -= 1;
            if (game->economy.loan_interest_rate < 1) game->economy.loan_interest_rate = 1;
            printf("Property values increase by 10%%. Loan interest decreases.\n");
            break;

        case ECO_ECONOMIC_RECESSION:
            for (int i = 0; i < MAX_PROPERTIES; i++) {
                adjustPropertyValue(&game->board.properties[i], -15);
                adjustPropertyRent(&game->board.properties[i], -10);
            }
            game->economy.loan_interest_rate += 2;
            printf("Property values decrease by 15%%. Rent decreases 10%%. Loan interest rises.\n");
            break;

        case ECO_TOURISM_BOOM:
            printf("Hotels receive double rent. Southern coastal properties increase 15%%.\n");
            break;

        case ECO_FUEL_CRISIS:
            for (int i = 0; i < MAX_RAILWAYS; i++) {
                game->board.railways[i].rent *= 2;
            }
            printf("Railway rent doubles. Development costs increase 20%%.\n");
            break;

        case ECO_HEAVY_MONSOON:
            printf("Flood risk increases. Insurance premiums increase. Coastal properties -10%%.\n");
            break;

        case ECO_GOVERNMENT_HOUSING_PROGRAMME:
            printf("House construction costs reduce 25%%.\n");
            break;

        case ECO_FOREIGN_INVESTMENT:
            for (int i = 0; i < MAX_PROPERTIES; i++) {
                adjustPropertyValue(&game->board.properties[i], 20);
            }
            printf("Commercial properties increase 20%%.\n");
            break;

        case ECO_POLITICAL_UNREST:
            printf("Riot probability doubles. Hotel rent drops 50%%.\n");
            break;
    }

    printf("\n");
}

static void triggerGovernmentRegulation(struct Game *game)
{
    enum Regulation reg = (enum Regulation)(rand() % NUM_REGULATIONS);

    printf("Government Regulation\n\n%s Introduced.\n", regulationName(reg));

    switch (reg) {
        case REG_REDUCE_LOAN_INTEREST:
            game->economy.loan_interest_rate -= 2;
            if (game->economy.loan_interest_rate < 1) game->economy.loan_interest_rate = 1;
            printf("Loan interest reduced by 2%%. New rate: %d%%\n",
                   game->economy.loan_interest_rate);
            break;

        case REG_HOUSING_SUBSIDY:
            printf("House construction costs reduced by 30%%.\n");
            break;

        case REG_RAILWAY_MODERNIZATION:
            for (int i = 0; i < MAX_RAILWAYS; i++) {
                game->board.railways[i].rent += game->board.railways[i].rent / 4;
            }
            printf("Railway rents increase by 25%%.\n");
            break;

        case REG_INCREASE_PROPERTY_TAX:
            printf("Income Tax increases by 50%%.\n");
            break;

        case REG_LUXURY_PROPERTY_TAX:
            printf("Hotels incur an annual maintenance tax of 25%% of property value.\n");
            break;

        case REG_ELECTRICITY_TARIFF_REVISION:
            printf("Utility rents increase by 20%%.\n");
            break;

        case REG_INSURANCE_REGULATION:
            printf("Insurance premiums decrease by 15%%. Coverage unchanged.\n");
            break;

        case REG_ANTI_SPECULATION_ACT:
            printf("Players may own at most three undeveloped properties.\n");
            break;
    }

    printf("\n");
}

static void triggerRegionalDevelopmentCard(struct Game *game)
{
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        game->economy.regional_snapshot_touched[i] = 0;
    }

    enum RegionalCard card = (enum RegionalCard)(rand() % NUM_REGIONAL_CARDS);

    game->economy.has_regional_card = 1;
    game->economy.regional_card = card;
    game->economy.regional_rounds_remaining = 15;

    printf("Regional Development Card\n\n%s\n", regionalCardName(card));

    switch (card) {
        case REG_CARD_SOUTHERN_TOURISM_BOOM: /* Galle Fort(14) Unawatuna(15) Hikkaduwa(16) */
            for (int i = 14; i <= 16; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyRent(&game->board.properties[i], 40);
            }
            printf("Galle Fort, Unawatuna and Hikkaduwa rental income +40%%\n");
            break;

        case REG_CARD_PORT_CITY_EXPANSION: /* Pettah(0) Maradana(1) */
            for (int i = 0; i <= 1; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyValue(&game->board.properties[i], 25);
            }
            printf("Pettah and Maradana values +25%%\n");
            break;

        case REG_CARD_IT_INDUSTRY_GROWTH: /* Nugegoda(5) Maharagama(6) Kottawa(7) */
            for (int i = 5; i <= 7; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyValue(&game->board.properties[i], 20);
            }
            printf("Maharagama, Nugegoda and Kottawa values +20%%\n");
            break;

        case REG_CARD_NORTHERN_DEVELOPMENT: /* Jaffna Town(17) Nallur(18) Trincomalee(19) */
            for (int i = 17; i <= 19; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyValue(&game->board.properties[i], 30);
            }
            printf("Jaffna Town, Nallur and Trincomalee values +30%%\n");
            break;

        case REG_CARD_TEA_EXPORT_BOOM: /* Nuwara Eliya(20) */
            snapshotForRegionalCard(game, 20);
            adjustPropertyValue(&game->board.properties[20], 35);
            printf("Nuwara Eliya value +35%%\n");
            break;

        case REG_CARD_AIRPORT_EXPANSION: /* Negombo(8) Katunayake(9) Ja-Ela(10) */
            for (int i = 8; i <= 10; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyRent(&game->board.properties[i], 30);
            }
            printf("Negombo, Katunayake and Ja-Ela rents +30%%\n");
            break;

        case REG_CARD_UNIVERSITY_CITY_GROWTH: /* Kandy City(11) Peradeniya(12) */
            snapshotForRegionalCard(game, 11);
            adjustPropertyValue(&game->board.properties[11], 20);
            snapshotForRegionalCard(game, 12);
            adjustPropertyValue(&game->board.properties[12], 20);
            printf("Peradeniya and Kandy City values +20%%\n");
            break;

        case REG_CARD_BEACH_POLLUTION: /* Galle Fort(14) Unawatuna(15) Hikkaduwa(16) */
            for (int i = 14; i <= 16; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyRent(&game->board.properties[i], -30);
            }
            printf("Southern coastal rents -30%%\n");
            break;

        case REG_CARD_FLOOD_DAMAGE: /* Negombo(8) Katunayake(9) Ja-Ela(10) as low-lying coastal */
            for (int i = 8; i <= 10; i++) {
                snapshotForRegionalCard(game, i);
                adjustPropertyValue(&game->board.properties[i], -20);
            }
            printf("Low-lying coastal properties lose 20%% value\n");
            break;

        case REG_CARD_TRANSPORT_STRIKE:
            for (int i = 0; i < MAX_RAILWAYS; i++) {
                game->board.railways[i].rent -= game->board.railways[i].rent * 40 / 100;
                if (game->board.railways[i].rent < 50) game->board.railways[i].rent = 50;
            }
            printf("Railway revenue reduced by 40%%\n");
            break;

        case REG_CARD_ELECTRICITY_TARIFF_INCREASE:
            printf("Utility rent +25%%\n");
            break;

        case REG_CARD_WATER_SHORTAGE:
            printf("Water utility revenue +20%%; surrounding properties -10%%\n");
            break;
    }

    printf("\n");
}

static void triggerDisaster(struct Game *game)
{
    const char *disasters[5] = {
        "Fire", "Flood", "Riot", "Building Collapse", "Electrical Failure"
    };

    int idx = rand() % MAX_PROPERTIES;
    struct Property *p = &game->board.properties[idx];

    if (p->owner == -1 || (p->houses == 0 && p->hotel == 0)) {
        return; /* only developed, owned properties can be damaged */
    }

    const char *disaster = disasters[rand() % 5];
    struct Player *owner = &game->players[p->owner];

    int repair_cost = p->price / 10;
    if (repair_cost < 100) repair_cost = 100;

    printf("Disaster\n\n%s occurred.\n\nAffected Property :\n\n%s.\n\n",
           disaster, p->name);

    if (owner->insurance.active && owner->insurance.property_index == idx) {
        int coverage_percent = 80;
        if (owner->insurance.type == COMPREHENSIVE_INSURANCE) coverage_percent = 100;

        int compensation = (repair_cost * coverage_percent) / 100;
        owner->cash += compensation;
        owner->insurance_claims++;

        printf("Insurance Claim Approved.\n\nCompensation Paid :\nLKR %d.\n\n", compensation);
    }
    else {
        if (owner->cash >= repair_cost) {
            owner->cash -= repair_cost;
        }
        printf("Property uninsured. Owner bears repair cost of LKR %d.\n\n", repair_cost);
    }
}

static void updatePropertyDepreciation(struct Game *game)
{
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        struct Property *p = &game->board.properties[i];

        if (p->owner == -1) continue;

        p->age++;

        if (p->age > 50 && p->age % 5 == 0 && p->depreciation < 30) {
            p->depreciation += 1;

            printf("Property\n\n%s\n\nhas depreciated by %d%%.\n\nCurrent Value\n\nLKR %d.\n\n",
                   p->name, p->depreciation,
                   p->price - (p->price * p->depreciation / 100));
        }
    }
}

static void updateNationalEventTimers(struct Game *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        struct Player *player = &game->players[i];

        if (!player->has_active_event) continue;

        player->event_rounds_remaining--;

        if (player->event_rounds_remaining <= 0) {
            player->has_active_event = 0;
        }
    }
}

void runEconomicSystems(struct Game *game)
{
    updatePropertyDepreciation(game);
    updateNationalEventTimers(game);

    if (game->economy.boom_rounds_remaining > 0) {
        game->economy.boom_rounds_remaining--;
        if (game->economy.boom_rounds_remaining == 0) {
            revertMarketBoom(game, game->economy.boom_group);
            game->economy.boom_group = -1;
        }
    }

    if (game->economy.decline_rounds_remaining > 0) {
        game->economy.decline_rounds_remaining--;
        if (game->economy.decline_rounds_remaining == 0) {
            revertMarketDecline(game, game->economy.decline_group);
            game->economy.decline_group = -1;
        }
    }

    if (game->economy.regional_rounds_remaining > 0) {
        game->economy.regional_rounds_remaining--;
        if (game->economy.regional_rounds_remaining == 0) {
            revertRegionalCard(game);
            game->economy.has_regional_card = 0;
        }
    }

    if (game->round % 10 == 0) {
        triggerInflation(game);
        triggerMarketReview(game);
        triggerDisaster(game);
    }

    if (game->round % 15 == 0) {
        triggerEconomicEvent(game);
        triggerRegionalDevelopmentCard(game);
    }

    if (game->round % 20 == 0) {
        triggerGovernmentRegulation(game);
    }

    printCurrentMarketConditions(game);
}

void printCurrentMarketConditions(const struct Game *game)
{
    printf("=========================================\n");
    printf("Current Market Conditions\n");
    printf("=========================================\n\n");

    if (game->economy.boom_group != -1) {
        printf("Market Boom\n-------------\nGroup %d (+20%%)\nRounds Remaining : %d\n\n",
               game->economy.boom_group, game->economy.boom_rounds_remaining);
    }

    if (game->economy.decline_group != -1) {
        printf("Market Decline\n----------------\nGroup %d (-15%%)\nRounds Remaining : %d\n\n",
               game->economy.decline_group, game->economy.decline_rounds_remaining);
    }

    if (game->economy.has_regional_card) {
        printf("Regional Development\n-----------------------\n%s\nRounds Remaining : %d\n\n",
               regionalCardName(game->economy.regional_card),
               game->economy.regional_rounds_remaining);
    }

    printf("Inflation\n------------\n%+d%%\n\n", game->economy.inflation_rate);

    printf("Current Loan Interest\n-----------------------\n%d%%\n\n",
           game->economy.loan_interest_rate);

    printf("=========================================\n\n");
}
