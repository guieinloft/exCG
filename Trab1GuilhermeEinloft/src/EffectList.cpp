#include "EffectList.h"
#include "Button.h"
#include "gl_canvas2d.h"
#include "Layer.h"

#include "Effects/Effect.h"
#include "Effects/EffBriCon.h"
#include "Effects/EffGrayscale.h"
#include "Effects/EffHSLAdjust.h"
#include "Effects/EffInvert.h"
#include "Effects/EffGamma.h"
#include "Effects/EffGaussian.h"

EffectList::EffectList(int x, int y) {
    for (int i = 0; i < EFF_NUM; i++)
        effectButtons[i] = new Button(x, y + i * 32, 256, 32);
    //load texts
    effectButtons[EFF_BRI_CON]->changeText("Brilho e Contraste");
    effectButtons[EFF_GRAYSCALE]->changeText("Escala de Cinza");
    effectButtons[EFF_HSL_ADJUST]->changeText("Ajuste HSL");
    effectButtons[EFF_INVERT]->changeText("Inverter Cores");
    effectButtons[EFF_GAMMA]->changeText("Correcao de Gama");
    effectButtons[EFF_GAUSSIAN]->changeText("Desfoque Gaussiano");
    selectedEffect = -1;
    this->x = x;
    this->y = y;

    //load effects
    effects[EFF_BRI_CON] = new EffBriCon(x, y);
    effects[EFF_GRAYSCALE] = new EffGrayscale(x, y);
    effects[EFF_HSL_ADJUST] = new EffHSLAdjust(x, y);
    effects[EFF_INVERT] = new EffInvert(x, y);
    effects[EFF_GAMMA] = new EffGamma(x, y);
    effects[EFF_GAUSSIAN] = new EffGaussian(x, y);
}

EffectList::~EffectList() {
    for (int i = 0; i < EFF_NUM; i++)
        delete effectButtons[i];
    delete effects[EFF_BRI_CON];
    delete effects[EFF_GRAYSCALE];
    delete effects[EFF_HSL_ADJUST];
    delete effects[EFF_INVERT];
    delete effects[EFF_GAMMA];
    delete effects[EFF_GAUSSIAN];
}

void EffectList::Render() {
    CV::color(0.15, 0.15, 0.15);
    CV::rectFill(x, y, x + 255, y + 255);
    if (selectedEffect == -1) {
        for (int i = 0; i < EFF_NUM; i++)
            effectButtons[i]->Render();
    }
    else effects[selectedEffect]->renderMenu();
}

void EffectList::checkMouse(Mouse mouse, Layer *layer, Canvas *canvas) {
    if (selectedEffect == -1 || layer == NULL) {
        for (int i = 0; i < EFF_NUM; i++) {
            int bt_status = effectButtons[i]->checkClick(mouse);
            if (bt_status == 1) 
                selectedEffect = i;
        }
    }
    else if (effects[selectedEffect]->checkMouse(mouse, layer->getImage())) {
        selectedEffect = -1;
        canvas->update();
    }
}

void EffectList::changePosition(int x, int y) {
    for (int i = 0; i < EFF_NUM; i++) {
        effectButtons[i]->changePosition(x, y + i * 32);
        effects[i]->changePosition(x, y);
    }
    this->x = x;
    this->y = y;
}
