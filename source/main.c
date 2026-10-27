
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cards.h"
#include "poker.h"
#include "scoring.h"

typedef enum {
    SCREEN_TITLE,
    SCREEN_TEST
} GameScreen;

static Deck deck;
static Card hand[HAND_SIZE];
static int hand_count = 0;
static int selected[HAND_SIZE];

static Card discard_pile[DECK_SIZE];
static int discard_count = 0;

static int hands_remaining = 4;
static int discards_remaining = 3;
static int game_over = 0;

static int cursor = 0;
static int selected_count = 0;

static PokerResult result;
static ScoreResult score;
static int show_result = 0;

static void refill_hand(void);

static void start_test_game(void)
{
    deck_init(&deck);
    deck_shuffle(&deck);

    hand_count = 0;
    discard_count = 0;

    hands_remaining = 4;
    discards_remaining = 3;
    game_over = 0;

    selected_count = 0;
    cursor = 0;
    show_result = 0;

    for (int i = 0; i < HAND_SIZE; i++) {
        selected[i] = 0;
    }

    refill_hand();
}


static void discard_card(Card card)
{
    if (discard_count < DECK_SIZE) {
        discard_pile[discard_count++] = card;
    }
}

static int draw_card(Card *card)
{
    if (deck_draw(&deck, card)) {
        return 1;
    }

    if (discard_count == 0) {
        return 0;
    }

    /* Copiar el descarte al mazo. */
    for (int i = 0; i < discard_count; i++) {
        deck.cards[i] = discard_pile[i];
    }

    /* El mazo reciclado solo contiene esas cartas. */
    deck.card_count = discard_count;
    deck.next_card = 0;
    deck_shuffle(&deck);

    discard_count = 0;

    return deck_draw(&deck, card);
}



static void refill_hand(void)
{
    while (hand_count < HAND_SIZE) {
        if (!draw_card(&hand[hand_count])) {
            break;
        }

        hand_count++;
    }
}


static void remove_selected_and_draw(void)
{
    int write_index = 0;

    for (int i = 0; i < hand_count; i++) {
        if (selected[i]) {
            discard_card(hand[i]);
        } else {
            hand[write_index++] = hand[i];
        }
    }

    hand_count = write_index;

    for (int i = 0; i < HAND_SIZE; i++) {
        selected[i] = 0;
    }

    selected_count = 0;
    cursor = 0;

    refill_hand();
}



static void evaluate_selection(void)
{
    Card chosen[5];
    int count = 0;

    for (int i = 0; i < hand_count; i++) {
        if (selected[i] && count < 5) {
            chosen[count++] = hand[i];
        }
    }

    if (count > 0) {
        result = poker_evaluate(chosen, count);
        score = score_hand(chosen, count, &result);
        show_result = 1;
    }
}


static void draw_screen(GameScreen screen)
{
    consoleClear();

    printf("BALATRO DS\n");
    printf("========================\n\n");

    if (screen == SCREEN_TITLE) {
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

    if (game_over) {
        printf("FIN DE LA PARTIDA\n");
        printf("Pulsa B para volver al titulo\n");
        return;
    }


    for (int i = 0; i < hand_count; i++) {
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

    
    if (show_result) {
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
    consoleDemoInit();
    srand((unsigned int)time(NULL));

    GameScreen screen = SCREEN_TITLE;
    draw_screen(screen);

    while (1) {
        swiWaitForVBlank();
        scanKeys();

        const int keys = keysDown();
        int redraw = 0;

        if (keys & KEY_START)
            break;

        if (screen == SCREEN_TITLE && (keys & KEY_A)) {
            start_test_game();
            screen = SCREEN_TEST;
            redraw = 1;
        } else if (screen == SCREEN_TEST) {
            if (keys & KEY_B) {
                screen = SCREEN_TITLE;
                redraw = 1;
            }

            if (keys & KEY_LEFT) {
                if (cursor > 0)
                    cursor--;
                redraw = 1;
            }

            if (keys & KEY_RIGHT) {
    		if (cursor < hand_count - 1)
        	    cursor++;
    		redraw = 1;
	    }

            if (keys & KEY_A) {
    		if (hand_count > 0) {
        	    if (selected[cursor]) {
            		selected[cursor] = 0;
            	    	selected_count--;
        	    } else if (selected_count < 5) {
            		selected[cursor] = 1;
            	    	selected_count++;
        	    }
        	show_result = 0;
        	redraw = 1;
    	        }
	    }

            
            if (keys & KEY_Y) {
                if (!game_over && hands_remaining > 0 &&
                    selected_count > 0) {

                        evaluate_selection();

                        hands_remaining--;
                        remove_selected_and_draw();

                        if (hands_remaining == 0 &&
                            discards_remaining == 0) {
                            game_over = 1;
                        }
                    redraw = 1;
                }
            }

            if (keys & KEY_X) {
                if (!game_over && discards_remaining > 0 &&
                selected_count > 0) {

                    discards_remaining--;
                    show_result = 0;
                    remove_selected_and_draw();

                    if (hands_remaining == 0 &&
                    discards_remaining == 0) {
                        game_over = 1;
                    }
                redraw = 1;
                }
            }


                    if (redraw) {
                draw_screen(screen);
            }
        }
    }

    return 0;
}

