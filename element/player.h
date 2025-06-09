#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

typedef struct {
    float x, y;
    float speed;
    float width, height;
    ALLEGRO_BITMAP* sprite;
} Player;

Player* New_Player(float startX, float startY);
void player_update(Player* p);
void player_draw(Player* p);
void player_destroy(Player* p);

#endif

