#include "successscene.h"
#include "menu.h"
#include "../global.h"
#include "../GAME_ASSERT.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int selected; // 0: Menu, 1: Exit
    ALLEGRO_BITMAP* image;
    ALLEGRO_SAMPLE* sound;
    ALLEGRO_FONT* font;
} SuccessSceneData;

static void success_draw(Scene* sc) {
    SuccessSceneData* data = (SuccessSceneData*)(sc->pDerivedObj);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (data->image) {
        al_draw_scaled_bitmap(
            data->image,
            0, 0,
            al_get_bitmap_width(data->image),
            al_get_bitmap_height(data->image),
            0, 0,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            0);
    }

    if (data->font) {
        ALLEGRO_COLOR active = al_map_rgb(255, 255, 0);
        ALLEGRO_COLOR inactive = al_map_rgb(150, 150, 150);

        al_draw_text(data->font, data->selected == 0 ? active : inactive, 400, 450, ALLEGRO_ALIGN_CENTER, "Return to Menu");
        al_draw_text(data->font, data->selected == 1 ? active : inactive, 400, 500, ALLEGRO_ALIGN_CENTER, "Exit Game");
    }

    al_flip_display();
}

static void success_update(Scene* sc) {
    SuccessSceneData* data = (SuccessSceneData*)(sc->pDerivedObj);
    ALLEGRO_EVENT ev;

    while (al_get_next_event(event_queue, &ev)) {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
            (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
            game_running = false;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_UP:
                    data->selected = (data->selected + 1) % 2;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (data->selected == 0) {
                        sc->scene_end = true;
                        sc->label = 0; // 回主選單
                    } else {
                        game_running = false; // 結束遊戲
                    }
                    break;
            }
        }
    }
}

static void success_destroy(Scene* sc) {
    SuccessSceneData* data = (SuccessSceneData*)(sc->pDerivedObj);
    if (data->image) al_destroy_bitmap(data->image);
    if (data->sound) al_destroy_sample(data->sound);
    if (data->font) al_destroy_font(data->font);
    free(data);
    free(sc);
}

Scene* New_SuccessScene() {
    Scene* sc = (Scene*)malloc(sizeof(Scene));
    GAME_ASSERT(sc);

    SuccessSceneData* data = (SuccessSceneData*)malloc(sizeof(SuccessSceneData));
    GAME_ASSERT(data);
    data->selected = 0;

    data->image = al_load_bitmap("assets/images/success.png");
    if (!data->image) fprintf(stderr, "[WARN] Failed to load success.png\n");

    data->sound = al_load_sample("assets/sound/success.wav");
    if (data->sound) al_play_sample(data->sound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    else fprintf(stderr, "[WARN] Failed to load success.wav\n");

    data->font = al_load_ttf_font("assets/fonts/pirulen.ttf", 32, 0);
    GAME_ASSERT(data->font);

    sc->label = 2;
    sc->scene_end = false;
    sc->pDerivedObj = data;
    sc->Update = success_update;
    sc->Draw = success_draw;
    sc->Destroy = success_destroy;

    return sc;
}
