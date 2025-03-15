#include "../gl_canvas2d.h"
#include "../Vector2.h"
#include "../Pacman.h"
#include "Ghost.h"
#include "Inky.h"

#include <stdlib.h>

void Inky::calc_target_chase(char map[32][30], Pacman *pac, Ghost *ghost) {
    Vector2 p_pos = pac->get_position();
    Vector2 b_pos = ghost->get_position();
    Vector2 pos;
    pos.x = 2 * p_pos.x - b_pos.x;
    pos.y = 2 * p_pos.y - b_pos.y;
    calc_target_target(map, pos);
}

void Inky::calc_target(char map[32][30], Pacman *pac, Ghost *ghost) {
    switch (state) {
        case 0:
        calc_target_chase(map, pac, ghost);
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

void Inky::move(char map[32][30], Pacman *pac, Ghost *ghost) {
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
    //g_pos.x = (int)(r_pos.x)/16;
    //g_pos.y = (int)(r_pos.y)/16;

    printf("\nINKY: %lf %lf %lf %lf", g_pos.x, g_pos.y, t_pos.x, t_pos.y);

    if (g_pos.x == t_pos.x && g_pos.y == t_pos.y) {
        calc_target(map, pac, ghost);
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
