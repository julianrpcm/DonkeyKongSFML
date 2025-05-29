#pragma once

class ScoreManager {
public:
    void addPoints(int points);
    int getScore() const;
    void reset();

private:
    int score = 0;
};
