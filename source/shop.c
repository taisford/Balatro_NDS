
#include "shop.h"
#include <stdlib.h>

void shop_generate_offers(ShopOffer *offers)
{
    if (offers == 0)
        return;

    for (int i = 0; i < SHOP_OFFER_COUNT; i++) {
        offers[i].joker.type = JOKER_BONUS_CHIPS;
        offers[i].joker.value = 20;
        offers[i].name = "Joker de bonificacion";
        offers[i].price = 4;
    }
}

int shop_buy_joker(const ShopOffer *offer,
                   Joker *jokers, int *joker_count,
                   int max_jokers, int *money)
{
    if (offer == 0 || jokers == 0 ||
        joker_count == 0 || money == 0)
        return 0;

    if (*joker_count >= max_jokers)
        return 0;

    if (*money < offer->price)
        return 0;

    jokers[*joker_count] = offer->joker;
    (*joker_count)++;
    *money -= offer->price;

    return 1;
}
