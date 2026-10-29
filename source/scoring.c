
#include "scoring.h"
#include "jokers.h"

static void get_base_score(PokerHandType type,
                           int *chips, int *mult)
{
    switch (type)
    {
    case HAND_HIGH_CARD:
        *chips = 5;
        *mult = 1;
        break;
    case HAND_PAIR:
        *chips = 10;
        *mult = 2;
        break;
    case HAND_TWO_PAIR:
        *chips = 20;
        *mult = 2;
        break;
    case HAND_THREE_KIND:
        *chips = 30;
        *mult = 3;
        break;
    case HAND_STRAIGHT:
        *chips = 30;
        *mult = 4;
        break;
    case HAND_FLUSH:
        *chips = 35;
        *mult = 4;
        break;
    case HAND_FULL_HOUSE:
        *chips = 40;
        *mult = 4;
        break;
    case HAND_FOUR_KIND:
        *chips = 60;
        *mult = 7;
        break;
    case HAND_STRAIGHT_FLUSH:
        *chips = 100;
        *mult = 8;
        break;
    default:
        *chips = 5;
        *mult = 1;
        break;
    }
}

ScoreResult score_hand(const Card *cards, int count,
                       const PokerResult *poker,
                       const Joker *jokers, int joker_count)
{
    ScoreResult result = {0};

    if (cards == 0 || poker == 0 || count <= 0)
        return result;

    get_base_score(poker->type,
                   &result.base_chips,
                   &result.base_mult);

    result.card_chips = 0;

    for (int i = 0; i < poker->scoring_count; i++)
    {
        int index = poker->scoring_indices[i];

        if (index >= 0 && index < count)
            result.card_chips += cards[index].nominal;
    }

    result.total_chips = result.base_chips + result.card_chips;
    result.total_mult = result.base_mult;

    jokers_apply((Joker *)jokers, joker_count,
                 &result.total_chips, &result.total_mult);

    result.score = result.total_chips * result.total_mult;

    return result;
}
