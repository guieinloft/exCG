#ifndef LAYER_LIST_H
#define LAYER_LIST_H

#include "Layer.h"
#include "Button.h"

#define MAX_LAYERS 3

enum Options {
    OPT_NEWLAYER,
    OPT_IMGLAYER,
    OPT_LAYERUP,
    OPT_LAYERDOWN,
    OPT_LAYERDEL,
    OPT_LAYERMRG,
    OPT_SAVEFILE,
    OPT_LOADFILE,
    OPT_NUM
};

class LayerList {
    public:
    LayerList(int x, int y);
    void RenderList();
    void checkMouse(Mouse mouse);
    Layer *getActiveLayer();
    void changePosition(int x, int y);
    Layer **getLayers();
    uint8_t getNLayers();

    private:
    int x, y;
    Layer *layers[MAX_LAYERS];
    int n_layers;
    uint8_t active_layer;
    Button *layerButtons[MAX_LAYERS];
    Button *optionButtons[8];
    int newest_layer;
    void createLayer();
    void moveLayer(int dir);
    void removeLayer();
};

#endif
