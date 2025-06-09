// scene/menu.c
#include "menu.h"
#include "../global.h"
#include "../GAME_ASSERT.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int selected; // 0: Start, 1: Exit
    ALLEGRO_FONT* font;
    ALLEGRO_SAMPLE* bgm; 
    ALLEGRO_BITMAP* background;
} Menu;

void menu_draw(Scene* sc) {
    Menu* menu = (Menu*)(sc->pDerivedObj);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_COLOR active = al_map_rgb(255, 255, 0);
    ALLEGRO_COLOR inactive = al_map_rgb(150, 150, 150);

    if (menu->background) {
        al_draw_scaled_bitmap(
        menu->background,
        0, 0,
        al_get_bitmap_width(menu->background),
        al_get_bitmap_height(menu->background),
        0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );
    } else {
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_draw_text(menu->font, menu->selected == 0 ? active : inactive, 400, 250, ALLEGRO_ALIGN_CENTER, "START");
    al_draw_text(menu->font, menu->selected == 1 ? active : inactive, 400, 300, ALLEGRO_ALIGN_CENTER, "EXIT");

    al_flip_display();
}

void menu_update(Scene* sc) {
    Menu* menu = (Menu*)(sc->pDerivedObj);

    ALLEGRO_EVENT ev;
    while (al_get_next_event(event_queue, &ev)) {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            game_running = false;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_UP:
                    menu->selected = (menu->selected + 1) % 2;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (menu->selected == 0) {
                        sc->scene_end = true; // 切到 GameScene
                        sc->label = 1;
                    } else if (menu->selected == 1) {
                        game_running = false;
                    }
                    break;
            }
        }
    }
}

void menu_destroy(Scene* sc) {
    Menu* menu = (Menu*)(sc->pDerivedObj);
    if (menu->font)
        al_destroy_font(menu->font);
    if (menu->bgm)
        al_destroy_sample(menu->bgm);
    if (menu->background) {
    al_destroy_bitmap(menu->background);
    }
    free(menu);
    free(sc);
}

Scene* New_MenuScene() {
    Scene* sc = (Scene*)malloc(sizeof(Scene));
    GAME_ASSERT(sc);
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    GAME_ASSERT(menu);

    menu->selected = 0;

    menu->font = al_load_ttf_font("assets/fonts/pirulen.ttf", 36, 0);
    GAME_ASSERT(menu->font);

    menu->background = al_load_bitmap("assets/images/menu_bg.png");
    if (!menu->background) {
        fprintf(stderr, "[WARN] Menu background not found: assets/images/menu_bg.png\n");
    }

    menu->bgm = al_load_sample("assets/sound/menu.wav");
    if (menu->bgm) {
        al_play_sample(menu->bgm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    } else {
        fprintf(stderr, "[WARN] Menu BGM not found: assets/sound/menu.wav\n");
    }

    sc->scene_end = false;
    sc->pDerivedObj = menu;
    sc->Update = menu_update;
    sc->Draw = menu_draw;
    sc->Destroy = menu_destroy;
    sc->label = 0;

    return sc;
}



