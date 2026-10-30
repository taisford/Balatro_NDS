
#include "jokers.h"

const char *joker_name(JokerType type)
{
    switch (type)
    {
    case JOKER_BONUS_CHIPS: return "Joker de fichas";
    case JOKER_BASE: return "Joker";
    case JOKER_GREEDY: return "Greedy Joker";
    case JOKER_JOLLY: return "Jolly Joker";
    case JOKER_ZANY: return "Zany Joker";
    case JOKER_MAD: return "Mad Joker";
    case JOKER_CRAZY: return "Crazy Joker";
    case JOKER_DROLL: return "Droll Joker";
    case JOKER_SLY: return "Sly Joker";
    case JOKER_HALF: return "Half Joker";
    case JOKER_EVEN_STEVEN: return "Even Steven";
    case JOKER_SHOWMAN: return "Showman";
    case JOKER_NONE:
    default: return "Vacio";
    }
}

const char *joker_description(JokerType type)
{
    switch (type)
    {
    case JOKER_BONUS_CHIPS:
        return "+20 fichas";

    case JOKER_BASE:
        return "+4 Mult";

    case JOKER_GREEDY:
        return "+3 Mult por cada Diamante puntuable";

    case JOKER_JOLLY:
        return "+8 Mult si la mano contiene Pareja";

    case JOKER_ZANY:
        return "+12 Mult si la mano contiene Trio";

    case JOKER_MAD:
        return "+10 Mult si la mano contiene Doble Pareja";

    case JOKER_CRAZY:
        return "+12 Mult si la mano contiene Escalera";

    case JOKER_DROLL:
        return "+10 Mult si la mano contiene Color";

    case JOKER_SLY:
        return "+50 fichas si la mano contiene Pareja";

    case JOKER_HALF:
        return "+20 Mult si juegas 3 cartas o menos";

    case JOKER_EVEN_STEVEN:
        return "+4 Mult por cada carta par puntuable";

    case JOKER_SHOWMAN:
        return "Permite Jokers repetidos en la tienda";

    case JOKER_NONE:
    default:
        return "";
    }
}

int joker_price(JokerType type)
{
    if (type == JOKER_NONE)
        return 0;

    if (type == JOKER_SHOWMAN)
        return 5;

    return 4;
}

static int hand_has_pair(PokerHandType type)
{
    return type == HAND_PAIR ||
           type == HAND_TWO_PAIR ||
           type == HAND_FULL_HOUSE ||
           type == HAND_FOUR_KIND ||
           type == HAND_FIVE_KIND ||
           type == HAND_FLUSH_HOUSE ||
           type == HAND_FLUSH_FIVE;
}

static int hand_has_three(PokerHandType type)
{
    return type == HAND_THREE_KIND ||
           type == HAND_FULL_HOUSE ||
           type == HAND_FOUR_KIND ||
           type == HAND_FIVE_KIND ||
           type == HAND_FLUSH_HOUSE ||
           type == HAND_FLUSH_FIVE;
}

void jokers_apply(Joker *jokers, int joker_count,
                  const Card *cards, int count,
                  const PokerResult *poker,
                  int *chips, int *mult)
{
    if (jokers == 0 || chips == 0 || mult == 0 ||
        cards == 0 || poker == 0 || count <= 0)
        return;

    for (int i = 0; i < joker_count; i++)
    {
        switch (jokers[i].type)
        {
        case JOKER_BONUS_CHIPS:
            *chips += jokers[i].value;
            break;

        case JOKER_BASE:
            *mult += 4;
            break;

        case JOKER_GREEDY:
            for (int j = 0; j < poker->scoring_count; j++)
            {
                int index = poker->scoring_indices[j];

                if (index >= 0 && index < count &&
                    cards[index].suit == SUIT_DIAMONDS)
                    *mult += 3;
            }
            break;

        case JOKER_JOLLY:
            if (hand_has_pair(poker->type))
                *mult += 8;
            break;

        case JOKER_ZANY:
            if (hand_has_three(poker->type))
                *mult += 12;
            break;

        case JOKER_MAD:
            if (poker->type == HAND_TWO_PAIR)
                *mult += 10;
            break;

        case JOKER_CRAZY:
            if (poker->type == HAND_STRAIGHT ||
                poker->type == HAND_STRAIGHT_FLUSH)
                *mult += 12;
            break;

        case JOKER_DROLL:
            if (poker->type == HAND_FLUSH ||
                poker->type == HAND_STRAIGHT_FLUSH ||
                poker->type == HAND_FLUSH_HOUSE ||
                poker->type == HAND_FLUSH_FIVE)
                *mult += 10;
            break;

        case JOKER_SLY:
            if (hand_has_pair(poker->type))
                *chips += 50;
            break;

        case JOKER_HALF:
            if (count <= 3)
                *mult += 20;
            break;

        case JOKER_EVEN_STEVEN:
            for (int j = 0; j < poker->scoring_count; j++)
            {
                int index = poker->scoring_indices[j];

                if (index >= 0 && index < count)
                {
                    int rank = (int)cards[index].rank;

                    if (rank >= RANK_2 && rank <= RANK_10 &&
                        rank % 2 == 0)
                        *mult += 4;
                }
            }
            break;

        case JOKER_SHOWMAN:
        case JOKER_NONE:
        default:
            break;
        }
    }
}
