#ifndef GHOST_H
#define GHOST_H

#include "../Pacman.h"
#include "../Vector2.h"

#define GHOST_SPEED 0.0625

class Ghost {
    public:
    Ghost(int init_x, int init_y, int scatter_x, int scatter_y);
    void Render();
    void move(char map[32][30], Pacman *pac);

    protected:
    Vector2 r_pos;
    Vector2 g_pos;
    Vector2 t_pos;
    Vector2 s_pos;
    int t_dir, a_dir, state;
    void calc_target(char map[32][30], Pacman *pac);
    void calc_target_target(char map[32][30], Vector2 pos);
    void calc_target_chase(char map[32][30], Pacman *pac);
    void calc_target_scatter(char map[32][30]);
    void calc_target_fright(char map[32][30]);
    void calc_target_eaten(char map[32][30]);
    void collide_pacman(Pacman *pac);
};

#endif
