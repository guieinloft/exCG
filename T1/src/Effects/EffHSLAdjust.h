#ifndef EFF_HSL_ADJUST_H
#define EFF_HSL_ADJUST_H

#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "Effect.h"


class EffHSLAdjust : public Effect {
    public:
    EffHSLAdjust(int x, int y);
    ~EffHSLAdjust();
    void apply(Image *image);
    void renderMenu();
    bool checkMouse(Mouse mouse, Image *image);
    void changePosition(int x, int y);

    private:
    Slider *sl_h;
    Slider *sl_s;
    Slider *sl_l;
};

#endif
