#include "ScoreManager.h"

void ScoreManager::addPoints(int points) {
    score += points;
}

int ScoreManager::getScore() const {
    return score;
}

void ScoreManager::reset() {
    score = 0;
}
