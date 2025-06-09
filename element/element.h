#ifndef ELEMENT_H
#define ELEMENT_H

// 基本元素結構：可更新、繪製、銷毀、互動
typedef struct _Elements {
    float x, y;     // 位置
    float w, h;     // 寬高
    int dele;       // 是否要刪除

    void (*Update)(struct _Elements*);
    void (*Draw)(struct _Elements*);
    void (*Destroy)(struct _Elements*);
    void (*Interact)(struct _Elements*);
} Elements;

#endif

