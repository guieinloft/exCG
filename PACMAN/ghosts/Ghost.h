#ifndef GHOST_H
#define GHOST_H

#include "../Pacman.h"
#include "../Vector2.h"

#define GHOST_SPEED 0.0625

class Ghost {
    public:
    Ghost(int init_x, int init_y, int scatter_x, int scatter_y, float rr, float gg, float bb);
    void Render();
    void move(char map[32][30], Pacman *pac);
    Vector2 get_position();
    void set_times(int t1, int t2, int t3, int t4, int t5, int t6, int t7);
    void set_state(int s);
    void reset(int init_x, int init_y);

    protected:
    Vector2 r_pos;
    Vector2 g_pos;
    Vector2 t_pos;
    Vector2 s_pos;
    float r, g, b, speed;
    int t_dir, a_dir, state, offset;
    int timers[7], timer_i;
    void calc_target(char map[32][30], Pacman *pac);
    void calc_target_target(char map[32][30], Vector2 pos);
    void calc_target_chase(char map[32][30], Pacman *pac);
    void calc_target_scatter(char map[32][30]);
    void calc_target_fright(char map[32][30]);
    void calc_target_eaten(char map[32][30]);
    void calc_target_ghost_house(char map[32][30]);
    void collide_pacman(Pacman *pac);
    void RenderTop();
    void RenderBottom();
    void RenderEyes();
};

#endif
