#include "cards.h"

#include <stdlib.h>

void deck_init(Deck *deck)
{
    int index = 0;

    for (int suit = SUIT_DIAMONDS;
         suit <= SUIT_SPADES; suit++) {

        for (int rank = RANK_2;
             rank <= RANK_ACE; rank++) {

            Card *card = &deck->cards[index++];

            card->suit = (Suit)suit;
            card->rank = (Rank)rank;
            card->id = (uint8_t)rank;

            if (rank <= RANK_10) {
                card->nominal = (uint8_t)rank;
            } else if (rank == RANK_ACE) {
                card->nominal = 11;
            } else {
                card->nominal = 10;
            }
        }
    }

    deck->next_card = 0;
    deck->card_count = DECK_SIZE;
}

void deck_shuffle(Deck *deck)
{
    for (int i = deck->card_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }

    deck->next_card = 0;
}

int deck_draw(Deck *deck, Card *card)
{
    if (deck->next_card >= deck->card_count)
        return 0;

    *card = deck->cards[deck->next_card++];
    return 1;
}

const char *card_rank_name(Rank rank)
{
    static const char *names[] = {
        "", "", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "Jack", "Queen", "King", "Ace"
    };

    if (rank < RANK_2 || rank > RANK_ACE)
        return "?";

    return names[rank];
}

const char *card_suit_name(Suit suit)
{
    static const char *names[] = {
        "Diamonds", "Clubs", "Hearts", "Spades"
    };

    if (suit < SUIT_DIAMONDS || suit > SUIT_SPADES)
        return "?";

    return names[suit];
}
