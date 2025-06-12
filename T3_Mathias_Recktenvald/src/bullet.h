/*
    Define a classe Bullet, responsável pelas balas do jogo 2D.
    Gerencia posição, movimento, destruição, renderização e estado ativo das balas.
*/

#ifndef BULLET_H
#define BULLET_H

#include <stdio.h>
#include <stdlib.h>
#include "gl_canvas2d.h"
#include "Vector2.h"

class Bullet {
    Vector2 position;
    float speed;
    float angle;
    bool active = true;

public:
    Bullet(float x = -1, float y = -1, float s = 0, float a = 0) : position(x, y), speed(8), angle(a) {}

    void move() {
        position.x += speed * cos(angle);
        position.y += speed * sin(angle);
    }

    void destroy() {
        active = false;
        position.x = -1;
        position.y = -1;
    }

    void verifyPosition() {
        if (position.x < 0 || position.x > SCREEN_WIDTH || position.y < 0 || position.y > SCREEN_HEIGHT) {
            destroy();
        }
    }

    void render(bool enemy) {
        move();
        if (enemy) {
            CV::color(1, 1, 0);
        } else {
            CV::color(0, 1, 0);
        }
        CV::circleFill(position.x, position.y, 5.0, 10);
        verifyPosition();
    }

    Vector2 getPosition() const {
        return position;
    }

    bool isActive() const {
        return active;
    }
};

#endif