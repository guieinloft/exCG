#ifndef EFFECT_H
#define EFFECT_H

#include "../Image.h"
#include "../Button.h"

float truncate(float a, float l, float r);

class Effect {
    public:
    Effect(int x, int y);
    ~Effect();
    virtual void apply(Image *image);
    virtual void renderMenu();
    virtual bool checkMouse(Mouse mouse, Image *image);
    virtual void changePosition(int x, int y);

    protected:
    int x, y;
    int params[4];
    Button *btOK;
    Button *btCancel;
};

#endif
