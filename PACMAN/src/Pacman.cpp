#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Pacman.h"
#include <stdio.h>

#define PAC_SPEED 1.0/16.0

Pacman::Pacman(int init_x, int init_y) {
    g_pos.x = (float)init_x - 1;
    g_pos.y = (float)init_y;
    t_pos.x = init_x;
    t_pos.y = init_y;
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.y * 16 + 8;
    a_dir = 3;
    t_dir = 3;
    score = 0;
    state = 0;
    super = 0;
}

void Pacman::pac_render() {
    printf("\nESTADO: %d", state);
    if (state != -1) {
        CV::color(1, 1, 0);
        CV::circleFill(r_pos.x, r_pos.y, 16, 16);
    }
}

void Pacman::handle_input(int key) {
    t_dir = key - 199;
    switch (t_dir) {
        case 1:
        if (a_dir == 3) t_dir = a_dir;
        break;

        case 2:
        if (a_dir == 4) t_dir = a_dir;
        break;

        case 3:
        if (a_dir == 1) t_dir = a_dir;
        break;

        case 4:
        if (a_dir == 2) t_dir = a_dir;
        break;
    }
}

void Pacman::collide_adir(char map[32][30]) {
    int x_inc = 0, y_inc = 0;
    switch (a_dir) {
    case 1:
        x_inc = -1;
        break;

    case 2:
        y_inc = -1;
        break;

    case 3:
        x_inc = 1;
        break;

    case 4:
        y_inc = 1;
        break;
        
    default:
        t_dir = a_dir;
        break;
    }
    if (map[(int)t_pos.y + y_inc][(int)t_pos.x + x_inc] == '|' || map[(int)t_pos.y + y_inc][(int)t_pos.x + x_inc] == '-') {
        a_dir = 0;
    }
    else {
        t_pos.x += x_inc;
        t_pos.y += y_inc;
    }
    t_dir = a_dir;
}

void Pacman::collide(char map[32][30]) {
    //checar se foi para um dos cantos da tela
    if (g_pos.x == 0) {
        g_pos.x = 29;
        t_pos.x = 29;
    }
    else if (g_pos.x == 29) {
        g_pos.x = 0;
        t_pos.x = 0;
    }

    //checar se comeu pastilha
    if (map[(int)t_pos.y][(int)t_pos.x] == '.') {
        map[(int)t_pos.y][(int)t_pos.x] = ' ';
        score += 10;
    }
    if (map[(int)t_pos.y][(int)t_pos.x] == '+') {
        map[(int)t_pos.y][(int)t_pos.x] = ' ';
        score += 50;
        super = 300;
    }

    int x_inc = 0, y_inc = 0;
    switch (t_dir) {
    case 1:
        x_inc = -1;
        break;

    case 2:
        y_inc = -1;
        break;

    case 3:
        x_inc = 1;
        break;

    case 4:
        y_inc = 1;
        break;
        
    default:
        t_dir = a_dir;
        break;
    }
    if (map[(int)t_pos.y + y_inc][(int)t_pos.x + x_inc] == '|' || map[(int)t_pos.y + y_inc][(int)t_pos.x + x_inc] == '-') {
        collide_adir(map);
    }
    else {
        t_pos.x += x_inc;
        t_pos.y += y_inc;
        a_dir = t_dir;
    }
}

void Pacman::move(char map[32][30]) {
    switch (a_dir) {
    case 1:
        g_pos.x -= PAC_SPEED;
        break;

    case 2:
        g_pos.y -= PAC_SPEED;
        break;

    case 3:
        g_pos.x += PAC_SPEED;
        break;

    case 4:
        g_pos.y += PAC_SPEED;
        break;

        default:
        break;
    }
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.y * 16 + 8;

    if (g_pos.x == t_pos.x && g_pos.y == t_pos.y) {
        collide(map);
    }
    super -= (super > 0);
}

int Pacman::get_score() {
    return score;
}

void Pacman::set_super(int s) {
    super = s;
    state = 1;
}

int Pacman::get_super() {
    int s = super;
    super -= (super > 0);
    state = (super > 0) - (state == -1);
    return s;
}

Vector2 Pacman::get_position() {
    return t_pos;
}

void Pacman::set_state(int s) {
    this->state = s;
    printf("\nESTADO: %d", state);
}
