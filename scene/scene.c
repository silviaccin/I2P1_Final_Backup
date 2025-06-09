#include "scene.h"
#include <stdlib.h>

void scene_update(Scene* self) {}
void scene_draw(Scene* self) {}
void scene_destroy(Scene* self) {
    free(self);
}

Scene* New_Scene(int label) {
    Scene* scene = (Scene*)malloc(sizeof(Scene));
    scene->label = label;
    scene->scene_end = false;
    scene->pDerivedObj = NULL;
    scene->Update = scene_update;
    scene->Draw = scene_draw;
    scene->Destroy = scene_destroy;
    return scene;
}

void _Register_elements(Scene* sc, Elements* ele) {
    if (sc->ele_num >= MAX_ELEMENT) return;

    sc->ele_list[sc->ele_num] = ele;
    sc->ele_num++;
}

