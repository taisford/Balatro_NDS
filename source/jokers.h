
#ifndef JOKERS_H
#define JOKERS_H

#define MAX_JOKERS 5

typedef enum {
    JOKER_NONE,
    JOKER_BONUS_CHIPS
} JokerType;

typedef struct {
    JokerType type;
    int value;
} Joker;

void jokers_apply(Joker *jokers, int joker_count,
                  int *chips, int *mult);

#endif
