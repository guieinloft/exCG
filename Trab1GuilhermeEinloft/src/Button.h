#ifndef BUTTON_H
#define BUTTON_H

#include "Image.h"
#include "Mouse.h"
#include <stdlib.h>

class Button {
    public:
    //construtor (cria um botão de largura w e h na pos (x,y)
    Button(int x, int y, int w, int h);
    //verifica se o botão foi clicado
    //obs: não seleciona o botão, o botão deve selecionado na função
    //chamadora, pois depende do contexto
    int checkClick(Mouse mouse);
    //mostra o botão na tela
    void Render();
    //retorna se foi pressionado
    bool isPressed();
    //retorna se está selecionado
    bool isSelected();
    //seleciona/desseleciona o botão (usar após o método checkClick)
    void select(bool s);
    //muda ícone caso o botão tenha 2 ícones
    void changeIcon();
    //carrega os ícones definidos nos paths
    void loadIcons(const char *path1, const char *path2 = NULL);
    //muda o texto do botão
    void changeText(const char *text);
    //muda a posição do botão
    void changePosition(int x, int y);

    protected:
    //posição
    int x, y;
    //largura
    int w, h;
    //ícones
    Image icon1;
    Image icon2;
    //texto
    bool iconCur;
    char text[256];
    //true se o botão está selecionado, c.c. falso
    bool selected;
    //true se o botão foi pressionado, c.c. falso
    bool pressed;
    //true se o mouse está em cima do botão, c.c. falso
    //útil apenas para o método Render
    bool hovered;
};

#endif
