/*
    Define as classes para o tanque do jogo 2D, incluindo corpo (Body), canhão (Turret) e tanque completo (Tank).
    Gerencia movimentação, disparos, colisões, power-ups e renderização gráfica.

    Classes:
    --------
    - Body: Controla posição, vida, escudo, velocidade e renderização do corpo do tanque.
    - Turret: Controla o canhão, ângulo, disparos e renderização do canhão/balas.
    - Tank: Integra corpo e canhão, gerencia colisões, power-ups e renderização do tanque.
*/

#ifndef TANK_H
#define TANK_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "bullet.h"
#include "curve.h"

#define MAX_HEALTH 100

class Body {
    Vector2 position;
    int width, height, speed;
    float angle;
    float health = MAX_HEALTH;
    bool isAlive = true;

    bool shield = false;
    time_t shieldTime = 0;
    float shieldDuration = 8.0;

    bool speedBoost = false;
    time_t speedBoostTime = 0;
    float speedBoostDuration = 8.0;

    public:
        Body(float x, float y, int w, int h, int s) : position(x, y), width(w), height(h), speed(s), angle(0) {}

        void setPosition(float x, float y) {
            position.set(x, y);
        }

        Vector2 getPosition() const {
            return position;
        }

        int getWidth() const {
            return width;
        }

        int getHeight() const {
            return height;
        }

        int getSpeed() const {
            return speed;
        }

        void setSpeed(int s) {
            speed = s;
        }

        void setAngle(float a) {
            angle = a;
        }

        float getAngle() const {
            return angle;
        }

        bool getIsAlive() const {
            return isAlive;
        }

        void move() {
            position.x += speed * cos(angle);
            position.y += speed * sin(angle);
        }

        void checkCollisionWithBullets(std::vector<Bullet>& bullets) {
            for (auto& bullet : bullets) {
                if (!bullet.isActive()) continue;
                Vector2 bpos = bullet.getPosition();
                if (bpos.x == -1 && bpos.y == -1) continue;
                float dx = position.x - bpos.x;
                float dy = position.y - bpos.y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < width / 2.0) {
                    if (shield) {
                        bullet.destroy();
                        continue;
                    }
                    health -= 10;
                    bullet.destroy();
                }

                if (health <= 0) {
                    isAlive = false;
                    return;
                }
            }
        }

        void collisionDamage(float damage) {
            if (shield) {
                damage /= 3;
            }
            health -= damage;
            if (health <= 0) {
                isAlive = false;
            }
        }

        void activateShield() {
            shield = true;
            shieldTime = time(0);
        }

        void updateShield() {
            if (shield) {
                time_t currentTime = time(0);
                if (difftime(currentTime, shieldTime) >= shieldDuration) {
                    shield = false;
                }
            }
        }

        void activateSpeedBoost() {
            speed = 3;
            speedBoost = true;
            speedBoostTime = time(0);
        }

        void updateSpeedBoost() {
            if (speedBoost) {
                time_t currentTime = time(0);
                if (difftime(currentTime, speedBoostTime) >= speedBoostDuration) {
                    speedBoost = false;
                    speed = 1.5;
                }
            }
        }

        void healthPowerUp() {
            health += 30;
            if (health > MAX_HEALTH) {
                health = MAX_HEALTH;
            }
        }

        void renderBody(float halfWidth, float halfHeight) {
            Vector2 vertices[4] = {
                {-halfWidth, -halfHeight},
                { halfWidth, -halfHeight},
                { halfWidth,  halfHeight},
                {-halfWidth,  halfHeight}
            };

            float vx[4], vy[4];

            for (int i = 0; i < 4; i++) {
                float x = vertices[i].x;
                float y = vertices[i].y;

                vx[i] = x * cos(angle) - y * sin(angle) + position.x;
                vy[i] = x * sin(angle) + y * cos(angle) + position.y;
            }

            CV::color(0.5, 0.5, 0.5);
            CV::polygonFill(vx, vy, 4);
        }

        void renderArrow(float halfWidth, float halfHeight) {
            float arrowLength = halfHeight + 10;
            float arrowWidth = halfWidth * 0.6f;
            float frontX = position.x + arrowLength * cos(angle);
            float frontY = position.y + arrowLength * sin(angle);
            float leftBaseX = position.x + (halfHeight - 5) * cos(angle) - arrowWidth * sin(angle);
            float leftBaseY = position.y + (halfHeight - 5) * sin(angle) + arrowWidth * cos(angle);
            float rightBaseX = position.x + (halfHeight - 5) * cos(angle) + arrowWidth * sin(angle);
            float rightBaseY = position.y + (halfHeight - 5) * sin(angle) - arrowWidth * cos(angle);
            float triX[3] = { frontX, leftBaseX, rightBaseX };
            float triY[3] = { frontY, leftBaseY, rightBaseY };
            CV::color(200/255.0, 200/255.0, 200/255.0);
            if (speedBoost) CV::color(0, 1, 0.5);
            CV::polygonFill(triX, triY, 3);
        }

        void renderHealthBar(float halfWidth, float halfHeight) {
            float barWidth = width;
            float barHeight = 5;
            float healthRatio = health / 100.0f;

            CV::color(0, 1, 0);
            CV::rectFill(position.x - barWidth / 2, position.y - halfHeight - 10, position.x - barWidth / 2 + barWidth * healthRatio, position.y - halfHeight - 10 + barHeight);

            CV::color(1, 0, 0);
            CV::rectFill(position.x - barWidth / 2 + barWidth * healthRatio, position.y - halfHeight - 10, position.x + barWidth / 2, position.y - halfHeight - 10 + barHeight);
        }

        void renderShield(float halfWidth, float halfHeight) {
            float shieldRadius = halfWidth * 1.75f;
            CV::color(0, 0, 1, 0.5);
            CV::circle(position.x, position.y, shieldRadius, 50);
        }

        void render() {
            float halfWidth = width / 2.0;
            float halfHeight = height / 2.0;

            renderBody(halfWidth, halfHeight);
            if (shield) renderShield(halfWidth, halfHeight);
            renderArrow(halfWidth, halfHeight);
            renderHealthBar(halfWidth, halfHeight);
        }
};

