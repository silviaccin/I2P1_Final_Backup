#ifndef GAME_ASSERT_H
#define GAME_ASSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_native_dialog.h>

#define GAME_ASSERT(cond)                                                                 \
    do {                                                                                  \
        if (!(cond)) {                                                                    \
            fprintf(stderr, "[ASSERT FAILED] %s:%d - %s\n", __FILE__, __LINE__, #cond);   \
            al_show_native_message_box(NULL, "Assertion Failed",                          \
                "Game Assertion Failed", #cond, NULL, ALLEGRO_MESSAGEBOX_ERROR);          \
            exit(EXIT_FAILURE);                                                           \
        }                                                                                 \
    } while (0)

#endif


