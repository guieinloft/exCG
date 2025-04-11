#ifndef EFFECT_LIST_H
#define EFFECT_LIST_H

#include "Button.h"
#include "Layer.h"
#include "Canvas.h"

#include "Effects/Effect.h"

enum Effects {
    EFF_BRI_CON,
    EFF_GRAYSCALE,
    EFF_HSL_ADJUST,
    EFF_INVERT,
    EFF_GAMMA,
    EFF_GAUSSIAN,
    EFF_NUM
};

class EffectList {
    public:
    //construtor/destrutor
    EffectList(int x, int y);
    ~EffectList();
    //mostra a lista de efeitos na tela
    void Render();
    //verifica se a lista de efeitos foi clicada
    //ou se os botões específicos do efeito foram clicados
    //e aplica o efeito atual se for necessário
    void checkMouse(Mouse mouse, Layer *layer, Canvas *canvas);
    //muda posição da lista de efeitos
    void changePosition(int x, int y);

    private:
    //botões para cada efeito
    Button *effectButtons[EFF_NUM];
    //efeito selecionado
    int selectedEffect;
    //posição
    int x, y;
    //efeitos
    Effect *effects[EFF_NUM];
};

#endif
