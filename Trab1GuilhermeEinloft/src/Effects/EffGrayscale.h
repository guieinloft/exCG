#ifndef EFF_GRAYSCALE_H
#define EFF_GRAYSCALE_H

#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "Effect.h"


class EffGrayscale : public Effect {
    public:
    EffGrayscale(int x, int y);
    ~EffGrayscale();
    void apply(Image *image);
    void renderMenu();
    bool checkMouse(Mouse mouse, Image *image);
    void changePosition(int x, int y);

    private:
    Slider *sl_r;
    Slider *sl_g;
    Slider *sl_b;
};

#endif
