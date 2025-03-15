#ifndef BLINKY_H
#define BLINKY_H

#include "Ghost.h"

class Blinky : public Ghost {
    public:
    Blinky(int init_x, int init_y, int scatter_x, int scatter_y) : Ghost(init_x, init_y, scatter_x, scatter_y, 1, 0, 0){}
    void move(char map[32][30], Pacman *pac);
    
    protected:
    void calc_target_chase(char map[32][30], Pacman *pac);
    void calc_target(char map[32][30], Pacman *pac);
};

#endif
