#include "player.h"
#include "../GAME_ASSERT.h"
#include "../global.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

Player* New_Player(float startX, float startY) {
    Player* p = (Player*)malloc(sizeof(Player));
    GAME_ASSERT(p);

    p->x = startX;
    p->y = startY;
    p->speed = 5;

    p->sprite = al_load_bitmap("assets/images/player.png");
    GAME_ASSERT(p->sprite);

    float scale = 0.1;
    p->width = al_get_bitmap_width(p->sprite) * scale;
    p->height = al_get_bitmap_height(p->sprite) * scale;

    return p;
}


void player_update(Player* p) {
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    //player 座標更新
    if (al_key_down(&keyState, ALLEGRO_KEY_UP))
        p->y -= p->speed;
    if (al_key_down(&keyState, ALLEGRO_KEY_DOWN))
        p->y += p->speed;
    if (al_key_down(&keyState, ALLEGRO_KEY_LEFT))
        p->x -= p->speed;
    if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
        p->x += p->speed;

    //邊界限制
    if (p->x < 0)
        p->x = 0;
    if (p->y < 0)
        p->y = 0;
    if (p->x + p->width > WINDOW_WIDTH)
        p->x = WINDOW_WIDTH - p->width;
    if (p->y + p->height > WINDOW_HEIGHT)
        p->y = WINDOW_HEIGHT - p->height;
}

void player_draw(Player* p) {
    float draw_width = al_get_bitmap_width(p->sprite) * 0.3;  //
    float draw_height = al_get_bitmap_height(p->sprite) * 0.3;

    al_draw_scaled_bitmap(
        p->sprite,
        0, 0, 
        al_get_bitmap_width(p->sprite),
        al_get_bitmap_height(p->sprite),
        p->x, p->y,   
        p->width,       
        p->height,                     
        0
    );
}

void player_destroy(Player* p) {
    if (p->sprite)
        al_destroy_bitmap(p->sprite);
    free(p);
}



