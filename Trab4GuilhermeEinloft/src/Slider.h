#ifndef SLIDER_H
#define SLIDER_H

#include "Button.h"
#include <stdint.h>

class Slider {
    public:
    //construtor/destrutor
    Slider(int x, int y);
    ~Slider();
    //verifica se o botão do slider foi clicado e seleciona
    void checkMouse(Mouse mouse);
    //mostra slider na tela
    void Render();
    //muda posição do slider
    void changePosition(int x, int y);
    //retorna parâmetro do slider
    uint8_t getParam();
    //muda parâmetro do slider e atualiza o botão
    void changeParam(uint8_t new_param);
    //retorna se o slider está selecionado
    bool isSelected();

    protected:
    //posição
    int x;
    int y;
    //parâmetro
    uint8_t param;
    //botão
    Button *button;
};

#endif
