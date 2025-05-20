#ifndef CANVAS_H
#define CANVAS_H

#include "Layer.h"
#include "Image.h"

//ignora alpha, opacidade e blending entre camadas
//trata o alpha como se fosse transparencia binaria
#define FAST_RENDER 1

class Canvas {
    public:
    //construtor/destrutor
    Canvas(int w, int h);
    ~Canvas();
    //mostra o canvas na tela e atualiza a posição
    void Render(int sw, int sh, Layer **layers, int n_layers);
    //retorna posição
    int get_x();
    int get_y();
    //retorna dimensões
    int get_w();
    int get_h();
    //manda atualizar a imagem do canvas (caso FAST_RENDER = 1)
    void update();

    private:
    //posição e dimensões
    int x, y;
    int w, h;
    //bool que define se atualiza na render
    bool up;
    //as imagens de fundo transparente
    Image *canvas_bg;
    Image *canvas;
};

#endif
