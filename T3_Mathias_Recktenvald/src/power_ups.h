/*
    Define a classe PowerUp para os power-ups do jogo 2D.
    Gerencia posição, tipo, estado ativo e renderização dos diferentes tipos de power-up (escudo, tiro rápido, vida, velocidade).
*/

#ifndef POWER_UPS_H
#define POWER_UPS_H

#include "Vector2.h"
#include "gl_canvas2d.h"

class PowerUp {
    Vector2 position;
    int type;
    int size;
    bool isActive;

public:
    PowerUp(float x, float y, int t, int s) : position(x, y), type(t), size(s), isActive(true) {}

    void setPosition(float x, float y) {
        position.set(x, y);
    }

    Vector2 getPosition() const {
        return position;
    }

    int getType() const {
        return type;
    }

    int getSize() const {
        return size;
    }

    bool getIsActive() const {
        return isActive;
    }

    void deactivate() {
        isActive = false;
    }

    void render() {
        if (isActive) {
            if (type == 0) {
                CV::color(0, 0, 1); // shield azul
            }
            if (type == 1) {
                CV::color(1, 0.5, 0); // shoot amp laranja
            }
            if (type == 2) {
                CV::color(1, 0, 0); // health vermelho
            }
            if (type == 3) {
                CV::color(0, 1, 0); // speed verde
            }
            CV::circleFill(position.x, position.y, size, 8);
        }
    }
};

#endif