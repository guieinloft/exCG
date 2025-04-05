#ifndef LAYER_LIST_H
#define LAYER_LIST_H

#include "Layer.h"
#include "Button.h"
#include "Slider.h"
#include "Textbox.h"
#include "Canvas.h"

#include <stdint.h>

#define MAX_LAYERS 3

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
    LayerList(int x, int y);
    void RenderList();
    bool checkMouse(Mouse mouse, Canvas *canvas);
    void checkKeyboard(int key, Canvas *canvas);
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
    Button *layerOpButtons[MAX_LAYERS];
    Button *optionButtons[8];
    Slider *sl_opacity;
    int newest_layer;
    void createLayer();
    void moveLayer(int dir);
    void removeLayer();
    void loadImage();
    void saveImage();
    void loadProject();
    void saveProject();
    //file popup
    bool popup, popupType;
    Textbox *textbox;
    Button *btOk;
    Button *btCancel;
};

#endif
