
#ifndef SCORING_H
#define SCORING_H

#include "poker.h"

typedef struct {
    int base_chips;
    int base_mult;
    int card_chips;
    int total_chips;
    int total_mult;
    int score;
} ScoreResult;

ScoreResult score_hand(const Card *cards, int count,
                       const PokerResult *poker);

#endif
