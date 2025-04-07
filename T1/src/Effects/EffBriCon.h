#ifndef EFF_BRI_CON_H
#define EFF_BRI_CON_H

#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "Effect.h"


class EffBriCon : public Effect {
    public:
    EffBriCon(int x, int y);
    ~EffBriCon();
    void apply(Image *image);
    void renderMenu();
    bool checkMouse(Mouse mouse, Image *image);
    void changePosition(int x, int y);

    private:
    Slider *sl_bri;
    Slider *sl_con;
};

#endif
