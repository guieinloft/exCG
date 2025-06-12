/*
 * mouse.h
 * struct auxiliar contendo informações do mouse
 */

#ifndef MOUSE_H
#define MOUSE_H

typedef struct mouse_t {
    bool l, m, r;
    int x, y;
    int xp, yp;
    int button, state;
    int wheel, direction;
} Mouse;

#endif
