
#include "economy.h"

int economy_blind_reward(int blind_type)
{
    switch (blind_type) {
        case 0: /* Ciega pequeña */
            return 3;

        case 1: /* Ciega grande */
            return 4;

        case 2: /* Ciega jefe */
            return 5;

        default:
            return 0;
    }
}

int economy_remaining_hands_bonus(int hands_remaining)
{
    if (hands_remaining < 0) {
        return 0;
    }

    return hands_remaining;
}
