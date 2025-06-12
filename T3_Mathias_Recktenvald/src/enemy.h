/*
    Define as classes para inimigos do jogo 2D, incluindo Enemy_a (triângulo) e Enemy_b (pentágono).
    Gerencia movimentação, colisão com curvas, balas e tanque, vida, renderização, disparos (Enemy_a) e boost de velocidade (Enemy_b).

    Classes:
    --------
    - Enemy_a: Inimigo triangular que persegue o jogador, atira, possui vida e verifica colisões.
    - Enemy_b: Inimigo pentagonal que persegue o jogador, possui boost de velocidade temporário e verifica colisões.
*/

#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <ctime>
#include "Vector2.h"
#include "bullet.h"
#include "gl_canvas2d.h"

class Enemy_a {
    int size;
    float speed;
    Vector2 position;
    float health;
    float max_health;
    bool isAlive = true;
    std::vector<Vector2> vertices;

    std::vector<Bullet> bullets; 
    clock_t lastShotTime; 
    const float shootInterval = 2.0f;

    void updateVertices() {
        vertices.clear();
        float halfSize = size / 2.0f;
        float height = size * sqrt(3) / 2.0f;

        vertices.push_back(Vector2(position.x, position.y + height / 2.0f));
        vertices.push_back(Vector2(position.x - halfSize, position.y - height / 2.0f));
        vertices.push_back(Vector2(position.x + halfSize, position.y - height / 2.0f));
    }

public:
    Enemy_a(float x, float y, int s, float spd, float mh) : position(x, y), size(s), speed(spd), max_health(mh), lastShotTime(0) {
        health = max_health;
        updateVertices();
    }

    void moveTowards(const Vector2& target, const Curve* curve = nullptr) {
        if (!isAlive) return; // Prevent movement if dead
        Vector2 targetCopy = target;
        Vector2 direction = targetCopy - position;
        direction.normalize();
        float nextX = position.x + direction.x * speed;
        float nextY = position.y + direction.y * speed;

        // Predict next vertices
        float halfSize = size / 2.0f;
        float height = size * sqrt(3) / 2.0f;
        std::vector<Vector2> nextVertices = {
            Vector2(nextX, nextY + height / 2.0f),
            Vector2(nextX - halfSize, nextY - height / 2.0f),
            Vector2(nextX + halfSize, nextY - height / 2.0f)
        };

        bool collides = false;
        if (curve) {
            float collisionRadius = 8.0f;
            for (int i = 0; i < 3; ++i) {
                const Vector2& v1 = nextVertices[i];
                const Vector2& v2 = nextVertices[(i + 1) % 3];
                if (curve->isColliding(v1, v2, collisionRadius)) {
                    collides = true;
                    break;
                }
            }
        }

        if (!collides) {
            position.x = nextX;
            position.y = nextY;
            updateVertices();
        }
    }

    // Accepts two curves for collision (e.g., inside and outside)
    void moveTowards(const Vector2& target, const Curve* curve1 = nullptr, const Curve* curve2 = nullptr, float tankSize = 0) {
        if (!isAlive) return; // Prevent movement if dead
        // Stop if colliding with tank
        if (tankSize > 0 && checkCollisionWithTank(target, tankSize)) {
            return;
        }
        Vector2 targetCopy = target;
        Vector2 direction = targetCopy - position;
        direction.normalize();
        float nextX = position.x + direction.x * speed;
        float nextY = position.y + direction.y * speed;

        // Predict next vertices
        float halfSize = size / 2.0f;
        float height = size * sqrt(3) / 2.0f;
        std::vector<Vector2> nextVertices = {
            Vector2(nextX, nextY + height / 2.0f),
            Vector2(nextX - halfSize, nextY - height / 2.0f),
            Vector2(nextX + halfSize, nextY - height / 2.0f)
        };

        bool collides = false;
        float collisionRadius = 8.0f;
        for (int i = 0; i < 3; ++i) {
            const Vector2& v1 = nextVertices[i];
            const Vector2& v2 = nextVertices[(i + 1) % 3];
            if ((curve1 && curve1->isColliding(v1, v2, collisionRadius)) ||
                (curve2 && curve2->isColliding(v1, v2, collisionRadius))) {
                collides = true;
                break;
            }
        }

        if (!collides) {
            position.x = nextX;
            position.y = nextY;
            updateVertices();
        }
    }

