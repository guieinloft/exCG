/*
    Define a classe Barrel, responsável pelos barris do jogo 2D.
    Gerencia posição, vida, colisão com balas, explosão animada e renderização do barril.
*/

#ifndef BARREL_H
#define BARREL_H

#include "Vector2.h"
#include "gl_canvas2d.h"
#include "bullet.h"
#include <vector>
#include <ctime>

class Barrel {
    int size;
    Vector2 position;
    float health;
    float max_health;
    bool isAlive = true;
    
    // Explosion animation parameters
    bool exploding = false;
    clock_t explosionStartTime;
    float explosionDuration = 1.2f;
    std::vector<Vector2> explosionParticles;
    const int numParticles = 25;
    std::vector<float> particleBaseSize;
    std::vector<float> particleStartSize; 

    void initExplosion() {
        explosionStartTime = clock();
        exploding = true;
        
        explosionParticles.clear();
        particleStartSize.clear();
        particleBaseSize.clear();
        
        for (int i = 0; i < numParticles; i++) {
            float offsetRadius = (size / 2.0f) * ((float)rand() / RAND_MAX); 
            float randomAngle = ((float)rand() / RAND_MAX) * 2 * PI;
            float offsetX = cos(randomAngle) * offsetRadius;
            float offsetY = sin(randomAngle) * offsetRadius;
            
            explosionParticles.push_back(Vector2(position.x + offsetX, position.y + offsetY)); 
            
            particleStartSize.push_back(size / 10.0f); 
            particleBaseSize.push_back(size / 0.75f + (rand() % (int)(size / 2.0f)));
        }
    }

public:
    Barrel(float x, float y, int s, float mh) : position(x, y), size(s), max_health(mh) {
        health = max_health;
    }

    void setPosition(float x, float y) {
        position.set(x, y);
    }

    Vector2 getPosition() const {
        return position;
    }

    int getSize() const {
        return size;
    }

    bool getIsAlive() const {
        return isAlive;
    }

    bool isExploding() const {
        return exploding;
    }

    void checkCollisionWithBullet(std::vector<Bullet>& bullets, int *score) {
        if (!isAlive || exploding) return;
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            Vector2 bulletPos = it->getPosition();

            float halfSize = size / 2.0f;
            if (bulletPos.x >= position.x - halfSize && bulletPos.x <= position.x + halfSize &&
                bulletPos.y >= position.y - halfSize && bulletPos.y <= position.y + halfSize) {

                health -= 20;

                it->destroy();
                it = bullets.erase(it);

                if (health <= 0) {
                    *score += 5;
                    isAlive = false;
                    initExplosion();
                    return;
                }
            } else {
                ++it;
            }
        }
    }    
    
    void updateExplosion() {
        if (!exploding) return;
        
        clock_t currentTime = clock();
        float elapsedTime = static_cast<float>(currentTime - explosionStartTime) / CLOCKS_PER_SEC;
        
        if (elapsedTime >= explosionDuration) {
            exploding = false;
            return;
        }
    }
    
    void renderExplosion() {
        if (!exploding) return;
        
        clock_t currentTime = clock();
        float elapsedTime = static_cast<float>(currentTime - explosionStartTime) / CLOCKS_PER_SEC;
        float progress = elapsedTime / explosionDuration;
        
        for (int i = 0; i < explosionParticles.size(); i++) {
            float currentParticleSize;
            float peakTime = 0.4f; 
            if (progress < peakTime) { 
                currentParticleSize = particleStartSize[i] + (particleBaseSize[i] - particleStartSize[i]) * (progress / peakTime);
            } else { 
                currentParticleSize = particleBaseSize[i] * std::max(0.0f, (1.0f - (progress - peakTime) / (1.0f - peakTime)));
            }
            currentParticleSize = std::max(0.0f, currentParticleSize);

            float r_val, g_val, b_val;

            if (progress < 0.15f) {
                r_val = 1.0f;
                g_val = 1.0f;
                b_val = 0.5f * (1.0f - progress / 0.15f); 
            } else if (progress < 0.4f) {
                float phaseProgress = (progress - 0.15f) / 0.25f;
                r_val = 1.0f;
                g_val = 1.0f - phaseProgress * 0.5f; 
                b_val = 0.0f;
            } else if (progress < 0.75f) { 
                float phaseProgress = (progress - 0.4f) / 0.35f;
                r_val = 1.0f;
                g_val = 0.5f - phaseProgress * 0.4f; 
                b_val = 0.0f;
            } else {
                float phaseProgress = (progress - 0.75f) / 0.25f;
                float smoke_intensity = 1.0f - phaseProgress;
                r_val = 0.5f * smoke_intensity; 
                g_val = 0.15f * smoke_intensity; 
                b_val = 0.05f * smoke_intensity; 
            }
            
            r_val = std::max(0.0f, std::min(1.0f, r_val));
            g_val = std::max(0.0f, std::min(1.0f, g_val));
            b_val = std::max(0.0f, std::min(1.0f, b_val));
            
            CV::color(r_val, g_val, b_val);
            CV::circleFill(explosionParticles[i].x, explosionParticles[i].y, currentParticleSize, 12);
        }
    }

    void render() {
        if (isAlive) {
            CV::color(0.5, 0.5, 0.5);
            CV::circleFill(position.x, position.y, size / 2.0f, 10);
        } else if (exploding) {
            updateExplosion();
            renderExplosion();
        }
    }
};

#endif