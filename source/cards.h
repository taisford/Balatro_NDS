#ifndef CARDS_H
#define CARDS_H

#include <stdint.h>

#define DECK_SIZE 52
#define HAND_SIZE 8

typedef enum {
    SUIT_DIAMONDS,
    SUIT_CLUBS,
    SUIT_HEARTS,
    SUIT_SPADES
} Suit;

typedef enum {
    RANK_2 = 2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_ACE
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
    uint8_t id;
    uint8_t nominal;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    int next_card;
} Deck;

void deck_init(Deck *deck);
void deck_shuffle(Deck *deck);
int deck_draw(Deck *deck, Card *card);

const char *card_rank_name(Rank rank);
const char *card_suit_name(Suit suit);

#endif
