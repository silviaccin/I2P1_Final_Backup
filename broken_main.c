#include "global.h"
#include "GameWindow.h"
#include "scene/menu.h"
#include "scene/sceneManager.h"

#include <allegro5/allegro.h>


int main(int argc, char **argv) {
    // 初始化 Allegro 核心（顯示錯誤用）
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    // 初始化遊戲視窗、鍵盤、音訊等
    game_init();

    // 啟動場景管理器，從 MenuScene 開始
    scene_manager_init(New_MenuScene());

    // 遊戲主迴圈
    while (game_running) {
        scene_manager_update();
        scene_manager_draw();
    }

    // 收尾處理
    scene_manager_destroy();
    game_destroy();

    return 0;
}
