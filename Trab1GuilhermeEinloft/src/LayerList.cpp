#include "LayerList.h"
#include "Layer.h"
#include "Button.h"
#include "Slider.h"
#include "Textbox.h"
#include "gl_canvas2d.h"

#include <stdlib.h>
#include <stdio.h>

LayerList::LayerList(int x, int y) {
    this->x = x;
    this->y = y;
    this->n_layers = 0;
    for (int i = 0; i < MAX_LAYERS; i++) {
        layers[i] = NULL;
        layerButtons[i] = NULL;
        layerOpButtons[i] = NULL;
    }
    this->active_layer = 0;
    this->n_layers = 0;
    for (int i = 0; i < 8; i++)
        optionButtons[i] = new Button(x + i * 32, y + 32 * MAX_LAYERS, 32, 32);
    optionButtons[OPT_NEWLAYER]->loadIcons("./T1/images/icons/newlayer.bmp");
    optionButtons[OPT_LAYERUP]->loadIcons("./T1/images/icons/layerup.bmp");
    optionButtons[OPT_LAYERDOWN]->loadIcons("./T1/images/icons/layerdown.bmp");
    optionButtons[OPT_LAYERDEL]->loadIcons("./T1/images/icons/layerdel.bmp");
    optionButtons[OPT_LAYERCOPY]->loadIcons("./T1/images/icons/layercopy.bmp");
    optionButtons[OPT_LAYERMRG]->loadIcons("./T1/images/icons/layermrg.bmp");
    optionButtons[OPT_SAVEFILE]->loadIcons("./T1/images/icons/savefile.bmp");
    optionButtons[OPT_LOADFILE]->loadIcons("./T1/images/icons/loadfile.bmp");
    this->newest_layer = 0;
    popup = 0;
    textbox = new Textbox(x, y + 32);
    btOk = new Button(x, y + 96, 128, 32);
    btCancel = new Button(x+128, y + 96, 128, 32);
    btOk->changeText("OK");
    btCancel->changeText("CANCELAR");
    sl_opacity = new Slider(x, y + 32 * MAX_LAYERS + 64);
}

LayerList::~LayerList() {
    while (n_layers > 0) removeLayer();
    for (int i = 0; i < 8; i++)
        delete optionButtons[i];
    delete textbox;
    delete btOk;
    delete btCancel;
    delete sl_opacity;
}

void LayerList::RenderList() {
    if (popup) {
        CV::color(0.5, 0.5, 0.5);
        if (popupType == 0)
            CV::text(x, y+30, "SALVAR ARQUIVO:");
        else CV::text(x, y+30, "CARREGAR ARQUIVO:");
        textbox->Render();
        btOk->Render();
        btCancel->Render();
    }
    else {
        CV::color(0.15, 0.15, 0.15);
        CV::rectFill(x, y, x + 255, y + 32 * MAX_LAYERS - 1);
        for (int i = 0; i < n_layers; i++) {
            layerButtons[i]->Render();
            layerOpButtons[i]->Render();
        }
        for (int i = 0; i < 8; i++)
            optionButtons[i]->Render();
        CV::text(x, y + 32 * MAX_LAYERS + 46, "Opacidade:");
        sl_opacity->Render();
    }
}

void LayerList::createLayer() {
    newest_layer++;
    layers[n_layers] = new Layer(0, 0);
    layerButtons[n_layers] = new Button(x, y + 32 * (MAX_LAYERS - n_layers - 1), 256, 32);
    layerOpButtons[n_layers] = new Button(x + 228, y + 4 + 32 * (MAX_LAYERS - n_layers - 1), 24, 24);
    layerOpButtons[n_layers]->loadIcons("./T1/images/icons/vis_on.bmp", "./T1/images/icons/vis_off.bmp");
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
    tempbt = layerOpButtons[active_layer + dir];
    layerOpButtons[active_layer + dir] = layerOpButtons[active_layer];
    layerOpButtons[active_layer] = tempbt;
    layerOpButtons[active_layer]->changePosition(x + 228, y + 4 + 32 * (MAX_LAYERS - active_layer)); 
    layerOpButtons[active_layer + dir]->changePosition(x + 228, y + 4 + 32 * (MAX_LAYERS - active_layer - dir));
    active_layer += dir;
}

