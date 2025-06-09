#include "gamescene.h"
#include "successscene.h"
#include "failscene.h"
#include "../global.h"
#include "../GAME_ASSERT.h"
#include "../element/player.h"
#include "../element/enemy.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENEMY 100
#define GOAL_SCORE 80
#define TIME_LIMIT 60
#define MAX_HP 5
#define COLLISION_MARGIN 10 

typedef struct {
    ALLEGRO_BITMAP* background;
    ALLEGRO_SAMPLE* bgm;
    ALLEGRO_SAMPLE_ID bgm_id;
    ALLEGRO_BITMAP* hp_images[MAX_HP + 1];
    Player* player;
    Enemy* enemies[MAX_ENEMY];
    int enemy_count;
    int hp;
    double last_enemy_time;
    double last_split_enemy_time;

    int score;
    double start_time;

    ALLEGRO_SAMPLE* hit_sound;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* enemy2_sprite;
} GameScene;

//碰撞判定
bool check_collision(Player* p, Enemy* e) {
    float px = p->x + COLLISION_MARGIN;
    float py = p->y + COLLISION_MARGIN;
    float pw = p->width - 2 * COLLISION_MARGIN;
    float ph = p->height - 2 * COLLISION_MARGIN;

    float ex = e->x + COLLISION_MARGIN;
    float ey = e->y + COLLISION_MARGIN;
    float ew = e->width - 2 * COLLISION_MARGIN;
    float eh = e->height - 2 * COLLISION_MARGIN;

    return (
        px < ex + ew &&
        px + pw > ex &&
        py < ey + eh &&
        py + ph > ey
    );
}

