#include "LayerList.h"
#include "Layer.h"
#include "Button.h"
#include "gl_canvas2d.h"

LayerList::LayerList(int x, int y) {
    this->x = x;
    this->y = y;
    this->n_layers = 0;
    for (int i = 0; i < MAX_LAYERS; i++) {
        layers[i] = NULL;
        layerButtons[i] = NULL;
    }
    this->active_layer = 0;
    this->n_layers = 0;
    for (int i = 0; i < 8; i++)
        optionButtons[i] = new Button(x + i * 32, y + 32 * MAX_LAYERS, 32, 32);
    optionButtons[OPT_NEWLAYER]->loadIcons("images/icons/newlayer.bmp");
    optionButtons[OPT_IMGLAYER]->loadIcons("images/icons/imglayer.bmp");
    optionButtons[OPT_LAYERUP]->loadIcons("images/icons/layerup.bmp");
    optionButtons[OPT_LAYERDOWN]->loadIcons("images/icons/layerdown.bmp");
    optionButtons[OPT_LAYERDEL]->loadIcons("images/icons/layerdel.bmp");
    optionButtons[OPT_LAYERMRG]->loadIcons("images/icons/layermrg.bmp");
    optionButtons[OPT_SAVEFILE]->loadIcons("images/icons/savefile.bmp");
    optionButtons[OPT_LOADFILE]->loadIcons("images/icons/loadfile.bmp");
    this->newest_layer = 0;
}

void LayerList::RenderList() {
    CV::color(0.15, 0.15, 0.15);
    CV::rectFill(x, y, x + 255, y + 32 * MAX_LAYERS - 1);
    for (int i = 0; i < n_layers; i++)
        layerButtons[i]->Render();
    for (int i = 0; i < 8; i++)
        optionButtons[i]->Render();
}

void LayerList::createLayer() {
    newest_layer++;
    layers[n_layers] = new Layer(0, 0);
    layerButtons[n_layers] = new Button(x, y + 32 * (MAX_LAYERS - n_layers - 1), 256, 32);
    char layername[32];
    sprintf(layername, "CAMADA %d", newest_layer);
    layerButtons[n_layers]->changeText(layername);
    active_layer = n_layers;
    n_layers++;
}

void LayerList::moveLayer(int dir) {
    if (n_layers == 0) return;
    if (dir == 1 && active_layer == n_layers - 1) return;
    if (dir == -1 && active_layer == 0) return;
    Layer *temp = layers[active_layer + dir];
    layers[active_layer + dir] = layers[active_layer];
    layers[active_layer] = temp;
    Button *tempbt = layerButtons[active_layer + dir];
    layerButtons[active_layer + dir] = layerButtons[active_layer];
    layerButtons[active_layer] = tempbt;
    layerButtons[active_layer]->changePosition(x, y + 32 * (MAX_LAYERS - active_layer)); 
    layerButtons[active_layer + dir]->changePosition(x, y + 32 * (MAX_LAYERS - active_layer - dir));
    active_layer += dir;
}

void LayerList::removeLayer() {
    if (n_layers == 0) return;
    delete layers[active_layer];
    delete layerButtons[active_layer];
    for (int i = active_layer; i < MAX_LAYERS - 1; i++) {
        layers[i] = layers[i+1];
        layerButtons[i] = layerButtons[i+1];
        if (layerButtons[i] != NULL)
            layerButtons[i]->changePosition(x, y + 32 * (MAX_LAYERS - 1 - i));
    }
    layers[MAX_LAYERS - 1] = NULL;
    layerButtons[MAX_LAYERS - 1] = NULL;
    n_layers--;
    active_layer -= (active_layer != 0);
}

void LayerList::checkMouse(Mouse mouse) {
    int bt_status;
    for (int i = 0; i < n_layers; i++) {
        bt_status = layerButtons[i]->checkClick(mouse);
        if (bt_status == 1) active_layer = i;
    }
    for (int i = 0; i < n_layers; i++) {
        layerButtons[i]->select(i == active_layer);
    }
    for (int i = 0; i < OPT_NUM; i++) {
        bt_status = optionButtons[i]->checkClick(mouse);
        if (bt_status == 1) optionButtons[i]->select(true);
        else if (bt_status == 0) optionButtons[i]->select(false);
    }
    if (optionButtons[OPT_NEWLAYER]->isPressed() && n_layers < MAX_LAYERS) {
        createLayer();
        layers[active_layer]->createBlank(640, 480);
    }
    else if (optionButtons[OPT_IMGLAYER]->isPressed() && n_layers < MAX_LAYERS) {
        createLayer();
        layers[active_layer]->loadImage("images/testimg.bmp");
    }
    else if (optionButtons[OPT_LAYERUP]->isPressed())
        moveLayer(1);
    else if (optionButtons[OPT_LAYERDOWN]->isPressed())
        moveLayer(-1);
    else if (optionButtons[OPT_LAYERDEL]->isPressed())
        removeLayer();
    else if (optionButtons[OPT_LAYERMRG]->isPressed() && active_layer > 0) {
        layers[active_layer - 1]->getImage()->blend(
            *(layers[active_layer]->getImage()),
            layers[active_layer]->get_x(), layers[active_layer]->get_y(),
            layers[active_layer-1]->get_x(), layers[active_layer-1]->get_y());
        removeLayer();
    }
}

Layer *LayerList::getActiveLayer() {
    return layers[active_layer];
}

void LayerList::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < n_layers; i++)
        layerButtons[i]->changePosition(x, y + 32 * (MAX_LAYERS - 1 - i));
    for (int i = 0; i < 8; i++)
        optionButtons[i]->changePosition(x + i * 32, y + 32 * MAX_LAYERS);
}

Layer **LayerList::getLayers() {
    return layers;
}

uint8_t LayerList::getNLayers() {
    return n_layers;
}