void LayerList::removeLayer() {
    if (n_layers == 0) return;
    delete layers[active_layer];
    delete layerButtons[active_layer];
    delete layerOpButtons[active_layer];
    for (int i = active_layer; i < MAX_LAYERS - 1; i++) {
        layers[i] = layers[i+1];
        layerButtons[i] = layerButtons[i+1];
        layerOpButtons[i] = layerOpButtons[i+1];
        if (layerButtons[i] != NULL && layerOpButtons[i] != NULL) {
            layerButtons[i]->changePosition(x, y + 32 * (MAX_LAYERS - 1 - i));
            layerOpButtons[i]->changePosition(x + 228, y + 4 + 32 * (MAX_LAYERS - 1 - i));
        }
    }
    layers[MAX_LAYERS - 1] = NULL;
    layerButtons[MAX_LAYERS - 1] = NULL;
    layerOpButtons[MAX_LAYERS - 1] = NULL;
    n_layers--;
    active_layer -= (active_layer != 0);
}

void LayerList::loadProject() {
    char path[300] = "./T1/projects/";
    strcat(path, textbox->getText());
    if (strstr(path, ".pcc") == NULL)
        strcat(path, ".pcc");
    FILE *file = fopen(path, "rb");
    if (file == NULL) return;
    while (n_layers > 0) removeLayer();
    fseek(file, 0x4, SEEK_SET);
    int layer_num;
    fread(&layer_num, sizeof(int), 1, file);
    for (int i = 0; i < layer_num; i++) {
        createLayer();
        layers[i]->loadFile(file);
    }
    fclose(file);
}

void LayerList::loadImage() {
    char path[280] = "./T1/images/";
    strcat(path, textbox->getText());
    if (n_layers >= MAX_LAYERS) return;
    createLayer();
    if (!layers[active_layer]->getImage()->bmp_load(path))
        removeLayer();
}

void LayerList::saveProject() {
    char path[300] = "./T1/projects/";
    strcat(path, textbox->getText());
    if (strstr(path, ".pcc") == NULL)
        strcat(path, ".pcc");
    FILE *file = fopen(path, "wb");
    if (file == NULL) return;
    fwrite("PCC_", sizeof(char), 4, file);
    fwrite(&n_layers, sizeof(int), 1, file);
    for (int i = 0; i < n_layers; i++)
        layers[i]->saveFile(file);
    fclose(file);
}

void LayerList::saveImage() {
    char path[280] = "./T1/images/";
    strcat(path, textbox->getText());
    Image *img = new Image();
    img->clear_image(640, 480);
    for (int i = 0; i < n_layers; i++)
        img->blend(layers[i]->getImage(), layers[i]->get_x(),
            layers[i]->get_y(), 0, 0, layers[i]->getOpacity());
    img->bmp_save(path);
    img->close_image();
    delete img;
}

