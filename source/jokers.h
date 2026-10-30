
#ifndef JOKERS_H
#define JOKERS_H

#include "poker.h"

#define MAX_JOKERS 5

typedef enum {
    JOKER_NONE,
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
    JOKER_EVEN_STEVEN
} JokerType;

typedef struct {
    JokerType type;
    int value;
} Joker;

void jokers_apply(Joker *jokers, int joker_count,
                  const Card *cards, int count,
                  const PokerResult *poker,
                  int *chips, int *mult);

const char *joker_name(JokerType type);
int joker_price(JokerType type);

#endif
