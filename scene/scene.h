#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include "../global.h"

typedef struct _Scene Scene;

typedef void (*fptrUpdate)(Scene*);
typedef void (*fptrDraw)(Scene*);
typedef void (*fptrDestroy)(Scene*);

struct _Scene {
    int label;
    bool scene_end;
    void* pDerivedObj;

    fptrUpdate Update;
    fptrDraw Draw;
    fptrDestroy Destroy;
};

#endif

