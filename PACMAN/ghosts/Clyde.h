#ifndef CLYDE_H
#define CLYDE_H

#include "Ghost.h"

class Clyde : public Ghost {
    public:
    Clyde(int init_x, int init_y, int scatter_x, int scatter_y) : Ghost(init_x, init_y, scatter_x, scatter_y, 1, 0.6, 0){}
    void move(char map[32][30], Pacman *pac);
    
    //private:
    void calc_target(char map[32][30], Pacman *pac);
    void calc_target_chase(char map[32][30], Pacman *pac);
};

#endif
