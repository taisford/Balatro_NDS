#include "poker.h"

#include <string.h>

const char *poker_hand_name(PokerHandType type)
{
    static const char *names[] = {
        "High Card",
        "Pair",
        "Two Pair",
        "Three of a Kind",
        "Straight",
        "Flush",
        "Full House",
        "Four of a Kind",
        "Straight Flush",
        "Five of a Kind",
        "Flush House",
        "Flush Five"
    };

    if (type < HAND_HIGH_CARD || type > HAND_FLUSH_FIVE)
        return "Unknown";

    return names[type];
}

PokerResult poker_evaluate(const Card *cards, int count)
{
    PokerResult result = {0};
    result.type = HAND_HIGH_CARD;

    if (cards == NULL || count <= 0 || count > 5)
    	return result;

    int rank_count[15] = {0};
    int suit_count[4] = {0};
    int rank_mask = 0;
    int flush_suit = -1;

    for (int i = 0; i < count; i++) {
        int rank = cards[i].rank;
        int suit = cards[i].suit;

        if (rank >= 2 && rank <= 14) {
            rank_count[rank]++;
            rank_mask |= 1 << rank;
        }

        if (suit >= 0 && suit < 4)
            suit_count[suit]++;
    }

    for (int s = 0; s < 4; s++) {
        if (suit_count[s] >= 5) {
            flush_suit = s;
            break;
        }
    }

    int pairs = 0;
    int triples = 0;
    int fours = 0;
    int fives = 0;
    int pair_rank = 0;
    int triple_rank = 0;
    int four_rank = 0;
    int five_rank = 0;

    for (int r = 14; r >= 2; r--) {
        if (rank_count[r] == 2) {
            pairs++;
            if (!pair_rank) pair_rank = r;
        } else if (rank_count[r] == 3) {
            triples++;
            triple_rank = r;
        } else if (rank_count[r] == 4) {
            fours++;
            four_rank = r;
        } else if (rank_count[r] >= 5) {
            fives++;
            five_rank = r;
        }
    }

    int straight_high = 0;
    int run = 0;

    for (int r = 2; r <= 14; r++) {
        if (rank_mask & (1 << r)) {
            run++;
            if (run >= 5)
                straight_high = r;
        } else {
            run = 0;
        }
    }

    /* Ace-low straight: A, 2, 3, 4, 5. */
    if (!straight_high &&
        (rank_mask & (1 << 14)) &&
        (rank_mask & (1 << 2)) &&
        (rank_mask & (1 << 3)) &&
        (rank_mask & (1 << 4)) &&
        (rank_mask & (1 << 5))) {
        straight_high = 5;
    }

    int flush_five = 0;
    int flush_house = 0;
    int straight_flush = 0;

    if (count == 5 && flush_suit >= 0) {
        if (fives)
            flush_five = 1;

        if (triples && pairs)
            flush_house = 1;

        if (straight_high)
            straight_flush = 1;
    }

    if (flush_five) {
        result.type = HAND_FLUSH_FIVE;
    } else if (flush_house) {
        result.type = HAND_FLUSH_HOUSE;
    } else if (fives) {
        result.type = HAND_FIVE_KIND;
    } else if (straight_flush) {
        result.type = HAND_STRAIGHT_FLUSH;
    } else if (fours) {
        result.type = HAND_FOUR_KIND;
    } else if (triples && pairs) {
        result.type = HAND_FULL_HOUSE;
    } else if (flush_suit >= 0) {
        result.type = HAND_FLUSH;
    } else if (straight_high) {
        result.type = HAND_STRAIGHT;
    } else if (triples) {
        result.type = HAND_THREE_KIND;
    } else if (pairs >= 2) {
        result.type = HAND_TWO_PAIR;
    } else if (pairs == 1) {
        result.type = HAND_PAIR;
    }

    /*
     * Select the cards contributing to the chosen hand.
     * This first version handles standard hands; special
     * Balatro modifiers will be added later.
     */
    for (int i = 0; i < count; i++) {
        int r = cards[i].rank;
        int include = 0;

        switch (result.type) {
        case HAND_FLUSH:
        case HAND_FLUSH_FIVE:
        case HAND_FLUSH_HOUSE:
            include = (cards[i].suit == flush_suit);
            break;

        case HAND_STRAIGHT:
        case HAND_STRAIGHT_FLUSH:
            include = (straight_high == 5 && r == 14) ||
                      (r >= straight_high - 4 && r <= straight_high);
            break;

        case HAND_FOUR_KIND:
            include = (r == four_rank);
            break;

        case HAND_FIVE_KIND:
            include = (r == five_rank);
            break;

        case HAND_FULL_HOUSE:
            include = (r == triple_rank) ||
                      (r == pair_rank);
            break;

        case HAND_THREE_KIND:
            include = (r == triple_rank);
            break;

        case HAND_PAIR:
            include = (rank_count[r] == 2);
            break;

        case HAND_TWO_PAIR:
            include = (rank_count[r] == 2);
            break;

        case HAND_HIGH_CARD:
            include = 1;
            break;

        default:
            break;
        }

        if (include && result.scoring_count < HAND_SIZE) {
            result.scoring_indices[result.scoring_count++] = i;
        }
    }

    /* High Card scores only the highest card. */
    if (result.type == HAND_HIGH_CARD && count > 0) {
        int best = 0;
        for (int i = 1; i < count; i++) {
            if (cards[i].nominal > cards[best].nominal)
                best = i;
        }
        result.scoring_indices[0] = best;
        result.scoring_count = 1;
    }

    return result;
}
