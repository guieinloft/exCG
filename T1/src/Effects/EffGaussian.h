#ifndef EFF_GAUSSIAN_H
#define EFF_GAUSSIAN_H

#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "Effect.h"


class EffGaussian : public Effect {
    public:
    EffGaussian(int x, int y);
    void apply(Image *image);
    void renderMenu();
    bool checkMouse(Mouse mouse, Image *image);
    void changePosition(int x, int y);

    private:
    Slider *sl_radius;
};

#endif
