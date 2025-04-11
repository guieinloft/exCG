#ifndef LAYER_LIST_H
#define LAYER_LIST_H

#include "Layer.h"
#include "Button.h"
#include "Slider.h"
#include "Textbox.h"
#include "Canvas.h"

#include <stdint.h>

#define MAX_LAYERS 6

enum Options {
    OPT_NEWLAYER,
    OPT_LAYERUP,
    OPT_LAYERDOWN,
    OPT_LAYERDEL,
    OPT_LAYERCOPY,
    OPT_LAYERMRG,
    OPT_SAVEFILE,
    OPT_LOADFILE,
    OPT_NUM
};

class LayerList {
    public:
    //construtor/destrutor
    LayerList(int x, int y);
    ~LayerList();
    //mostra a lista de camadas na tela
    void RenderList();
    //verifica se os botões da camada foram clicados
    //ou se a textbox e os botões de confirmação foram clicados
    //cria/move/remove camada apropriadamente
    bool checkMouse(Mouse mouse, Canvas *canvas);
    //verifica a tecla para a textbox
    //cria/move/remove camada apropriadamente
    void checkKeyboard(int key, Canvas *canvas);
    //retorna camada ativa
    Layer *getActiveLayer();
    //muda posição da lista de camada
    void changePosition(int x, int y);
    //retorna vetor de camadas
    Layer **getLayers();
    //retorna número de camadas
    uint8_t getNLayers();

    private:
    //posição
    int x, y;
    //vetor de camadas
    Layer *layers[MAX_LAYERS];
    //número atual de camadas
    int n_layers;
    //camada ativa
    uint8_t active_layer;
    //botões de camadas
    Button *layerButtons[MAX_LAYERS];
    //botões de visibilidade de camadas
    Button *layerOpButtons[MAX_LAYERS];
    //botões de opções
    Button *optionButtons[8];
    //slider de opacidade
    Slider *sl_opacity;
    //layer mais recente (usado para nomear a próxima camada)
    int newest_layer;
    //funções auxiliares pra criar/mover/remover camada e botões
    void createLayer();
    void moveLayer(int dir);
    void removeLayer();
    //carrega/salva imagem no caminho definido na textbox
    void loadImage();
    void saveImage();
    //carrega/salva projeto no caminho definido na textbox
    void loadProject();
    void saveProject();
    //mostrar popup da textbox
    //tipo do popup da textbox: 0 = salvar; 1 = carregar
    bool popup, popupType;
    //textbox
    Textbox *textbox;
    //botões de confirmação e cancelamento
    Button *btOk;
    Button *btCancel;
};

#endif