// 繪製遊戲場景
void game_scene_draw(Scene* sc) {
    GameScene* gs = (GameScene*)(sc->pDerivedObj);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_scaled_bitmap(
        gs->background,
        0, 0,
        al_get_bitmap_width(gs->background),
        al_get_bitmap_height(gs->background),
        0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    player_draw(gs->player);
    for (int i = 0; i < gs->enemy_count; i++) {
        if (gs->enemies[i] && gs->enemies[i]->alive)
            enemy_draw(gs->enemies[i]);
    }

    if (gs->font) {
        char score_text[32];
        sprintf(score_text, "Score: %d", gs->score);
        al_draw_text(gs->font, al_map_rgb(255, 255, 255), 10, 10, 0, score_text);

        char time_text[32];
        int remaining = (int)(TIME_LIMIT - (al_get_time() - gs->start_time));
        if (remaining < 0) remaining = 0;
        sprintf(time_text, "Time: %d", remaining);
        al_draw_text(gs->font, al_map_rgb(255, 255, 0), WINDOW_WIDTH / 2, 10, ALLEGRO_ALIGN_CENTER, time_text);
    }

    // 畫血條圖
    if (gs->hp > 0 && gs->hp <= MAX_HP && gs->hp_images[gs->hp]) {
        int original_w = al_get_bitmap_width(gs->hp_images[gs->hp]);
        int original_h = al_get_bitmap_height(gs->hp_images[gs->hp]);

        float scale = 0.2; // 縮放比例
        int draw_w = original_w * scale;
        int draw_h = original_h * scale;

        al_draw_scaled_bitmap(
            gs->hp_images[gs->hp],
            0, 0, original_w, original_h, // 原圖區域
            WINDOW_WIDTH - draw_w - 20, 10, // 畫面位置（右上角往內縮）
            draw_w, draw_h,                 // 縮放後的寬高
            0
        );

    }


    al_flip_display();
}

// 生成一般敵人
void spawn_enemy(GameScene* gs) {
    if (gs->enemy_count >= MAX_ENEMY) return;
    int direction = rand() % 4;
    Enemy* e = New_Enemy(ENEMY_TYPE_NORMAL, direction);
    gs->enemies[gs->enemy_count++] = e;
    gs->last_enemy_time = al_get_time();

    if (gs->enemy_count < MAX_ENEMY) {
        int dir = rand() % 4;
        Enemy* e = New_Enemy(ENEMY_TYPE_NORMAL, dir);
        gs->enemies[gs->enemy_count++] = e;
        gs->last_enemy_time = al_get_time();
    }

}

// 更新所有敵人狀態
// miss 分裂型敵人扣血
void update_enemies(Scene* sc, GameScene* gs) {
    int new_count = 0;

    for (int i = 0; i < gs->enemy_count; i++) {
        Enemy* e = gs->enemies[i];

        if (e && e->alive) {
            enemy_update(e);

            if (e->type == ENEMY_TYPE_SPLIT && e->entered_screen &&
                (e->x >= WINDOW_WIDTH ||
                 e->x + e->width <= 0 ||
                 e->y >= WINDOW_HEIGHT ||
                 e->y + e->height <= 0)) {

                e->alive = false;

                printf("[DEBUG] Missed enemy! type=%d, x=%.1f, y=%.1f, w=%.1f, h=%.1f, entered=%d, HP(before)=%d\n",
                       e->type, e->x, e->y, e->width, e->height, e->entered_screen, gs->hp);

                if (gs->hp > 0) {
                    gs->hp--;
                }

                printf("[DEBUG] HP(after)=%d\n", gs->hp);

                if (gs->hp <= 0) {
                    gs->hp = 0;
                    gs->score = 0;
                    sc->scene_end = true;
                    sc->label = 3; // FailScene
                    return;
                }
            }

            gs->enemies[new_count++] = e;
        } else {
            if (e) {
                enemy_destroy(e);
                gs->enemies[i] = NULL;
            }
        }
    }

    gs->enemy_count = new_count;
}

// 遊戲主邏輯更新
void game_scene_update(Scene* sc) {
    GameScene* gs = (GameScene*)(sc->pDerivedObj);
    ALLEGRO_EVENT ev;

    while (al_get_next_event(event_queue, &ev)) {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            game_running = false;
        }
    }

    player_update(gs->player);
    update_enemies(sc, gs);

    double now = al_get_time();
    double elapsed = now - gs->start_time;
    double interval = 1.0;

    //20 秒後產生分裂型敵人
    if (elapsed >= 20.0 && now - gs->last_split_enemy_time >= 1.5) {
        for (int i = 0; i < 2; i++) {
            int dir = rand() % 4;
            Enemy* e = spawn_split_enemy(dir);
            if (gs->enemy_count < MAX_ENEMY) {
                gs->enemies[gs->enemy_count++] = e;
            }
        }
        gs->last_split_enemy_time = now;
    }


    //分裂型則分裂出兩隻enemy3 or 加分
    if (now - gs->last_enemy_time >= interval) {
        if (elapsed >= 20.0 && rand() % 5 == 0) {
            int dir = rand() % 4;
            Enemy* e = spawn_split_enemy(dir);
            gs->enemies[gs->enemy_count++] = e;
        } else {
            spawn_enemy(gs); //原本的敵人
        }
        gs->last_enemy_time = now;
    }

    //檢查每個活著的敵人是否與玩家有碰撞
    for (int i = 0; i < gs->enemy_count; i++) {
        Enemy* e = gs->enemies[i];
        if (e && e->alive && check_collision(gs->player, e)) {
            if (e->type == ENEMY_TYPE_SPLIT) {
                int dir1 = rand() % 4;
                int dir2 = rand() % 4;
                Enemy* left = New_Enemy(ENEMY_TYPE_NORMAL, dir1);
                left->x = e->x - 10;
                left->y = e->y;
                left->sprite = al_load_bitmap("assets/images/enemy3.png");
                left->entered_screen = false;  //一定要重設，不然可能被誤判

                Enemy* right = New_Enemy(ENEMY_TYPE_NORMAL, dir2);
                right->x = e->x + 10;
                right->y = e->y;
                right->sprite = al_load_bitmap("assets/images/enemy3.png");
                right->entered_screen = false;


                left->x = e->x - 10;
                left->y = e->y;
                right->x = e->x + 10;
                right->y = e->y;

                gs->enemies[gs->enemy_count++] = left;
                gs->enemies[gs->enemy_count++] = right;

                e->alive = false;
                if (gs->hit_sound)
                    al_play_sample(gs->hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            } else {
                gs->score++;
                e->alive = false;
                if (gs->hit_sound)
                    al_play_sample(gs->hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                printf("[INFO] Score: %d\n", gs->score);
            }
        }

    }

    if (gs->score >= GOAL_SCORE && elapsed <= TIME_LIMIT) {
        sc->scene_end = true;
        sc->label = 2; // SuccessScene
    } else if (elapsed > TIME_LIMIT && gs->score < GOAL_SCORE) {
        sc->scene_end = true;
        sc->label = 3; // FailScene
    }
}

//釋放資源
void game_scene_destroy(Scene* sc) {
    GameScene* gs = (GameScene*)(sc->pDerivedObj);

    al_stop_sample(&gs->bgm_id);  // 停止播放
    if (gs->bgm){
        al_destroy_sample(gs->bgm);
        gs->bgm = NULL;
    }
    
    if (gs->background)
        al_destroy_bitmap(gs->background);
    if (gs->player)
        player_destroy(gs->player);
    if (gs->hit_sound)
        al_destroy_sample(gs->hit_sound);
    if (gs->font)
        al_destroy_font(gs->font);

    for (int i = 0; i <= MAX_HP; i++) {
        if (gs->hp_images[i]) {
            al_destroy_bitmap(gs->hp_images[i]);
        }
    }      

    for (int i = 0; i < gs->enemy_count; i++) {
        if (gs->enemies[i])
            enemy_destroy(gs->enemies[i]);
    }

    free(gs);
    free(sc);
}

// 初始化
Scene* New_GameScene() {
    Scene* sc = (Scene*)malloc(sizeof(Scene));
    GAME_ASSERT(sc);

    GameScene* gs = (GameScene*)malloc(sizeof(GameScene));
    GAME_ASSERT(gs);

    gs->background = al_load_bitmap("assets/images/background.png");
    GAME_ASSERT(gs->background);

    gs->player = New_Player(100, 100);
    GAME_ASSERT(gs->player);

    gs->hit_sound = al_load_sample("assets/sound/hit.wav");
    GAME_ASSERT(gs->hit_sound);

    gs->font = al_load_ttf_font("assets/fonts/pirulen.ttf", 24, 0);
    GAME_ASSERT(gs->font);

    gs->enemy_count = 0;
    gs->last_enemy_time = al_get_time();

    gs->score = 0;
    gs->start_time = al_get_time();

    gs->last_split_enemy_time = al_get_time(); // 初始為遊戲開始時間


    gs->enemy2_sprite = al_load_bitmap("assets/images/enemy2.png");
    GAME_ASSERT(gs->enemy2_sprite);

    gs->score = 0;
    gs->start_time = al_get_time();

    gs->hp = MAX_HP;

    gs->bgm = al_load_sample("assets/sound/bgm.wav");  // ← 請替換為你的檔名與路徑
    GAME_ASSERT(gs->bgm);

    al_play_sample(gs->bgm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &gs->bgm_id);


    //血條
    for (int i = 1; i <= MAX_HP; i++) {
        char path[64];
        sprintf(path, "assets/images/hp%d.png", i);
        gs->hp_images[i] = al_load_bitmap(path);
        GAME_ASSERT(gs->hp_images[i]);
    }
    gs->hp_images[0] = NULL;


    for (int i = 1; i <= MAX_HP; i++) {
        GAME_ASSERT(gs->hp_images[i]);
    }

    sc->label = 1;
    sc->scene_end = false;
    sc->pDerivedObj = gs;
    sc->Update = game_scene_update;
    sc->Draw = game_scene_draw;
    sc->Destroy = game_scene_destroy;

    return sc;
}