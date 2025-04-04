#ifndef EFF_INVERT_H
#define EFF_INVERT_H

#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "Effect.h"


class EffInvert : public Effect {
    public:
    EffInvert(int x, int y);
    void apply(Image *image);
    void renderMenu();
    bool checkMouse(Mouse mouse, Image *image);
    void changePosition(int x, int y);

    private:
    Slider *sl_thres;
    Button *btType[2];
};

#endif
