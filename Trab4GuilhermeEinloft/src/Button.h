#ifndef BUTTON_H
#define BUTTON_H

#include "Mouse.h"

class Button {
    public:
    //construtor (cria um botão de largura w e h na pos (x,y)
    Button(int x, int y, int w, int h, bool sticky = 0);
    //verifica se o botão foi clicado e seleciona de acordo
    int checkClick(Mouse mouse);
    //mostra o botão na tela
    void Render();
    //retorna se foi pressionado
    bool isPressed();
    //retorna se está selecionado
    bool isSelected();
    //seleciona/desseleciona o botão
    void select(bool s);
    //muda o texto do botão
    void changeText(const char *text);
    //muda a posição do botão
    void changePosition(int x, int y);

    protected:
    //posição
    int x, y;
    //largura
    int w, h;
    //texto
    char text[256];
    //true se o botão está selecionado, c.c. falso
    bool selected;
    //true se o botão foi pressionado, c.c. falso
    bool pressed;
    //true se o mouse está em cima do botão, c.c. falso
    //útil apenas para o método Render
    bool hovered;
    //true se botao fica selecionado após largar o mouse
    bool sticky;
};

#endif
