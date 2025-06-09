#include "global.h"
#include "GameWindow.h"
#include "scene/menu.h"
#include "scene/sceneManager.h"
#include "GAME_ASSERT.h"
#include <allegro5/allegro.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    game_init();

    Scene* start_scene = New_MenuScene();
    GAME_ASSERT(start_scene);

    scene_manager_init(start_scene);

    while (game_running) {
        scene_manager_update();
        scene_manager_draw();
    }

    scene_manager_destroy();
    game_destroy();
    return 0;

    printf("[INFO] Game exited cleanly.\n");
    return 0;
}
