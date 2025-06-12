/*
    Arquivo principal do jogo 2D de tanque.
    Responsável por inicializar o jogo, gerenciar o laço principal, entidades (jogador, inimigos, barris, power-ups), renderização, controle das curvas de limite, entrada do usuário e fluxo geral do jogo.

    Principais responsabilidades:
    ----------------------------
    - Inicialização da janela e contexto gráfico.
    - Controle do fluxo de definição das curvas de jogo (área jogável).
    - Gerenciamento do jogador, inimigos, barris e power-ups: criação, atualização, colisões e remoção.
    - Controle do tempo de jogo, pontuação e condições de vitória/derrota.
    - Processamento de entradas do teclado e mouse para movimentação, disparos e definição das curvas.
    - Renderização de todos os elementos do jogo e das telas de início e fim.
    - Gerenciamento do ranking e pontuação dos jogadores.
*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>

#include "gl_canvas2d.h"

#include "fps.h"
#include "tank.h"
#include "curve.h"
#include "enemy.h"
#include "barrel.h"
#include "power_ups.h"
#include "score.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1920, screenHeight = 1080;

Fps fps;
time_t startTime = time(0);
int score = 0;

Tank player(0, 0);

Curve insideCurve;
Curve outsideCurve;
bool drawingInside = true;
bool drawingOutside = false;
bool curves = false;

std::vector<Enemy_a*> enemies;
std::vector<Enemy_b*> enemies_b;
std::vector<Barrel*> barrels;
std::vector<PowerUp*> powerUps;

ScoreManager scoreManager;
bool showNameInput = false;
std::string playerName = "";
bool scoreRegistered = false;

int mouseX, mouseY;

enum GameScreen {
    GAME,
    GAME_OVER,
    RANKING
};
GameScreen currentScreen = GAME;

void removeBullets(std::vector<Bullet>& bullets) {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isActive(); }
        ),
        bullets.end()
    );
}

void removeDeadEnemies() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->getIsAlive()) {
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = enemies_b.begin(); it != enemies_b.end(); ) {
        if (!(*it)->getIsAlive()) {
            delete *it;
            it = enemies_b.erase(it);
        } else {
            ++it;
        }
    }
}

void removeDeadBarrels() {
    for (auto it = barrels.begin(); it != barrels.end(); ) {
        if (!(*it)->getIsAlive() && !(*it)->isExploding()) {
            delete *it;
            it = barrels.erase(it);
        } else {
            ++it;
        }
    }
}

void removeInactivePowerUps() {
    for (auto it = powerUps.begin(); it != powerUps.end(); ) {
        if (!(*it)->getIsActive()) {
            delete *it;
            it = powerUps.erase(it);
        } else {
            ++it;
        }
    }
}

void startGame() {
    player.setPosition(Curve::generateSpawn(insideCurve, outsideCurve, 80, 160));
    startTime = time(0);
    Vector2 spawn = Curve::generateSpawn(insideCurve, outsideCurve, 40, 80);
    enemies.push_back(new Enemy_a(spawn.x, spawn.y, 40, 0.7, 30.0));
    Vector2 spawn_b = Curve::generateSpawn(insideCurve, outsideCurve, 50, 100);
    enemies_b.push_back(new Enemy_b(spawn_b.x, spawn_b.y, 50, 0.5, 80.0));
}

void resetGameState() {
    for (auto enemy : enemies) delete enemy;
    enemies.clear();
    for (auto enemy : enemies_b) delete enemy;
    enemies_b.clear();
    for (auto barrel : barrels) delete barrel;
    barrels.clear();
    for (auto powerUp : powerUps) delete powerUp;
    powerUps.clear();
    score = 0;
    player = Tank(0, 0);
    scoreRegistered = false;
    playerName = "";
}

void renderBg() {
    CV::color(32 / 255.0, 32 / 255.0, 32 / 255.0);
    CV::rectFill(0, 0, screenWidth, screenHeight);

    insideCurve.render();
    outsideCurve.render();
}

void renderTime() {
    time_t elapsedTime = time(0) - startTime;
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    std::string timeText = "Tempo: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    CV::color(1, 1, 1);
    CV::text(10, screenHeight - 40, timeText.c_str());
    std::string scoreText = "Pontuacao: " + std::to_string(score);
    CV::text(10, screenHeight - 20, scoreText.c_str());
}

void playerLogic() {
    player.move();
    player.render(mouseX, mouseY);
    player.checkCollisionWithCurve(insideCurve);
    player.checkCollisionWithCurve(outsideCurve);

    Curve::checkBulletsCurveCollision(player.getBullets(), insideCurve, outsideCurve);
    removeBullets(player.getBullets());

    player.updateShield();
    player.shootAmpUpdate();
    player.updateSpeedBoost();
}

void enemyLogic() {
    for (auto& enemy : enemies) {
        if (!enemy->getIsAlive()) continue;

        enemy->moveTowards(player.getPosition(), &insideCurve, &outsideCurve, 80);

        enemy->shootAt(player.getPosition());
        player.checkCollisionWithBullets(enemy->getBullets());
        enemy->checkCollisionWithBullets(player.getBullets(), &score);

        Curve::checkBulletsCurveCollision(enemy->getBullets(), insideCurve, outsideCurve);

        enemy->render(player.getPosition());

        player.checkCollisionWithEntity(enemy->getPosition(), enemy->getSize());
        removeBullets(enemy->getBullets());
    }
    for (auto& enemy : enemies_b) {
        if (!enemy->getIsAlive()) continue;

        enemy->moveTowards(player.getPosition(), &insideCurve, &outsideCurve, 80);
        enemy->boost();

        enemy->checkCollisionWithBullets(player.getBullets(), &score);

        enemy->render(player.getPosition());

        player.checkCollisionWithEntity(enemy->getPosition(), enemy->getSize());
    }
}

void barrelLogic() {
    for (auto& barrel : barrels) {
        barrel->render();

        if (barrel->getIsAlive()) {
            barrel->checkCollisionWithBullet(player.getBullets(), &score);
            player.checkCollisionWithEntity(barrel->getPosition(), barrel->getSize());
        }
    }
}

void powerUpLogic() {
    for (auto& powerUp : powerUps) {
        powerUp->render();

        bool coll = player.checkCollisionWithPowerUp(powerUp->getPosition(), powerUp->getSize(), powerUp->getType());
        if (coll) {
            powerUp->deactivate();
        }
    }
}

void enemySpawn() {
    static int lastSpawnTime_a = 0;
    static int lastSpawnTime_b = 0;
    int elapsedTime = (int)(time(0) - startTime);
    if (elapsedTime / 10 > lastSpawnTime_a / 10) {
        Vector2 spawn = Curve::generateSpawn(insideCurve, outsideCurve, 40, 80);
        enemies.push_back(new Enemy_a(spawn.x, spawn.y, 40, 0.7, 30.0));
        lastSpawnTime_a = elapsedTime;
    }
    if (elapsedTime / 8 > lastSpawnTime_b / 8) {
        Vector2 spawn = Curve::generateSpawn(insideCurve, outsideCurve, 50, 100);
        enemies_b.push_back(new Enemy_b(spawn.x, spawn.y, 50, 0.5, 80.0));
        lastSpawnTime_b = elapsedTime;
    }

}

void barrelSpawn() {
    static int lastSpawnTime = 0;
    int elapsedTime = (int)(time(0) - startTime);
    if (elapsedTime / 5 > lastSpawnTime / 5) {
        Vector2 spawn = Curve::generateSpawn(insideCurve, outsideCurve, 50, 100);
        barrels.push_back(new Barrel(spawn.x, spawn.y, 50, 8.0));
        lastSpawnTime = elapsedTime;
    }
}

void powerUpSpawn() {
    static int lastSpawnTime_p = 0;
    int elapsedTime = (int)(time(0) - startTime);
    if (elapsedTime / 5 > lastSpawnTime_p / 5) {
        Vector2 spawn = Curve::generateSpawn(insideCurve, outsideCurve, 40, 80);
        powerUps.push_back(new PowerUp(spawn.x, spawn.y, rand() % 4, 20));
        lastSpawnTime_p = elapsedTime;
    }
}

void renderGame() {
    fps.update();
    renderBg();
    renderTime();

    playerLogic();

    enemyLogic();
    removeDeadEnemies();
    enemySpawn();

    barrelLogic();
    removeDeadBarrels();
    barrelSpawn();

    powerUpLogic();
    removeInactivePowerUps();
    powerUpSpawn();
}

void renderLost() {
    CV::color(32 / 255.0, 32 / 255.0, 32 / 255.0);
    CV::rectFill(0, 0, screenWidth, screenHeight);
    std::string scoreText = "Pontuacao: " + std::to_string(score);
    CV::color(1, 1, 1);
    CV::text(screenWidth / 2 - 120, screenHeight / 2 + 50, scoreText.c_str());
    CV::text(screenWidth / 2 - 100, screenHeight / 2, "Game Over");
    // Só pede nome se for top 10
    const auto& scores = scoreManager.getScores();
    bool canEnterRanking = (scores.size() < 10) || (score > scores.back().score);
    if (!scoreRegistered && canEnterRanking) {
        CV::color(1,1,0);
        CV::text(screenWidth / 2 - 200, screenHeight / 2 - 70, "Digite seu nome e pressione ENTER:");
        CV::text(screenWidth / 2 - 200, screenHeight / 2 - 100, playerName.c_str());
    } else if (!scoreRegistered && !canEnterRanking) {
        CV::color(1,0.5,0.5);
        CV::text(screenWidth / 2 - 250, screenHeight / 2 - 70, "Score insuficiente para entrar no ranking.");
    }
}

void renderRanking() {
    CV::color(32 / 255.0, 32 / 255.0, 32 / 255.0);
    CV::rectFill(0, 0, screenWidth, screenHeight);
    CV::color(1,1,1);
    CV::text(screenWidth / 2 - 100, screenHeight / 2 + 200, "Ranking - Top 10");
    scoreManager.render(screenWidth / 2 - 200, screenHeight / 2 + 150);
    CV::color(1,1,0);
    CV::text(screenWidth / 2 - 200, screenHeight / 2 - 200, "Pressione ENTER para jogar novamente.");
    CV::text(screenWidth / 2 - 120, screenHeight / 2 - 230, "Pressione ESC para sair.");
}

void defineCurves() {
    renderBg();
    if (drawingInside) {
        insideCurve.defineCurve("curva interna");
    }
    else if (drawingOutside) {
        insideCurve.render();
        outsideCurve.defineCurve("curva externa");
    }
}

void render()
{
    fps.frameStart();
    if (currentScreen == GAME) {
        if (curves) {
            if (player.getIsAlive()) {
                renderGame();
            } else {
                currentScreen = GAME_OVER;
            }
        } else {
            defineCurves();
        }
    } else if (currentScreen == GAME_OVER) {
        renderLost();
    } else if (currentScreen == RANKING) {
        renderRanking();
    }
    fps.sleepIfNeeded();
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    if (currentScreen == GAME) {
        switch(key) {
            case 27:
                exit(0);
                break;
            case 13:
                if (drawingInside) {
                    insideCurve.closeCurve();
                    drawingInside = false;
                    drawingOutside = true;
                } else if (drawingOutside) {
                    outsideCurve.closeCurve();
                    drawingOutside = false;
                    startGame();
                    curves = true;
                }
                break;
            case 'a':
                player.setAngle(player.getAngle() + 0.2);
                break;
            case 'd':
                player.setAngle(player.getAngle() - 0.2);
                break;
        }
    }
    else if (currentScreen == GAME_OVER) {
        const auto& scores = scoreManager.getScores();
        bool canEnterRanking = (scores.size() < 10) || (score > scores.back().score);
        if (!scoreRegistered && canEnterRanking) {
            if (key >= 32 && key <= 126 && playerName.size() < 16) {
                playerName += (char)key;
            } else if (key == 8 && !playerName.empty()) {
                playerName.pop_back();
            } else if (key == 13) {
                std::string nameToSave = playerName.empty() ? "Player" : playerName;
                scoreManager.addScore(nameToSave, score);
                scoreRegistered = true;
                currentScreen = RANKING;
            }
        } else if (!scoreRegistered && !canEnterRanking) {
            // Não entra no ranking, só mostra o ranking
            scoreRegistered = true;
            currentScreen = RANKING;
        }
        if (key == 27) exit(0);
    }
    else if (currentScreen == RANKING) {
        if (key == 27) exit(0);
        if (key == 13) { // ENTER para jogar novamente
            resetGameState();
            startGame();
            currentScreen = GAME;
        }
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (curves) {
            player.shoot();
        } else {
            if (drawingInside) {
                insideCurve.addPoint(x, screenHeight - y);
            } else {
                outsideCurve.addPoint(x, screenHeight - y);
            }
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (!curves) {
            if (drawingInside) {
                insideCurve.removeLastPoint();
            }
            else if (drawingOutside) {
                outsideCurve.removeLastPoint();
            }
        }
    }
}

int main(void)
{
    scoreManager.load();
    CV::init(&screenWidth, &screenHeight, "Tank Game - T3 - Mathias Recktenvald");
    CV::run();
}
