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
    //construtor/destrutor
    RightMenu(int x, int y);
    ~RightMenu();
    //mostra color picker ou a lista de efeitos na tela
    void Render();
    //verifica se o menu atual foi clicado
    bool checkMouse(Mouse mouse, Layer *layer, Canvas *canvas);
    //muda posição
    void changePosition(int x, int y);
    //pega as cores do color picker
    rgb_color *getFGColor();
    rgb_color *getBGColor();

    private:
    //posição
    int x, y;
    //botões para selecionar o menu
    Button *menuButtons[2];
    //botão selecionado
    bool selectedMenu;
    //color picker
    ColorPicker *color_picker;
    //lista de efeitos
    EffectList *effect_list;
};

#endif