class Turret {
    Vector2 position;
    float angle;
    int height;
    std::vector<Bullet> bullets;
    clock_t lastShotTime;
    float cooldown = 0.5;
    bool shootAmp = false;
    float shootAmpDuration = 4.0;
    time_t shootAmpStartTime = 0;

public:
    Turret(float x, float y, int h) : position(x, y), angle(0), height(h), lastShotTime(0) {}

    void setAngle(float a) {
        angle = a;
    }

    float getAngle() const {
        return angle;
    }

    void shoot(Vector2 tankPosition) {
        clock_t currentTime = clock();
        float elapsedTime = static_cast<float>(currentTime - lastShotTime) / CLOCKS_PER_SEC;

        if (elapsedTime >= cooldown) {
            float endX = tankPosition.x + position.x + height * cos(angle);
            float endY = tankPosition.y + position.y + height * sin(angle);

            bullets.emplace_back(endX, endY, 10, angle);
            lastShotTime = currentTime;
        }
    }

    void shootAmpActivate() {
        shootAmp = true;
        cooldown = 0.1;
        shootAmpStartTime = time(0);
    }

    void shootAmpUpdate() {
        if (shootAmp) {
            time_t currentTime = time(0);
            if (difftime(currentTime, shootAmpStartTime) >= shootAmpDuration) {
                shootAmp = false;
                cooldown = 0.5;
            }
        }
    }

    std::vector<Bullet>& getBullets() {
        return bullets;
    }

    void render(const Vector2& tankPosition) {
        float turretLength = height;
        float turretWidth = 10.0;
        float angleRad = angle;

        float halfWidth = turretWidth / 2.0f;
        float baseX = tankPosition.x;
        float baseY = tankPosition.y;
        float endX = baseX + turretLength * cos(angleRad);
        float endY = baseY + turretLength * sin(angleRad);

        float perpX = -sin(angleRad) * halfWidth;
        float perpY = cos(angleRad) * halfWidth;
        float rectX[4] = { baseX + perpX, baseX - perpX, endX - perpX, endX + perpX };
        float rectY[4] = { baseY + perpY, baseY - perpY, endY - perpY, endY + perpY };
        CV::color(0.3, 0.3, 0.3);
        if (shootAmp) {
            CV::color(1, 0.5, 0);
        }
        CV::polygonFill(rectX, rectY, 4);

        for (auto& bullet : bullets) {
            if (bullet.getPosition().x != -1 && bullet.getPosition().y != -1) {
                bullet.render(false);
            }
        }
    }
};

class Tank {
    Body body;
    Turret turret;
    std::vector<Vector2> vertices;
    int reverseFrames = 0;
    int lastReverseCollisionTime = 0; // store last reverse collision time in seconds
    int lastEnemyCollisionTime = 0; // store last enemy collision time in seconds

