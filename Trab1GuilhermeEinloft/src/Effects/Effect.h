#ifndef EFFECT_H
#define EFFECT_H

#include "../Image.h"
#include "../Button.h"

//função auxiliar para manter um valor entre outros dois valores
//geralmente 0 e 255
inline float truncate(float a, float l, float r) {
    return (l * (a < l) + r * (a > r) + a * (a >= l && a <= r));
}

class Effect {
    public:
    //construtor
    Effect(int x, int y);
    //destrutor
    virtual ~Effect();
    //aplica o efeito na imagem
    virtual void apply(Image *image);
    //renderiza menu do efeito
    virtual void renderMenu();
    //verifica se os botões no menu foram clicados
    virtual bool checkMouse(Mouse mouse, Image *image);
    //muda a posição do menu
    virtual void changePosition(int x, int y);

    protected:
    int x, y;
    int params[4];
    //botões comuns a todos
    Button *btOK;
    Button *btCancel;
};

#endif
