#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "scene.h"

void scene_manager_init(Scene* start_scene);
void scene_manager_update();
void scene_manager_draw();
void scene_manager_destroy();

#endif

