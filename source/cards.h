#ifndef CARDS_H
#define CARDS_H

#include <stdint.h>

#define DECK_SIZE 52
#define MAX_DECK_SIZE 104
#define HAND_SIZE 8

typedef enum
{
    SUIT_DIAMONDS,
    SUIT_CLUBS,
    SUIT_HEARTS,
    SUIT_SPADES
} Suit;

typedef enum
{
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

typedef enum
{
    ENHANCEMENT_NONE,
    ENHANCEMENT_BONUS,
    ENHANCEMENT_MULT,
    ENHANCEMENT_WILD,
    ENHANCEMENT_GLASS,
    ENHANCEMENT_STEEL,
    ENHANCEMENT_STONE,
    ENHANCEMENT_GOLD,
    ENHANCEMENT_LUCKY
} CardEnhancement;

typedef enum
{
    EDITION_NONE,
    EDITION_FOIL,
    EDITION_HOLOGRAPHIC,
    EDITION_POLYCHROME,
    EDITION_NEGATIVE
} CardEdition;

typedef enum
{
    SEAL_NONE,
    SEAL_GOLD,
    SEAL_RED,
    SEAL_BLUE,
    SEAL_PURPLE
} CardSeal;

typedef struct
{
    Suit suit;
    Rank rank;
    uint8_t id;
    uint8_t nominal;

    CardEnhancement enhancement;
    CardEdition edition;
    CardSeal seal;

    uint8_t debuffed;
} Card;

typedef struct
{
    Card cards[MAX_DECK_SIZE];
    int next_card;
    int card_count;
} Deck;

void deck_init(Deck *deck);
void deck_shuffle(Deck *deck);
int deck_draw(Deck *deck, Card *card);

const char *card_rank_name(Rank rank);
const char *card_suit_name(Suit suit);

#endif
