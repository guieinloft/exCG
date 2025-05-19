#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Button.h"
#include "Mouse.h"

class Textbox {
    public:
    //construtor (cria textbox na posição (x,y))
    Textbox(int x, int y);
    //verifica se a textbox foi clicada e seleciona caso tenha sido
    //desseleciona caso o clique for fora da textbox
    void checkMouse(Mouse mouse);
    //verifica se a tecla é um caracter ascii válido e adiciona ao texto
    bool checkKeyboard(int key);
    //mostra a textbox na tela
    void Render();
    //muda a posição da textbox
    void changePosition(int x, int y);
    //reseta o texto da textbox
    void reset();
    //retorna o texto da textbox
    char *getText();

    private:
    //texto da textbox
    char text[256];
    //tamanho atual do texto
    int text_size;
    //posição
    int x, y;
    //botão da textbox
    Button *button;
};

#endif
