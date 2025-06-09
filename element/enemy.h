#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <stdbool.h>

typedef enum {
    ENEMY_TYPE_NORMAL = 1,
    ENEMY_TYPE_SPLIT = 2  // 新增：會分裂的敵人
} EnemyType;

typedef struct {
    float x, y;
    float dx, dy;
    float speed;
    float width, height;
    EnemyType type;
    bool alive;
    bool entered_screen;
    ALLEGRO_BITMAP* sprite;
} Enemy;

Enemy* New_Enemy(EnemyType type, int direction);
Enemy* spawn_split_enemy(int direction);
void enemy_update(Enemy* e);
void enemy_draw(Enemy* e);
void enemy_destroy(Enemy* e);

#endif
