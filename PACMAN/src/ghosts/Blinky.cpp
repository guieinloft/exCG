#include "../gl_canvas2d.h"
#include "../Vector2.h"
#include "../Pacman.h"
#include "Ghost.h"
#include "Blinky.h"

#include <stdlib.h>

void Blinky::Render() {
    switch (state) {
        case 2:
        CV::color(1, 1, 1);
        CV::circleFill(r_pos.x, r_pos.y, 16, 16);
        CV::rectFill(r_pos.x - 16, r_pos.y, r_pos.x + 16, r_pos.y + 16);
        CV::color(0, 0, 1);
        CV::circleFill(r_pos.x-6, r_pos.y, 6, 8);
        CV::circleFill(r_pos.x+6, r_pos.y, 6, 8);
        break;

        case 3:
        CV::color(1, 1, 1);
        CV::circleFill(r_pos.x-6, r_pos.y, 6, 8);
        CV::circleFill(r_pos.x+6, r_pos.y, 6, 8);
        break;
        
        default:
        CV::color(1, 0, 0);
        CV::circleFill(r_pos.x, r_pos.y, 16, 16);
        CV::rectFill(r_pos.x - 16, r_pos.y, r_pos.x + 16, r_pos.y + 16);
        CV::circleFill(r_pos.x-6, r_pos.y, 6, 8);
        CV::circleFill(r_pos.x+6, r_pos.y, 6, 8);
        break;
    }
}

void Blinky::calc_target_chase(char map[32][30], Pacman *pac) {
    calc_target_target(map, pac->get_position());
}

void Blinky::calc_target(char map[32][30], Pacman *pac) {
    //checar se foi para um dos cantos da tela
    if (g_pos.x == 0) {
        g_pos.x = 29;
        t_pos.x = 29;
    }
    else if (g_pos.x == 29) {
        g_pos.x = 0;
        t_pos.x = 0;
    }

    switch (state) {
        case 0:
        calc_target_chase(map, pac);
        break;

        case 1:
        calc_target_scatter(map);
        break;

        case 2:
        calc_target_fright(map);
        break;

        case 3:
        calc_target_eaten(map);
        break;

        default:
        calc_target_fright(map);
        break;
    }
}

void Blinky::move(char map[32][30], Pacman *pac) {
    switch (a_dir) {
    case 1:
        g_pos.x -= GHOST_SPEED;
        break;

    case 2:
        g_pos.y -= GHOST_SPEED;
        break;

    case 3:
        g_pos.x += GHOST_SPEED;
        break;

    case 4:
        g_pos.y += GHOST_SPEED;
        break;

        default:
        break;
    }
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.y * 16 + 8;
    //g_pos.x = (int)(r_pos.x)/16;
    //g_pos.y = (int)(r_pos.y)/16;


    if (g_pos.x == t_pos.x && g_pos.y == t_pos.y) {
        calc_target(map, pac);
    }
    collide_pacman(pac);
}
