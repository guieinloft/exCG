#include "RightMenu.h"
#include "ColorPicker.h"
#include "EffectList.h"
#include "Button.h"
#include "colors.h"
#include "Layer.h"
#include "Canvas.h"

RightMenu::RightMenu(int x, int y) {
    for (int i = 0; i < 2; i++)
        menuButtons[i] = new Button(x + i * 128, y, 128, 32);
    //load texts
    menuButtons[0]->changeText("CORES");
    menuButtons[1]->changeText("EFEITOS");
    selectedMenu = 0;
    color_picker = new ColorPicker(x, y + 32);
    effect_list = new EffectList(x, y + 32);
    this->x = x;
    this->y = y;
}

RightMenu::~RightMenu() {
    for (int i = 0; i < 2; i++)
        delete menuButtons[i];
    delete color_picker;
    delete effect_list;
}

void RightMenu::Render() {
    for (int i = 0; i < 2; i++)
        menuButtons[i]->Render();
    if (!selectedMenu) color_picker->Render();
    else effect_list->Render();
}

bool RightMenu::checkMouse(Mouse mouse, Layer *layer, Canvas *canvas) {
    for (int i = 0; i < 2; i++) {
        int bt_status = menuButtons[i]->checkClick(mouse);
        if (bt_status == 1) selectedMenu = i;
    }
    menuButtons[0]->select(!selectedMenu);
    menuButtons[1]->select(selectedMenu);
    if (!selectedMenu) color_picker->checkMouse(mouse);
    else effect_list->checkMouse(mouse, layer, canvas);
    return (mouse.x > this->x - 8);
}

void RightMenu::changePosition(int x, int y) {
    for (int i = 0; i < 2; i++)
        menuButtons[i]->changePosition(x + i * 128, y);
    color_picker->changePosition(x, y + 32);
    effect_list->changePosition(x, y + 32);
    this->x = x;
    this->y = y;
}

rgb_color *RightMenu::getFGColor() {
    return color_picker->getFGColor();
}

rgb_color *RightMenu::getBGColor() {
    return color_picker->getBGColor();
}
