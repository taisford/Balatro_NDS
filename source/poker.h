#ifndef POKER_H
#define POKER_H

#include "cards.h"

typedef enum {
    HAND_HIGH_CARD,
    HAND_PAIR,
    HAND_TWO_PAIR,
    HAND_THREE_KIND,
    HAND_STRAIGHT,
    HAND_FLUSH,
    HAND_FULL_HOUSE,
    HAND_FOUR_KIND,
    HAND_STRAIGHT_FLUSH,
    HAND_FIVE_KIND,
    HAND_FLUSH_HOUSE,
    HAND_FLUSH_FIVE
} PokerHandType;

typedef struct {
    PokerHandType type;
    int scoring_count;
    int scoring_indices[HAND_SIZE];
} PokerResult;

PokerResult poker_evaluate(const Card *cards, int count);
const char *poker_hand_name(PokerHandType type);

#endif
