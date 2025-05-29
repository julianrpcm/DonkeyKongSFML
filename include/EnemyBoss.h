#include "Enemy.hpp"

class BossEnemy : public Enemy {
public:
    BossEnemy(/*params*/);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    void shootBarrel();
    float shootCooldown;
};
