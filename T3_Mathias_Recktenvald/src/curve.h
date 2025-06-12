/*
    Classe Curve para curvas fechadas do tipo B-Spline no jogo 2D.
    Gerencia pontos de controle, renderização, fechamento, colisão, testes de inclusão e geração de posições seguras entre curvas.
*/

#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <string>
#include "gl_canvas2d.h"
#include <random>

class Curve {
private:
    std::vector<std::pair<int, int>> controlPoints;
    bool closed = false;

public:
    void addPoint(int x, int y) {
        if (!closed) {
            controlPoints.emplace_back(x, SCREEN_HEIGHT - y);
        }
    }

    void removeLastPoint() {
        if (!controlPoints.empty()) {
            controlPoints.pop_back();
        }
    }

    void closeCurve() {
        if (controlPoints.size() >= 2) {
            closed = true;
        }
    }

    const std::vector<std::pair<int, int>>& getControlPoints() const {
        return controlPoints;
    }

    void render() {
        int n = controlPoints.size();
        if (n < 4) return;

        CV::color(200/255.0, 200/255.0, 200/255.0);

        for (int i = 0; i < n; ++i) {
            const auto& p0 = controlPoints[i % n];
            const auto& p1 = controlPoints[(i + 1) % n];
            const auto& p2 = controlPoints[(i + 2) % n];
            const auto& p3 = controlPoints[(i + 3) % n];

            drawBSplineSegment(p0, p1, p2, p3);
        }
    }

    // Desenha um segmento da curva B-Spline
    void drawBSplineSegment(const std::pair<int, int>& p0,
                            const std::pair<int, int>& p1,
                            const std::pair<int, int>& p2,
                            const std::pair<int, int>& p3) {
        float prevX = 0, prevY = 0;
        bool first = true;

        for (float t = 0; t <= 1.0f; t += 0.01f) {
            float t2 = t * t;
            float t3 = t2 * t;

            float b0 = (1 - t) * (1 - t) * (1 - t) / 6.0f;
            float b1 = (3 * t3 - 6 * t2 + 4) / 6.0f;
            float b2 = (-3 * t3 + 3 * t2 + 3 * t + 1) / 6.0f;
            float b3 = t3 / 6.0f;

            float x = b0 * p0.first + b1 * p1.first + b2 * p2.first + b3 * p3.first;
            float y = b0 * p0.second + b1 * p1.second + b2 * p2.second + b3 * p3.second;

            if (!first) {
                CV::line(prevX, prevY, x, y);
            } else {
                first = false;
            }

            prevX = x;
            prevY = y;
        }
    }

    // Exibe instruções e renderiza a curva durante a definição dos pontos
    void defineCurve(const std::string& curveName) {
        CV::color(1, 1, 1);
        std::string message = "Desenhe a " + curveName + ". Clique para  com o botao esquerdo para adicionar pontos e direito para remover.";
        CV::text(10, 30, message.c_str());
        message = "Pressione ENTER para terminar.";
        CV::text(10, 10, message.c_str());

        render(); 

        for (const auto& point : controlPoints) {
            CV::color(44 / 255.0, 203 / 255.0, 222 / 255.0);
            CV::circleFill(point.first, point.second, 5, 10);
        }
    }

    // Verifica colisão de um ponto (com raio) com a curva
    bool isColliding(const Vector2& point, float radius) const {
        if (!closed || controlPoints.size() < 4) {
            return false;
        }

        int n = controlPoints.size();

        for (int i = 0; i < n; ++i) {
            const auto& p0 = controlPoints[i % n];
            const auto& p1 = controlPoints[(i + 1) % n];
            const auto& p2 = controlPoints[(i + 2) % n];
            const auto& p3 = controlPoints[(i + 3) % n];

            for (float t = 0; t <= 1.0f; t += 0.01f) {
                float t2 = t * t;
                float t3 = t2 * t;

                float b0 = (1 - t) * (1 - t) * (1 - t) / 6.0f;
                float b1 = (3 * t3 - 6 * t2 + 4) / 6.0f;
                float b2 = (-3 * t3 + 3 * t2 + 3 * t + 1) / 6.0f;
                float b3 = t3 / 6.0f;

                float x = b0 * p0.first + b1 * p1.first + b2 * p2.first + b3 * p3.first;
                float y = b0 * p0.second + b1 * p1.second + b2 * p2.second + b3 * p3.second;

                float effectiveRadius = (radius > 3.0f) ? (radius - 3.0f) : radius;
                float distanceSquared = (point.x - x) * (point.x - x) + (point.y - y) * (point.y - y);

                if (distanceSquared <= effectiveRadius * effectiveRadius) {
                    return true; 
                }
            }
        }

        return false;
    }

