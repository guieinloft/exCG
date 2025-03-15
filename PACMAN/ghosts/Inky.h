#ifndef INKY_H
#define INKY_H

#include "Ghost.h"

class Inky : public Ghost {
    public:
    Inky(int init_x, int init_y, int scatter_x, int scatter_y) : Ghost(init_x, init_y, scatter_x, scatter_y, 0, 1, 1){}
    void move(char map[32][30], Pacman *pac, Ghost *ghost);
    
    //private:
    void calc_target(char map[32][30], Pacman *pac, Ghost *ghost);
    void calc_target_chase(char map[32][30], Pacman *pac, Ghost *ghost);
};

#endif
