
#include "shop.h"
#include <stdlib.h>

#define AVAILABLE_JOKERS 12

static const JokerType available_jokers[AVAILABLE_JOKERS] = {
    JOKER_BONUS_CHIPS,
    JOKER_BASE,
    JOKER_GREEDY,
    JOKER_JOLLY,
    JOKER_ZANY,
    JOKER_MAD,
    JOKER_CRAZY,
    JOKER_DROLL,
    JOKER_SLY,
    JOKER_HALF,
    JOKER_EVEN_STEVEN,
    JOKER_SHOWMAN
};

static int has_joker(const Joker *jokers, int joker_count,
                     JokerType type)
{
    for (int i = 0; i < joker_count; i++)
    {
        if (jokers[i].type == type)
            return 1;
    }

    return 0;
}

void shop_generate_offers(ShopOffer *offers,
                          const Joker *jokers,
                          int joker_count)
{
    if (offers == 0)
        return;

    int showman_active = has_joker(jokers, joker_count,
                                   JOKER_SHOWMAN);

    int available[AVAILABLE_JOKERS];
    int available_count = 0;

    for (int i = 0; i < AVAILABLE_JOKERS; i++)
    {
        JokerType type = available_jokers[i];

        if (showman_active ||
            !has_joker(jokers, joker_count, type))
        {
            available[available_count++] = i;
        }
    }

    for (int i = 0; i < SHOP_OFFER_COUNT; i++)
    {
        int selected;

        if (showman_active)
        {
            /* Showman permite ofertas repetidas */
            selected = rand() % AVAILABLE_JOKERS;
        }
        else
        {
            /* Sin Showman, no se repiten ofertas */
            int position = rand() % available_count;
            selected = available[position];

            available[position] = available[available_count - 1];
            available_count--;
        }

        JokerType type = available_jokers[selected];

        offers[i].joker.type = type;
        offers[i].joker.value =
            (type == JOKER_BONUS_CHIPS) ? 20 : 0;
        offers[i].name = joker_name(type);
        offers[i].price = joker_price(type);
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

    /* Comprobar si Showman está activo */
    int showman_active = 0;

    for (int i = 0; i < *joker_count; i++)
    {
        if (jokers[i].type == JOKER_SHOWMAN)
        {
            showman_active = 1;
            break;
        }
    }

    /* Sin Showman, impedir comprar un Joker repetido */
    if (!showman_active)
    {
        for (int i = 0; i < *joker_count; i++)
        {
            if (jokers[i].type == offer->joker.type)
                return 0;
        }
    }

    jokers[*joker_count] = offer->joker;
    (*joker_count)++;
    *money -= offer->price;

    return 1;
}
