#ifndef EFF_GAMMA_H
#define EFF_GAMMA_H

#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "Effect.h"


class EffGamma : public Effect {
    public:
    EffGamma(int x, int y);
    ~EffGamma();
    void apply(Image *image);
    void renderMenu();
    bool checkMouse(Mouse mouse, Image *image);
    void changePosition(int x, int y);

    private:
    Slider *sl_gamma;
};

#endif
