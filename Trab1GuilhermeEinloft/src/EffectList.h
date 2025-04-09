#ifndef EFFECT_LIST_H
#define EFFECT_LIST_H

#include "Button.h"
#include "Layer.h"
#include "Canvas.h"

#include "Effects/Effect.h"

enum Effects {
    EFF_BRI_CON,
    EFF_GRAYSCALE,
    EFF_HSL_ADJUST,
    EFF_INVERT,
    EFF_GAMMA,
    EFF_GAUSSIAN,
    EFF_NUM
};

class EffectList {
    public:
    EffectList(int x, int y);
    ~EffectList();
    void Render();
    void checkMouse(Mouse mouse, Layer *layer, Canvas *canvas);
    void changePosition(int x, int y);

    private:
    Button *effectButtons[EFF_NUM];
    int selectedEffect;
    int x, y;
    Effect *effects[EFF_NUM];
};

#endif
