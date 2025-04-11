#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include "colors.h"
#include "Slider.h"
#include "Button.h"

class ColorPicker {
    public:
    //construtor/destrutor
    ColorPicker(int x, int y);
    ~ColorPicker();
    //mostra o color picker na tela
    void Render();
    //verifica se os sliders foram clicados
    //ou se a cor foi trocada
    void checkMouse(Mouse mouse);
    //pega as cores principal/secundaria
    rgb_color *getFGColor();
    rgb_color *getBGColor();
    //muda a cor atual
    void changeCurColor(rgb_color color);
    //muda posição
    void changePosition(int x, int y);

    private:
    //posição
    int x;
    int y;
    //cores
    rgb_color colors[2];
    //cor atual
    bool currentColor;
    //sliders de cor
    Slider *sliders[4];
    //botões para selecionar a cor
    Button *buttons[2];
};

#endif