    void shootAt(const Vector2& target) {
        if (!isAlive) return; // Prevent shooting if dead
        Vector2 targetCopy = target;
        clock_t currentTime = clock();
        float elapsedTime = static_cast<float>(currentTime - lastShotTime) / CLOCKS_PER_SEC;

        if (elapsedTime >= shootInterval) {
            Vector2 direction = targetCopy - position;
            direction.normalize();

            float angle = atan2(direction.y, direction.x);
            bullets.emplace_back(position.x, position.y, 10, angle);
            lastShotTime = currentTime;
        }
    }

    void render(Vector2 playerPosition) {
        if (!isAlive) return;
        float halfSize = size / 2.0f;
        float height = size * sqrt(3) / 2.0f;

        float verticesX[3] = {
            position.x,
            position.x - halfSize,
            position.x + halfSize
        };
        float verticesY[3] = {
            position.y + height / 2.0f,
            position.y - height / 2.0f,
            position.y - height / 2.0f
        };

        CV::color(1, 0, 0);
        CV::polygonFill(verticesX, verticesY, 3);

        float barWidth = size;
        float barHeight = 5;
        float healthRatio = static_cast<float>(health) / max_health;

        CV::color(0, 1, 0);
        CV::rectFill(position.x - barWidth / 2, position.y + height / 2.0f + 5, position.x - barWidth / 2 + barWidth * healthRatio, position.y + height / 2.0f + 5 + barHeight);

        CV::color(1, 0, 0);
        CV::rectFill(position.x - barWidth / 2 + barWidth * healthRatio, position.y + height / 2.0f + 5, position.x + barWidth / 2, position.y + height / 2.0f + 5 + barHeight);

        for (auto& bullet : bullets) {
            if (bullet.getPosition().x != -1 && bullet.getPosition().y != -1) {
                bullet.render(true);
            }
        }
    }

    void checkCollisionWithBullets(std::vector<Bullet>& tankBullets, int* score) {
        if (!isAlive) return;
        for (auto it = tankBullets.begin(); it != tankBullets.end(); ) {
            Vector2 bulletPos = it->getPosition();

            float halfSize = size / 2.0f;
            if (bulletPos.x >= position.x - halfSize && bulletPos.x <= position.x + halfSize &&
                bulletPos.y >= position.y - halfSize && bulletPos.y <= position.y + halfSize) {

                health -= 20;

                it->destroy();
                it = tankBullets.erase(it);

                if (health <= 0) {
                    *score += 10;
                    isAlive = false; 
                    return;
                }
            } else {
                ++it;
            }
        }
    }

    // Check collision with the tank (circle-circle collision)
    bool checkCollisionWithTank(const Vector2& tankPos, float tankSize) const {
        float enemyRadius = size / 2.0f;
        float tankRadius = tankSize / 2.0f;
        float dx = position.x - tankPos.x;
        float dy = position.y - tankPos.y;
        float distSq = dx * dx + dy * dy;
        float minDist = enemyRadius + tankRadius;
        return distSq < minDist * minDist;
    }

    Vector2 getPosition() const {
        return position;
    }

    void setPosition(Vector2 pos) {
        position.x = pos.x;
        position.y = pos.y;
    }

    std::vector<Bullet>& getBullets() {
        return bullets;
    }

    bool getIsAlive() const {
        return isAlive;
    }

    int getSize() const {
        return size;
    }
};

class Enemy_b {
    int size;
    float speed;
    Vector2 position;
    float health;
    float max_health;
    bool isAlive = true;
    std::vector<Vector2> vertices;

    time_t spawnTime;
    const float boostInterval = 5.0f;
    const float boostDuration = 0.5f;

    void updateVertices() {
        vertices.clear();
        float r = size / 2.0f;
        float cx = position.x;
        float cy = position.y;
        for (int i = 0; i < 5; ++i) {
            float angle = PI / 2 + i * 2 * PI / 5;
            float x = cx + r * cos(angle);
            float y = cy + r * sin(angle);
            vertices.push_back(Vector2(x, y));
        }
    }

public:
    Enemy_b(float x, float y, int s, float spd, float mh) : position(x, y), size(s), speed(spd), max_health(mh) {
        health = max_health;
        updateVertices();
        spawnTime = time(0);
    }

    void moveTowards(const Vector2& target, const Curve* curve = nullptr) {
        if (!isAlive) return;
        Vector2 targetCopy = target;
        Vector2 direction = targetCopy - position;
        direction.normalize();
        float nextX = position.x + direction.x * speed;
        float nextY = position.y + direction.y * speed;

        float halfSize = size / 2.0f;
        float height = size * sqrt(3) / 2.0f;
        std::vector<Vector2> nextVertices = {
            Vector2(nextX, nextY + height / 2.0f),
            Vector2(nextX - halfSize, nextY - height / 2.0f),
            Vector2(nextX + halfSize, nextY - height / 2.0f)
        };

        bool collides = false;
        if (curve) {
            float collisionRadius = 8.0f;
            for (int i = 0; i < 3; ++i) {
                const Vector2& v1 = nextVertices[i];
                const Vector2& v2 = nextVertices[(i + 1) % 3];
                if (curve->isColliding(v1, v2, collisionRadius)) {
                    collides = true;
                    break;
                }
            }
        }

        if (!collides) {
            position.x = nextX;
            position.y = nextY;
            updateVertices();
        }
    }

