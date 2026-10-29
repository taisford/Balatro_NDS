
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cards.h"
#include "poker.h"
#include "scoring.h"
#include "economy.h"

typedef enum
{
    SCREEN_TITLE,
    SCREEN_TEST
} GameScreen;

typedef enum
{
    BLIND_SMALL,
    BLIND_BIG,
    BLIND_BOSS
} BlindType;

typedef struct
{
    int round;
    BlindType blind;
    int target_score;
    int current_score;
} RoundState;

static Deck deck;
static Card hand[HAND_SIZE];
static int hand_count = 0;
static int selected[HAND_SIZE];
static RoundState round_state;
static int money = 4;
static int last_reward = 0;
static PrintConsole topScreen;
static PrintConsole bottomScreen;

static Card discard_pile[DECK_SIZE];
static int discard_count = 0;

static int hands_remaining = 4;
static int discards_remaining = 3;
static int game_over = 0;

static int cursor = 0;
static int selected_count = 0;
static int sort_mode = 0;

static PokerResult result;
static ScoreResult score;
static int show_result = 0;

static void refill_hand(void);

static void reset_blind_deck(void)
{
    deck_init(&deck);
    deck_shuffle(&deck);

    hand_count = 0;
    discard_count = 0;
    selected_count = 0;
    cursor = 0;

    for (int i = 0; i < HAND_SIZE; i++)
    {
        selected[i] = 0;
    }

    refill_hand();
}

static void start_test_game(void)
{
    money = 4;
    last_reward = 0;
    sort_mode = 0;

    deck_init(&deck);
    deck_shuffle(&deck);

    hand_count = 0;
    discard_count = 0;

    hands_remaining = 4;
    discards_remaining = 3;
    game_over = 0;
    round_state.round = 1;
    round_state.blind = BLIND_SMALL;
    round_state.target_score = 300;
    round_state.current_score = 0;

    selected_count = 0;
    cursor = 0;
    show_result = 0;

    for (int i = 0; i < HAND_SIZE; i++)
    {
        selected[i] = 0;
    }

    refill_hand();
}

static int get_blind_base(int round)
{
    static const int base_scores[] = {
        300, 800, 2000, 5000,
        11000, 20000, 35000, 50000};

    if (round < 1 || round > 8)
    {
        return 50000;
    }

    return base_scores[round - 1];
}

static void update_blind_target(void)
{
    int base = get_blind_base(round_state.round);

    switch (round_state.blind)
    {
    case BLIND_SMALL:
        round_state.target_score = base;
        break;

    case BLIND_BIG:
        round_state.target_score = base * 3 / 2;
        break;

    case BLIND_BOSS:
        round_state.target_score = base * 2;
        break;
    }
}

static void advance_blind(void)
{
    int reward = economy_blind_reward(round_state.blind) + economy_remaining_hands_bonus(hands_remaining);

    money += reward;
    last_reward = reward;

    switch (round_state.blind)
    {
    case BLIND_SMALL:
        round_state.blind = BLIND_BIG;
        break;

    case BLIND_BIG:
        round_state.blind = BLIND_BOSS;
        break;

    case BLIND_BOSS:
        round_state.blind = BLIND_SMALL;
        round_state.round++;
        break;
    }

    round_state.current_score = 0;
    update_blind_target();

    hands_remaining = 4;
    discards_remaining = 3;
    game_over = 0;
    show_result = 0;

    reset_blind_deck();
}

static void discard_card(Card card)
{
    if (discard_count < DECK_SIZE)
    {
        discard_pile[discard_count++] = card;
    }
}

static int draw_card(Card *card)
{
    if (deck_draw(&deck, card))
    {
        return 1;
    }

    if (discard_count == 0)
    {
        return 0;
    }

    for (int i = 0; i < discard_count; i++)
    {
        deck.cards[i] = discard_pile[i];
    }

    deck.card_count = discard_count;
    deck.next_card = 0;
    deck_shuffle(&deck);

    discard_count = 0;

    return deck_draw(&deck, card);
}

static void refill_hand(void)
{
    while (hand_count < HAND_SIZE)
    {
        if (!draw_card(&hand[hand_count]))
        {
            break;
        }

        hand_count++;
    }
}

static void remove_selected_and_draw(void)
{
    int write_index = 0;

    for (int i = 0; i < hand_count; i++)
    {
        if (selected[i])
        {
            discard_card(hand[i]);
        }
        else
        {
            hand[write_index++] = hand[i];
        }
    }

    hand_count = write_index;

    for (int i = 0; i < HAND_SIZE; i++)
    {
        selected[i] = 0;
    }

    selected_count = 0;
    cursor = 0;

    refill_hand();
}

static int card_value_desc(const void *a, const void *b)
{
    const Card *ca = (const Card *)a;
    const Card *cb = (const Card *)b;

    return (int)cb->rank - (int)ca->rank;
}

static int card_suit_value(const void *a, const void *b)
{
    const Card *ca = (const Card *)a;
    const Card *cb = (const Card *)b;

    if (ca->suit != cb->suit)
    {
        return (int)ca->suit - (int)cb->suit;
    }

    return (int)cb->rank - (int)ca->rank;
}

static void sort_hand(void)
{
    if (hand_count < 2)
    {
        return;
    }

    if (sort_mode == 0)
    {
        qsort(hand, hand_count, sizeof(Card), card_suit_value);
        sort_mode = 1;
    }
    else
    {
        qsort(hand, hand_count, sizeof(Card), card_value_desc);
        sort_mode = 0;
    }

    cursor = 0;
}

