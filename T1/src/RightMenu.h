#ifndef RIGHT_MENU_H
#define RIGHT_MENU_H

#include "ColorPicker.h"
#include "EffectList.h"
#include "Button.h"
#include "colors.h"
#include "Layer.h"
#include "Canvas.h"

class RightMenu {
    public:
    RightMenu(int x, int y);
    void Render();
    void checkMouse(Mouse mouse, Layer *layer, Canvas *canvas);
    void changePosition(int x, int y);
    rgb_color *getFGColor();
    rgb_color *getBGColor();

    private:
    Button *menuButtons[EFF_NUM];
    bool selectedMenu;
    ColorPicker *color_picker;
    EffectList *effect_list;
};

#endif
