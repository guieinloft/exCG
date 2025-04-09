#ifndef EFFECT_H
#define EFFECT_H

#include "../Image.h"
#include "../Button.h"

inline float truncate(float a, float l, float r) {
    return (l * (a < l) + r * (a > r) + a * (a >= l && a <= r));
}

class Effect {
    public:
    Effect(int x, int y);
    virtual ~Effect();
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