static void evaluate_selection(void)
{
    Card chosen[5];
    int count = 0;

    for (int i = 0; i < hand_count; i++)
    {
        if (selected[i] && count < 5)
        {
            chosen[count++] = hand[i];
        }
    }

    if (count > 0)
    {
        result = poker_evaluate(chosen, count);
        score = score_hand(chosen, count, &result);
        show_result = 1;
    }
}

static void draw_screen(GameScreen screen)
{
    consoleSelect(&topScreen);
    consoleClear();

    printf("BALATRO DS\n");
    printf("========================\n\n");

    if (screen == SCREEN_TITLE)
    {
        printf("Prototipo de desarrollo\n\n");
        printf("A - Iniciar prueba\n");
        printf("START - Salir\n");
        return;
    }

    printf("PARTIDA DE PRUEBA\n");
    printf("Manos: %d | Descartes: %d\n",
           hands_remaining, discards_remaining);
    printf("Mazo: %d | Descarte: %d\n",
           deck.card_count - deck.next_card, discard_count);
    printf("Seleccion: %d/5\n\n", selected_count);
    printf("RONDA: %d\n", round_state.round);

    switch (round_state.blind)
    {
    case BLIND_SMALL:
        printf("Ciega: PEQUENA\n");
        break;

    case BLIND_BIG:
        printf("Ciega: GRANDE\n");
        break;

    case BLIND_BOSS:
        printf("Ciega: JEFE\n");
        break;
    }

    printf("Objetivo: %d\n", round_state.target_score);
    printf("Puntuacion: %d\n\n", round_state.current_score);
    printf("Dinero: $%d\n", money);
    printf("Ultima recompensa: $%d\n\n", last_reward);

    if (game_over)
    {
        printf("FIN DE LA PARTIDA\n");
        printf("Pulsa B para volver al titulo\n");
        return;
    }

    consoleSelect(&bottomScreen);
    consoleClear();
    for (int i = 0; i < hand_count; i++)
    {
        printf("%c %c %s de %s\n",
               i == cursor ? '>' : ' ',
               selected[i] ? '*' : ' ',
               card_rank_name(hand[i].rank),
               card_suit_name(hand[i].suit));
    }

    printf("\nCruceta: mover");
    printf("\nA: seleccionar");
    printf("\nY: evaluar");
    printf("\nX: descartar seleccionadas");
    printf("\nB: volver");

    if (show_result)
    {
        printf("\n\nJugada: %s", poker_hand_name(result.type));
        printf("\nFichas base: %d", score.base_chips);
        printf("\nFichas cartas: %d", score.card_chips);
        printf("\nFichas totales: %d", score.total_chips);
        printf("\nMultiplicador: x%d", score.total_mult);
        printf("\nPUNTUACION: %d", score.score);
    }
}

int main(void)
{
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    consoleInit(&topScreen, 0, BgType_Text4bpp,
                BgSize_T_256x256, 31, 0,
                true, true);

    consoleInit(&bottomScreen, 0, BgType_Text4bpp,
                BgSize_T_256x256, 31, 0,
                false, true);

    consoleSelect(&bottomScreen);

    srand((unsigned int)time(NULL));

    GameScreen screen = SCREEN_TITLE;
    draw_screen(screen);

    while (1)
    {
        swiWaitForVBlank();
        scanKeys();

        const int keys = keysDown();
        int redraw = 0;

        if (keys & KEY_START)
            break;

        if (screen == SCREEN_TITLE && (keys & KEY_A))
        {
            start_test_game();
            screen = SCREEN_TEST;
            redraw = 1;
        }
        else if (screen == SCREEN_TEST)
        {
            if (keys & KEY_B)
            {
                for (int i = 0; i < HAND_SIZE; i++)
                {
                    selected[i] = 0;
                }

                selected_count = 0;
                redraw = 1;
            }

            if (keys & KEY_LEFT)
            {
                if (cursor > 0)
                    cursor--;
                redraw = 1;
            }

            if (keys & KEY_RIGHT)
            {
                if (cursor < hand_count - 1)
                    cursor++;
                redraw = 1;
            }

            if (keys & KEY_R)
            {
                sort_hand();
                redraw = 1;
            }

            if (keys & KEY_A)
            {
                if (hand_count > 0)
                {
                    if (selected[cursor])
                    {
                        selected[cursor] = 0;
                        selected_count--;
                    }
                    else if (selected_count < 5)
                    {
                        selected[cursor] = 1;
                        selected_count++;
                    }
                    show_result = 0;
                    redraw = 1;
                }
            }

            if (keys & KEY_Y)
            {
                if (!game_over && hands_remaining > 0 &&
                    selected_count > 0)
                {

                    evaluate_selection();

                    if (show_result)
                    {
                        round_state.current_score += score.score;
                    }

                    hands_remaining--;
                    remove_selected_and_draw();

                    if (round_state.current_score >= round_state.target_score)
                    {
                        advance_blind();
                    }
                    else if (hands_remaining == 0)
                    {
                        game_over = 1;
                    }

                    if (hands_remaining == 0 &&
                        discards_remaining == 0)
                    {
                        game_over = 1;
                    }
                    redraw = 1;
                }
            }

            if (keys & KEY_X)
            {
                if (!game_over && discards_remaining > 0 &&
                    selected_count > 0)
                {

                    discards_remaining--;
                    show_result = 0;
                    remove_selected_and_draw();

                    if (hands_remaining == 0 &&
                        discards_remaining == 0)
                    {
                        game_over = 1;
                    }
                    redraw = 1;
                }
            }

            if (redraw)
            {
                draw_screen(screen);
            }
        }
    }

    return 0;
}