    // Verifica colisão de um segmento de reta com a curva
    bool isColliding(const Vector2& p1, const Vector2& p2, float radius) const {
        const int steps = 10;
        for (int i = 0; i <= steps; ++i) {
            float t = i / (float)steps;
            Vector2 pt(
                p1.x + (p2.x - p1.x) * t,
                p1.y + (p2.y - p1.y) * t
            );
            if (isColliding(pt, radius)) {
                return true;
            }
        }
        return false;
    }

    bool isClosed() const {
        return closed;
    }

    // Testa se um ponto está dentro da curva fechada (algoritmo do raio
    static bool isPointInsideCurve(const Curve& curve, float x, float y) {
        const auto& pts = curve.getControlPoints();
        int n = pts.size();
        if (!curve.isClosed() || n < 3) return false;
        bool inside = false;
        for (int i = 0, j = n - 1; i < n; j = i++) {
            float xi = pts[i].first, yi = pts[i].second;
            float xj = pts[j].first, yj = pts[j].second;
            if (((yi > y) != (yj > y)) &&
                (x < (xj - xi) * (y - yi) / (yj - yi + 1e-6f) + xi)) {
                inside = !inside;
            }
        }
        return inside;
    }

    // Gera uma posição segura entre duas curvas para spawn
    static Vector2 generateSpawn(const Curve& inner, const Curve& outer, float size, float margin) {
        if (!inner.isClosed() || !outer.isClosed()) {
            return Vector2(0, 0);
        }

        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<float> disX(margin / 4.0f, SCREEN_WIDTH - size - margin / 4.0f);
        std::uniform_real_distribution<float> disY(margin / 4.0f, SCREEN_HEIGHT - size - margin / 4.0f);

        Vector2 spawn;
        int maxTries = 20000;
        int tries = 0;
        do {
            spawn.x = disX(gen);
            spawn.y = disY(gen);

            float half = size / 2.0f;
            float offset = margin / 4.0f;
            float cx = spawn.x + half;
            float cy = spawn.y + half;

            float vx[4] = { spawn.x - offset, spawn.x + size + offset, spawn.x + size + offset, spawn.x - offset };
            float vy[4] = { spawn.y - offset, spawn.y - offset, spawn.y + size + offset, spawn.y + size + offset };

            bool valid = true;
            for (int i = 0; i < 4; ++i) {
                if (!Curve::isPointInsideCurve(outer, vx[i], vy[i]) || Curve::isPointInsideCurve(inner, vx[i], vy[i])) {
                    valid = false;
                    break;
                }
            }

            if (valid && (!Curve::isPointInsideCurve(outer, cx, cy) || Curve::isPointInsideCurve(inner, cx, cy))) {
                valid = false;
            }

            if (valid) {
                return Vector2(spawn.x, spawn.y);
            }

            ++tries;
        } while (tries < maxTries);

        return Vector2(0, 0);
    }

    // Testa se um ponto está entre duas curvas (sem margem)
    static bool isPointBetweenCurves(const Curve& inner, const Curve& outer, float x, float y) {
        return isPointInsideCurve(outer, x, y) && !isPointInsideCurve(inner, x, y);
    }

    // Verifica colisão de balas com curvas e destrói as balas que colidirem
    static void checkBulletsCurveCollision(std::vector<Bullet>& bullets, const Curve& curve1, const Curve& curve2) {
        if (!curve1.isClosed() || !curve2.isClosed()) return;
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            Vector2 bulletPos = it->getPosition();
            if ((curve1.isColliding(bulletPos, 8.0f)) || (curve2.isColliding(bulletPos, 8.0f))) {
                it->destroy();
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }
};

#endif