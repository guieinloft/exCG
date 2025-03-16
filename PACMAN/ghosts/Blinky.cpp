#include "../gl_canvas2d.h"
#include "../Vector2.h"
#include "../Pacman.h"
#include "Ghost.h"
#include "Blinky.h"

#include <stdlib.h>

void Blinky::calc_target_chase(char map[32][30], Pacman *pac) {
    calc_target_target(map, pac->get_position());
}

void Blinky::calc_target(char map[32][30], Pacman *pac) {
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
        calc_target_ghost_house(map);
        break;
    }
    //checar se foi para um dos cantos da tela
    if (t_pos.x <= 0) {
        g_pos.x = 29;
        t_pos.x = 28;
    }
    else if (t_pos.x >= 29) {
        g_pos.x = 0;
        t_pos.x = 1;
    }

}

void Blinky::move(char map[32][30], Pacman *pac) {
    switch (a_dir) {
    case 1:
        g_pos.x -= speed;
        break;

    case 2:
        g_pos.y -= speed;
        break;

    case 3:
        g_pos.x += speed;
        break;

    case 4:
        g_pos.y += speed;
        break;

        default:
        break;
    }
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.y * 16 + 8;

    if (g_pos.x == t_pos.x && g_pos.y == t_pos.y) {
        calc_target(map, pac);
    }
    collide_pacman(pac);
    if (state < 2 && timer_i < 7) {
        timers[timer_i]--;
        if (timers[timer_i] <= 0) {
            timer_i++;
            set_state(0);
        }
    }
}
