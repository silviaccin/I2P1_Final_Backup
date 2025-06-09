#include "enemy.h"
#include "../GAME_ASSERT.h"
#include "../global.h"
#include <allegro5/allegro_image.h>
#include <stdlib.h>

Enemy* New_Enemy(EnemyType type, int direction) {
    Enemy* e = (Enemy*)malloc(sizeof(Enemy));
    GAME_ASSERT(e);

    e->type = type;
    e->alive = true;
    e->speed = 2.0;
    e->entered_screen = false;  //初始化為尚未進入畫面
    e->sprite = al_load_bitmap("assets/images/enemy3.png");
    GAME_ASSERT(e->sprite);

    float scale;

    switch (type) {
    case ENEMY_TYPE_SPLIT:
        e->sprite = al_load_bitmap("assets/images/enemy2.png");
        GAME_ASSERT(e->sprite);
        scale = 0.3;  //比普通敵人大
        break;
    case ENEMY_TYPE_NORMAL:
    default:
        e->sprite = al_load_bitmap("assets/images/enemy1.png");
        GAME_ASSERT(e->sprite);
        scale = 0.2;
        break;
    }

    e->width = al_get_bitmap_width(e->sprite) * scale;
    e->height = al_get_bitmap_height(e->sprite) * scale;

    int w = e->width;
    int h = e->height;

    // 根據方向設定初始座標和移動向量
    switch (direction) {
        case 0: // 從上方進場
            e->x = rand() % (WINDOW_WIDTH - w);
            e->y = -h;
            e->dx = 0;
            e->dy = 1;
            break;
        case 1: // 從下方進場
            e->x = rand() % (WINDOW_WIDTH - w);
            e->y = WINDOW_HEIGHT;
            e->dx = 0;
            e->dy = -1;
            break;
        case 2: // 從左邊進場
            e->x = -w;
            e->y = rand() % (WINDOW_HEIGHT - h);
            e->dx = 1;
            e->dy = 0;
            break;
        case 3: // 從右邊進場
            e->x = WINDOW_WIDTH;
            e->y = rand() % (WINDOW_HEIGHT - h);
            e->dx = -1;
            e->dy = 0;
            break;
    }

    return e;
}

Enemy* spawn_split_enemy(int direction) {
    Enemy* e = New_Enemy(ENEMY_TYPE_SPLIT, direction);
    e->dx = 0;
    e->dy = 1.5;
    e->sprite = al_load_bitmap("assets/images/enemy2.png");
    if (!e->sprite) {
    printf("[ERROR] Failed to load enemy2.png!\n");
    }
    return e;
}

void enemy_update(Enemy* e) {
    if (!e->alive) return;

    e->x += e->dx * e->speed;
    e->y += e->dy * e->speed;

    // 一旦任何部分進入畫面，就標記為 true
   float cx = e->x + e->width / 2;
   float cy = e->y + e->height / 2;

    if (!e->entered_screen &&
        cx >= 0 && cx <= WINDOW_WIDTH &&
        cy >= 0 && cy <= WINDOW_HEIGHT) {
        e->entered_screen = true;
    }

    // 超出畫面範圍標記為死亡
    if (e->x < -e->width || e->x > WINDOW_WIDTH + e->width ||
        e->y < -e->height || e->y > WINDOW_HEIGHT + e->height) {
        e->alive = false;
    }
}


void enemy_draw(Enemy* e) {
    if (!e->alive || !e->sprite) return;

    al_draw_scaled_bitmap(
        e->sprite,
        0, 0,
        al_get_bitmap_width(e->sprite),     // 原圖寬
        al_get_bitmap_height(e->sprite),    // 原圖高
        e->x, e->y,
        e->width, e->height,                // 用 e->width/height
        0
    );
}

void enemy_destroy(Enemy* e) {
    if (e->sprite)
        al_destroy_bitmap(e->sprite);
    free(e);
}




