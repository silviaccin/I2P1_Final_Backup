#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "successscene.h"
#include "failscene.h"
#include "../global.h"
#include "../GAME_ASSERT.h"

static Scene* current_scene = NULL;

void scene_manager_init(Scene* s) {
    current_scene = s;
    GAME_ASSERT(current_scene);
    printf("[INFO] Initial scene started. Label: %d\n", s->label);
}

void scene_manager_update() {
    if (!current_scene) return;

    current_scene->Update(current_scene);

    if (current_scene->scene_end) {
        int next_label = current_scene->label;
        current_scene->Destroy(current_scene);
        printf("[INFO] Scene ended. Switching to label: %d\n", next_label);

        switch (next_label) {
            case 0:
                current_scene = New_MenuScene(); break;
            case 1:
                current_scene = New_GameScene(); break;
            case 2:
                current_scene = New_SuccessScene(); break;
            case 3:
                current_scene = New_FailScene(); break;
            default:
                printf("[ERROR] Unknown scene label: %d\n", next_label);
                game_running = false;
                current_scene = NULL;
                break;
        }

        GAME_ASSERT(current_scene);
    }
}

void scene_manager_draw() {
    if (current_scene) {
        current_scene->Draw(current_scene);
    }
}

void scene_manager_destroy() {
    if (current_scene) {
        current_scene->Destroy(current_scene);
        current_scene = NULL;
    }
}






