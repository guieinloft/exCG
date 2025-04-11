#ifndef LAYER_H
#define LAYER_H

#include "Image.h"

#include <stdio.h>
#include <stdint.h>

class Layer {
    public:
    //construtor e destrutor
    Layer(int x, int y);
    ~Layer();
    //carrega imagem que está em path
    void loadImage(char *path);
    //cria camada vazia de dimensões w*h
    void createBlank(int w, int h);
    //mostra camada na tela
    void Render(int x, int y);
    //salva a camada no projeto file
    void saveFile(FILE *file);
    //carrega a camada do projeto file
    void loadFile(FILE *file);
    //retornam posição
    int get_x();
    int get_y();
    //definem posição
    void set_x(int x);
    void set_y(int y);
    //define visibilidade da camada
    void setVisibility(bool v);
    bool getVisibility();
    //define opacidade da camada
    //não funciona se FAST_RENDER = 1
    void setOpacity(uint8_t opacity);
    //retorna opacidade
    uint8_t getOpacity();
    //retorna ponteiro para imagem
    Image *getImage();

    private:
    //posição
    int x, y;
    //imagem da camada (toda camada é uma imagem)
    Image *image;
    //visibilidade
    bool visible;
    //opacidade
    uint8_t opacity;
};

#endif
