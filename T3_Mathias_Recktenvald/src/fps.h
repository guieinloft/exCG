/*
    Define a classe Fps para controle e limitação da taxa de quadros (frames por segundo) no jogo 2D.
    Gerencia contagem de frames, cálculo do FPS atual e sincronização para manter o limite de FPS.
*/

#ifndef FPS_H
#define FPS_H

#include <stdio.h>
#include <time.h>
#include <thread>
#include <chrono>

class Fps {
private:
    int frameCount;
    clock_t lastTime;
    int fps;
    static constexpr int FPS_LIMIT = 60;
    static constexpr double FRAME_DURATION = 1.0 / FPS_LIMIT;
    clock_t frameStartTime;

public:
    Fps() : frameCount(0), lastTime(0), fps(0), frameStartTime(0) {}

    void frameStart() {
        frameStartTime = clock();
    }

    void update() {
        frameCount++;
        clock_t currentTime = clock();
        if (currentTime - lastTime >= CLOCKS_PER_SEC) {
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    void sleepIfNeeded() {
        clock_t frameEndTime = clock();
        double elapsed = double(frameEndTime - frameStartTime) / CLOCKS_PER_SEC;
        double sleepTime = FRAME_DURATION - elapsed;
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }
    }

    int getFps() const {
        return fps;
    }
};

#endif