bool LayerList::checkMouse(Mouse mouse, Canvas *canvas) {
    if (popup) {
        textbox->checkMouse(mouse);
        if (btOk->checkClick(mouse) == 1) {
            if (popupType == 1) {
                if (strstr(textbox->getText(), ".bmp") != NULL)
                    loadImage();
                else
                    loadProject();
                canvas->update();
            }
            else {
                if (strstr(textbox->getText(), ".bmp") != NULL)
                    saveImage();
                else
                    saveProject();
            }
            popup = 0;
            textbox->reset();
        }
        if (btCancel->checkClick(mouse) == 1) {
            popup = 0;
        }
        return popup;
    }
    int bt_status;
    for (int i = 0; i < n_layers; i++) {
        bt_status = layerButtons[i]->checkClick(mouse);
        if (bt_status == 1) active_layer = i;
        bt_status = layerOpButtons[i]->checkClick(mouse);
        if (bt_status == 1) {
            layerOpButtons[i]->select(true);
            layerOpButtons[i]->changeIcon();
            layers[i]->setVisibility(!layers[i]->getVisibility());
            canvas->update();
        }
        else if (bt_status == 0)
            layerOpButtons[i]->select(false);
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
        canvas->update();
    }
    else if (optionButtons[OPT_LAYERCOPY]->isPressed() && n_layers < MAX_LAYERS && n_layers > 0) {
        int old_layer = active_layer;
        createLayer();
        layers[active_layer]->getImage()->copy(layers[old_layer]->getImage());
        layers[active_layer]->set_x(layers[old_layer]->get_x());
        layers[active_layer]->set_y(layers[old_layer]->get_y());
        canvas->update();
    }
    else if (optionButtons[OPT_LAYERUP]->isPressed()) {
        moveLayer(1);
        canvas->update();
    }
    else if (optionButtons[OPT_LAYERDOWN]->isPressed()) {
        moveLayer(-1);
        canvas->update();
    }
    else if (optionButtons[OPT_LAYERDEL]->isPressed()) {
        removeLayer();
        canvas->update();
    }
    else if (optionButtons[OPT_LAYERMRG]->isPressed() && active_layer > 0) {
        layers[active_layer - 1]->getImage()->blend(
            layers[active_layer]->getImage(),
            layers[active_layer]->get_x(), layers[active_layer]->get_y(),
            layers[active_layer-1]->get_x(), layers[active_layer-1]->get_y(),
            layers[active_layer]->getOpacity());
        removeLayer();
        canvas->update();
    }
    else if (optionButtons[OPT_SAVEFILE]->isPressed()) {
        popup = 1;
        popupType = 0;
    }
    else if (optionButtons[OPT_LOADFILE]->isPressed()) {
        popup = 1;
        popupType = 1;
    }
    if (layers[active_layer] != NULL) {
        sl_opacity->changeParam(layers[active_layer]->getOpacity());
        uint8_t old_opacity = layers[active_layer]->getOpacity();
        sl_opacity->checkMouse(mouse);
        layers[active_layer]->setOpacity(sl_opacity->getParam());
        uint8_t new_opacity = layers[active_layer]->getOpacity();
        if (old_opacity != new_opacity) canvas->update();
    }
    return popup || sl_opacity->isSelected();
}

void LayerList::checkKeyboard(int key, Canvas *canvas) {
    if (textbox->checkKeyboard(key)) {
        if (popupType == 1) {
            if (strstr(textbox->getText(), ".bmp") != NULL)
                loadImage();
            else
                loadProject();
            canvas->update();
        }
        else {
            if (strstr(textbox->getText(), ".bmp") != NULL)
                saveImage();
            else
                saveProject();
        }
        popup = 0;
        textbox->reset();
    }
}

Layer *LayerList::getActiveLayer() {
    return layers[active_layer];
}

void LayerList::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < n_layers; i++) {
        layerButtons[i]->changePosition(x, y + 32 * (MAX_LAYERS - 1 - i));
        layerOpButtons[i]->changePosition(x + 228, y + 4 + 32 * (MAX_LAYERS - 1 - i));
    }
    for (int i = 0; i < 8; i++)
        optionButtons[i]->changePosition(x + i * 32, y + 32 * MAX_LAYERS);
    textbox->changePosition(x, y+32);
    btOk->changePosition(x, y+96);
    btCancel->changePosition(x+128, y+96);
    sl_opacity->changePosition(x, y + 32 * MAX_LAYERS + 64);
}

Layer **LayerList::getLayers() {
    return layers;
}

uint8_t LayerList::getNLayers() {
    return n_layers;
}
