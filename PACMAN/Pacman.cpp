#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Pacman.h"
#include <stdio.h>
#include <math.h>

#define PAC_SPEED 1.0/16.0

Pacman::Pacman(int init_x, int init_y) {
    g_pos.x = (float)init_x - 0.5;
    g_pos.y = (float)init_y;
    t_pos.x = init_x;
    t_pos.y = init_y;
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.y * 16 + 8;
    a_dir = 0;
    t_dir = 0;
    score = 0;
    state = -1;
    super = 0;
    angle = 45;
    offset = 0;
    invincibility = INVINCIBILITY;
    lives = 0;
    ghost_points = 200;
}

void Pacman::pac_render() {
    if (state == -1) return;
    CV::color(1, 1, 0);
    switch (a_dir) {
        case 1:
        offset = 180;
        break;

        case 2:
        offset = 270;
        break;

        case 3:
        offset = 0;
        break;

        case 4:
        offset = 90;
        break;

        default:
        break;

    }
    for (int i = 0; i < 360; i+=2) {
        if (i > abs(angle) && i < 360 - abs(angle)) {
            CV::line(r_pos.x, r_pos.y, r_pos.x + 16 * cos((i + offset)*M_PI/180), r_pos.y + 16 * sin((i + offset)*M_PI/180));
        }
    }
    angle = angle - 5 + 90 * (angle < -45);
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
    invincibility--;
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
    ghost_points = 200;
    return s;
}

Vector2 Pacman::get_position() {
    return t_pos;
}

int Pacman::get_direction() {
    return a_dir;
}

void Pacman::set_state(int s) {
    if (state == -1) return;
    if (s == -1 && invincibility <= 0) {
        lives -= 1;
        a_dir = 0;
    }
    state = s;
    invincibility = INVINCIBILITY;
}

int Pacman::get_state() {
    return state;
}

void Pacman::set_invincibility(int i) {
    invincibility = i;
}

int Pacman::get_invincibility() {
    return invincibility;
}

int Pacman::get_lives() {
    return lives;
}

void Pacman::reset(int init_x, int init_y, bool reset_score) {
    g_pos.x = (float)init_x - 0.5;
    g_pos.y = (float)init_y;
    t_pos.x = init_x;
    t_pos.y = init_y;
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.y * 16 + 8;
    a_dir = 3;
    t_dir = 3;
    state = 0;
    super = 0;
    angle = 45;
    offset = 0;
    if (reset_score) {
        score = 0;
        lives = 3;
    }
}

void Pacman::ghost_capture() {
    score += ghost_points;
    ghost_points <<= 1;
}
