#ifndef PACMAN_H
#define PACMAN_H

#include "gl_canvas2d.h"
#include "Vector2.h"

#define INVINCIBILITY 60

class Pacman {
private:
    Vector2 r_pos;
    Vector2 g_pos;
    Vector2 t_pos;
    int t_dir, a_dir;
    int score, ghost_points;
    void collide(char map[32][30]);
    void collide_adir(char map[32][30]);
    int super, state, lives, invincibility;
    int angle, offset;

public:
    Pacman(int init_x, int init_y);
    void pac_render();
    void handle_input(int key);
    void move(char map[32][30]);
    int get_score();
    void set_super(int s);
    int get_super();
    Vector2 get_position();
    int get_direction();
    void set_state(int s);
    int get_state();
    void set_invincibility(int i);
    int get_invincibility();
    int get_lives();
    void reset(int init_x, int init_y, bool reset_points);
    void ghost_capture();
};

#endif
