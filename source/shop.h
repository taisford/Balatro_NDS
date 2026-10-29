
#ifndef SHOP_H
#define SHOP_H

#include "jokers.h"

#define SHOP_OFFER_COUNT 3

typedef struct {
    Joker joker;
    const char *name;
    int price;
} ShopOffer;

void shop_generate_offers(ShopOffer *offers);
int shop_buy_joker(const ShopOffer *offer,
                   Joker *jokers, int *joker_count,
                   int max_jokers, int *money);

#endif
