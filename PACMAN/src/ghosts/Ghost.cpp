#include "../gl_canvas2d.h"
#include "../Pacman.h"
#include "../Vector2.h"
#include "Ghost.h"

#include <stdlib.h>

Ghost::Ghost(int init_x, int init_y, int scatter_x, int scatter_y) {
    g_pos.x = init_x - 0.5;
    g_pos.y = init_y;
    t_pos.x = init_x;
    t_pos.y = init_y;
    r_pos.x = g_pos.x * 16 + 8;
    r_pos.y = g_pos.x * 16 + 8;
    s_pos.x = scatter_x;
    s_pos.y = scatter_y;
    a_dir = 3;
    t_dir = 3;
    state = 1;
}

void Ghost::Render() {
    CV::color(1, 1, 1);
    CV::circleFill(r_pos.x, r_pos.y, 16, 16);
    CV::rectFill(r_pos.x - 16, r_pos.y, r_pos.x + 16, r_pos.y + 16);
}

void Ghost::calc_target_target(char map[32][30], Vector2 pos) {
    float distances[4][2];
    distances[0][0] = 2;
    distances[0][1] = (pos.x - g_pos.x) * (pos.x - g_pos.x) + (pos.y - (g_pos.y - 1)) * (pos.y - (g_pos.y - 1));
    
    distances[1][0] = 1;
    distances[1][1] = (pos.x - (g_pos.x - 1)) * (pos.x - (g_pos.x - 1)) + (pos.y - g_pos.y) * (pos.y - g_pos.y);

    distances[2][0] = 4;
    distances[2][1] = (pos.x - g_pos.x) * (pos.x - g_pos.x) + (pos.y - (g_pos.y + 1)) * (pos.y - (g_pos.y + 1));
    
    distances[3][0] = 3;
    distances[3][1] = (pos.x - (g_pos.x + 1)) * (pos.x - (g_pos.x + 1)) + (pos.y - g_pos.y) * (pos.y - g_pos.y);

    //sorting for distances
    for (int i = 1; i < 4; i++) {
        float temp[2];
        temp[0] = distances[i][0];
        temp[1] = distances[i][1];
        int j = i;
        while (j > 0 && distances[j-1][1] > temp[1]) {
            distances[j][0] = distances[j-1][0];
            distances[j][1] = distances[j-1][1];
            j--;
        }
        distances[j][0] = temp[0];
        distances[j][1] = temp[1];
    }
    for (int i = 0; i < 4; i++) {
        int x_inc = 0, y_inc = 0;
        switch ((int)distances[i][0]) {
        case 1:
            if (a_dir != 3)
                x_inc = -1;
            break;

        case 2:
            if (a_dir != 4)
                y_inc = -1;
            break;

        case 3:
            if (a_dir != 1)
                x_inc = 1;
            break;

        case 4:
            if (a_dir != 2)
                y_inc = 1;
            break;
            
        default:
            break;
        }
        if (!(map[(int)g_pos.y + y_inc][(int)g_pos.x + x_inc] == '|' || map[(int)g_pos.y + y_inc][(int)g_pos.x + x_inc] == '-' || (x_inc == 0 && y_inc == 0))) {
            t_pos.x = (int)g_pos.x + x_inc;
            t_pos.y = (int)g_pos.y + y_inc;
            a_dir = (int)distances[i][0];
            break;
        }
    }
}

void Ghost::calc_target_fright(char map[32][30]) {
    //checar se foi para um dos cantos da tela
    if (g_pos.x == 0) {
        g_pos.x = 29;
        t_pos.x = 29;
    }
    else if (g_pos.x == 29) {
        g_pos.x = 0;
        t_pos.x = 0;
    }

    t_dir = rand() % 4 + 1;
    while (1) {
        if (t_dir == 2) {
            if (map[(int)t_pos.y - 1][(int)t_pos.x] != '|' && a_dir != 4) {
                t_pos.y -= 1;
                printf("\n%d %d", t_dir, a_dir);
                a_dir = t_dir;
                return;
            }
            else t_dir = 1;
        }
        if (t_dir == 1) {
            if (map[(int)t_pos.y][(int)t_pos.x - 1] != '|' && a_dir != 3) {
                t_pos.x -= 1;
                printf("\n%d %d", t_dir, a_dir);
                a_dir = t_dir;
                return;
            }
            else t_dir = 4;
        }
        if (t_dir == 4) {
            if (map[(int)t_pos.y + 1][(int)t_pos.x] != '|' && map[(int)t_pos.y + 1][(int)t_pos.x] != '-' && a_dir != 2) {
                t_pos.y += 1;
                printf("\n%d %d", t_dir, a_dir);
                a_dir = t_dir;
                return;
            }
            else t_dir = 3;
        }
        if (t_dir == 3) {
            if (map[(int)t_pos.y][(int)t_pos.x + 1] != '|' && a_dir != 1) {
                t_pos.x += 1;
                printf("\n%d %d", t_dir, a_dir);
                a_dir = t_dir;
                return;
            }
            else {
                t_dir = 2;
            }
        }
    }
}

void Ghost::calc_target_scatter(char map[32][30]) {
    calc_target_target(map, s_pos);
}

void Ghost::calc_target_chase(char map[32][30], Pacman *pac) {
    calc_target_target(map, pac->get_position());
}

void Ghost::calc_target_eaten(char map[32][30]) {
    calc_target_fright(map);
}

void Ghost::calc_target(char map[32][30], Pacman *pac) {
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

void Ghost::collide_pacman(Pacman *pac) {
    Vector2 p_pos = pac->get_position();
    if (t_pos.x == p_pos.x && t_pos.y == p_pos.y) {
        printf("\nACERTOU");
        switch (state) {
            case 0:
            pac->set_state(-1);
            break;

            case 1:
            pac->set_state(-1);
            break;

            case 2:
            state = 3;
            break;

            default:
            break;
        }
    }
}

void Ghost::move(char map[32][30], Pacman *pac) {
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

    //printf("\n%lf %lf %d %d", g_pos.x, g_pos.y, t_pos.x, t_pos.y);

    if (g_pos.x == t_pos.x && g_pos.y == t_pos.y) {
        calc_target(map, pac);
        collide_pacman(pac);
    }
}

