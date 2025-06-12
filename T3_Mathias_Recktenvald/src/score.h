/*
    Gerenciador de placar (ranking) dos maiores scores do jogo.
    Mantém os 10 maiores scores registrados, permitindo salvar, carregar e exibir o ranking.
    O arquivo de scores é salvo em T3_Mathias_Recktenvald/src/score.txt.

    Classes e métodos:
    - struct ScoreEntry: representa uma entrada do ranking (nome e score).
    - class ScoreManager: gerencia o ranking, incluindo métodos para adicionar score, carregar/salvar do arquivo e renderizar o ranking na tela.
*/

#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

struct ScoreEntry {
    std::string name;
    int score;
    bool operator<(const ScoreEntry& other) const {
        return score > other.score; // ordem decrescente
    }
};

class ScoreManager {
    std::vector<ScoreEntry> scores;
    const std::string filename = "T3_Mathias_Recktenvald/src/score.txt";
    const size_t maxScores = 10;
public:
    ScoreManager() { load(); }
    void addScore(const std::string& name, int score) {
        scores.push_back({name, score});
        std::sort(scores.begin(), scores.end());
        if (scores.size() > maxScores) scores.resize(maxScores);
        save();
    }
    void load() {
        scores.clear();
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (std::getline(iss, name, ';') && iss >> score) {
                scores.push_back({name, score});
            }
        }
        std::sort(scores.begin(), scores.end());
        if (scores.size() > maxScores) scores.resize(maxScores);
    }
    void save() {
        std::ofstream file(filename);
        for (const auto& entry : scores) {
            file << entry.name << ";" << entry.score << "\n";
        }
    }
    const std::vector<ScoreEntry>& getScores() const { return scores; }
    void render(int x, int y) {
        // Exibe os 10 maiores scores na tela
        for (size_t i = 0; i < scores.size(); ++i) {
            std::string line = std::to_string(i+1) + " - " + scores[i].name + ": " + std::to_string(scores[i].score);
            CV::color(1,1,1);
            CV::text(x, y - 25 * (int)i, line.c_str());
        }
    }
};

#endif
