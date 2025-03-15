#ifndef PACMAN_H
#define PACMAN_H

#include "gl_canvas2d.h"
#include "Vector2.h"

class Pacman {
private:
    Vector2 r_pos;
    Vector2 g_pos;
    Vector2 t_pos;
    int t_dir, a_dir;
    int score;
    void collide(char map[32][30]);
    void collide_adir(char map[32][30]);
    int super;
    int state;
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
};

#endif