    void updateVertices() {
        vertices.clear();
        Vector2 tankPosition = body.getPosition();
        float halfWidth = body.getWidth() / 2.0f;
        float halfHeight = body.getHeight() / 2.0f;
        float angle = body.getAngle();

        Vector2 localVerts[4] = {
            {-halfWidth, -halfHeight},
            { halfWidth, -halfHeight},
            { halfWidth,  halfHeight},
            {-halfWidth,  halfHeight}
        };

        for (int i = 0; i < 4; ++i) {
            float x = localVerts[i].x;
            float y = localVerts[i].y;
            float worldX = x * cos(angle) - y * sin(angle) + tankPosition.x;
            float worldY = x * sin(angle) + y * cos(angle) + tankPosition.y;
            vertices.emplace_back(worldX, worldY);
        }
    }

public:
    Tank(float x, float y) : body(x, y, 80, 50, 1.5), turret(0, 0, 60) {
        updateVertices();
    }

    void setPosition(Vector2 pos) {
        body.setPosition(pos.x, pos.y);
        updateVertices();
    }

    Vector2 getPosition() const {
        return body.getPosition();
    }

    void setAngle(float a) {
        body.setAngle(a);
        updateVertices();
    }

    float getAngle() const {
        return body.getAngle();
    }

    void setTurretAngle(float a) {
        turret.setAngle(a);
    }

    float getTurretAngle() const {
        return turret.getAngle();
    }

    bool getIsAlive() const {
        return body.getIsAlive();
    }

    void move() {
        int originalSpeed = body.getSpeed();
        if (reverseFrames > 0) {
            body.setSpeed(-abs(originalSpeed));
            reverseFrames--;
        } else {
            body.setSpeed(abs(originalSpeed));
        }
        body.move();
        updateVertices();
    }

    void shoot() {
        turret.shoot(body.getPosition());
    }

    std::vector<Bullet>& getBullets() {
        return turret.getBullets();
    }

    void activateShield() {
        body.activateShield();
    }

    void updateShield() {
        body.updateShield();
    }

    void activateSpeedBoost() {
        body.activateSpeedBoost();
    }

    void updateSpeedBoost() {
        body.updateSpeedBoost();
    }

    void healthPowerUp() {
        body.healthPowerUp();
    }

    void shootAmpActivate() {
        turret.shootAmpActivate();
    }

    void shootAmpUpdate() {
        turret.shootAmpUpdate();
    }

    void checkCollisionWithBullets(std::vector<Bullet>& bullets) {
        body.checkCollisionWithBullets(bullets);
    }

    void render(int mouseX, int mouseY) {
        body.render();

        float angle = atan2(mouseY - body.getPosition().y, mouseX - body.getPosition().x);
        turret.setAngle(angle);
        turret.render(body.getPosition());
    }

    void checkCollisionWithCurve(const Curve& curve) {
        float collisionRadius = 8.0f;
        if (!curve.isClosed() || curve.getControlPoints().size() < 4) return;
        for (int i = 0; i < 4; ++i) {
            const Vector2& v1 = vertices[i];
            const Vector2& v2 = vertices[(i + 1) % 4];
            if (curve.isColliding(v1, v2, collisionRadius)) {
                int now = (int)time(0);
                if (reverseFrames > 0) {
                    if (now - lastReverseCollisionTime <= 1) {
                        reverseFrames = 0;
                    }
                    lastReverseCollisionTime = now;
                } else {
                    body.collisionDamage(10);
                    reverseFrames = 30;
                    lastReverseCollisionTime = now;
                }
                return;
            }
        }
    }

    void checkCollisionWithEntity(const Vector2& enemyPos, float enemySize) {
        float tankRadius = body.getWidth() / 2.0f;
        float enemyRadius = enemySize / 2.0f;
        float dx = body.getPosition().x - enemyPos.x;
        float dy = body.getPosition().y - enemyPos.y;
        float distSq = dx * dx + dy * dy;
        float minDist = tankRadius + enemyRadius;
        if (distSq < minDist * minDist) {
            reverseFrames = 25;
            int now = (int)time(0);
            if (now - lastEnemyCollisionTime >= 1) {
                body.collisionDamage(10);
                lastEnemyCollisionTime = now;
            }
        }
    }

    bool checkCollisionWithPowerUp(const Vector2& powerUpPos, float powerUpSize, int powerUpType) {
        float tankRadius = body.getWidth() / 2.0f;
        float powerUpRadius = powerUpSize / 2.0f;
        float dx = body.getPosition().x - powerUpPos.x;
        float dy = body.getPosition().y - powerUpPos.y;
        float distSq = dx * dx + dy * dy;
        float minDist = tankRadius + powerUpRadius;
        if (distSq < minDist * minDist) {
            if (powerUpType == 0) {
                body.activateShield();
            } else if (powerUpType == 1) {
                turret.shootAmpActivate();
            } else if (powerUpType == 2) {
                body.healthPowerUp();
            } else if (powerUpType == 3) {
                body.activateSpeedBoost();
            }
            return true;
        }
        return false;
    }
};

#endif