    void moveTowards(const Vector2& target, const Curve* curve1 = nullptr, const Curve* curve2 = nullptr, float tankSize = 0) {
        if (!isAlive) return;
        if (tankSize > 0 && checkCollisionWithTank(target, tankSize)) {
            return;
        }
        Vector2 targetCopy = target;
        Vector2 direction = targetCopy - position;
        direction.normalize();
        float nextX = position.x + direction.x * speed;
        float nextY = position.y + direction.y * speed;

        float halfSize = size / 2.0f;
        float height = size * sqrt(3) / 2.0f;
        std::vector<Vector2> nextVertices = {
            Vector2(nextX, nextY + height / 2.0f),
            Vector2(nextX - halfSize, nextY - height / 2.0f),
            Vector2(nextX + halfSize, nextY - height / 2.0f)
        };

        bool collides = false;
        float collisionRadius = 8.0f;
        for (int i = 0; i < 3; ++i) {
            const Vector2& v1 = nextVertices[i];
            const Vector2& v2 = nextVertices[(i + 1) % 3];
            if ((curve1 && curve1->isColliding(v1, v2, collisionRadius)) ||
                (curve2 && curve2->isColliding(v1, v2, collisionRadius))) {
                collides = true;
                break;
            }
        }

        if (!collides) {
            position.x = nextX;
            position.y = nextY;
            updateVertices();
        }
    }

    void render(Vector2 playerPosition) {
        if (!isAlive) return;
        float verticesX[5];
        float verticesY[5];
        for (int i = 0; i < 5; ++i) {
            verticesX[i] = vertices[i].x;
            verticesY[i] = vertices[i].y;
        }
        CV::color(0, 1, 1);
        CV::polygonFill(verticesX, verticesY, 5);
        float barWidth = size;
        float barHeight = 5;
        float healthRatio = static_cast<float>(health) / max_health;
        CV::color(0, 1, 0);
        CV::rectFill(position.x - barWidth / 2, position.y + size / 2.0f + 5, position.x - barWidth / 2 + barWidth * healthRatio, position.y + size / 2.0f + 5 + barHeight);
        CV::color(1, 0, 0);
        CV::rectFill(position.x - barWidth / 2 + barWidth * healthRatio, position.y + size / 2.0f + 5, position.x + barWidth / 2, position.y + size / 2.0f + 5 + barHeight);
    }

    void checkCollisionWithBullets(std::vector<Bullet>& tankBullets, int* score) {
        if (!isAlive) return;
        for (auto it = tankBullets.begin(); it != tankBullets.end(); ) {
            Vector2 bulletPos = it->getPosition();

            float halfSize = size / 2.0f;
            if (bulletPos.x >= position.x - halfSize && bulletPos.x <= position.x + halfSize &&
                bulletPos.y >= position.y - halfSize && bulletPos.y <= position.y + halfSize) {

                health -= 20;

                it->destroy();
                it = tankBullets.erase(it);

                if (health <= 0) {
                    *score += 15;
                    isAlive = false;
                    return;
                }
            } else {
                ++it;
            }
        }
    }

    bool checkCollisionWithTank(const Vector2& tankPos, float tankSize) const {
        float enemyRadius = size / 2.0f;
        float tankRadius = tankSize / 2.0f;
        float dx = position.x - tankPos.x;
        float dy = position.y - tankPos.y;
        float distSq = dx * dx + dy * dy;
        float minDist = enemyRadius + tankRadius;
        return distSq < minDist * minDist;
    }

    void boost() {
        time_t currentTime = time(0);
        if (difftime(currentTime, spawnTime) >= boostInterval) {
            speed = 2.5f;
            spawnTime = currentTime;
        }
        if (difftime(currentTime, spawnTime) >= boostDuration) {
            speed = 0.5f;
        }
    }

    Vector2 getPosition() const {
        return position;
    }

    void setPosition(Vector2 pos) {
        position.x = pos.x;
        position.y = pos.y;
    }

    bool getIsAlive() const {
        return isAlive;
    }

    int getSize() const {
        return size;
    }
};

#endif