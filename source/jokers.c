
#include "jokers.h"

void jokers_apply(Joker *jokers, int joker_count,
                  int *chips, int *mult)
{
    if (jokers == 0 || chips == 0 || mult == 0)
        return;

    for (int i = 0; i < joker_count; i++)
    {
        switch (jokers[i].type)
        {
        case JOKER_BONUS_CHIPS:
            *chips += jokers[i].value;
            break;

        case JOKER_NONE:
        default:
            break;
        }
    }
}
