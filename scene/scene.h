#ifndef SCENE_H
#define SCENE_H

#define MAX_ELEMENT 64

#include <stdbool.h>
#include "../global.h"

typedef struct _Scene Scene;
typedef struct Elements Elements; 

typedef void (*fptrUpdate)(Scene*);
typedef void (*fptrDraw)(Scene*);
typedef void (*fptrDestroy)(Scene*);

struct _Scene {
    int label;
    bool scene_end;
    void* pDerivedObj; //場景的具體資料結構

    fptrUpdate Update;
    fptrDraw Draw;
    fptrDestroy Destroy;

    Elements* ele_list[MAX_ELEMENT];
    int ele_num;
};

#endif

