#include "Game.h"
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <random>

Game::Game()
    : window(sf::VideoMode(1280, 1024), "Magic Forest") {
    initUI();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();
    }

    if (gameState == GameState::MainMenu && e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });

        if (playText.getGlobalBounds().contains(mousePos)) {
            loadLevel();
            gameState = GameState::Playing;
        }

        if (exitText.getGlobalBounds().contains(mousePos)) {
            window.close();
        }
    }

    if (showDeathMenu && e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });

        if (retryText.getGlobalBounds().contains(mousePos)) {
            restartLevelFromBeginning();
            showDeathMenu = false;
        }

        if (quitText.getGlobalBounds().contains(mousePos)) {
            window.close();
        }
    }
}

void Game::update(float deltaTime) {
    if (gameState != GameState::Playing)
        return;

    auto ladders = level.getLadderRects();

    std::vector<sf::FloatRect> activeBlockers;
    if (level.getLadderBlockersEnabled()) {
        activeBlockers = level.getLadderBlockersRects();
    }

    player.update(deltaTime, baseColliders, ladders, activeBlockers, enemies, bossPtr.get());

    sf::FloatRect playerBounds = player.getBounds();
    sf::Vector2u windowSize = window.getSize();

    bool fueraHorizontal = playerBounds.left + playerBounds.width < 0.f ||
        playerBounds.left > windowSize.x;
    bool fueraVertical = playerBounds.top + playerBounds.height < 0.f ||
        playerBounds.top > windowSize.y;

    if ((fueraHorizontal || fueraVertical) && player.isAlive()) {
        player.die();
    }

    if (!player.isAlive() && player.hasDeathAnimationFinished()) {
        showDeathMenu = true;
    }

    for (auto& enemy : enemies) {
        enemy->update(deltaTime, enemyGroundColliders);
    }

    for (const auto& enemy : enemies) {
        auto* venom = dynamic_cast<VenomEnemy*>(enemy.get());
        if (venom) {
            for (const auto& projectile : venom->getProjectiles()) {
                if (projectile->getBounds().intersects(player.getBounds())) {
                    player.die();
                }
            }
        }
    }

    bossPtr->setLadders(ladders);
    bossPtr->setGroundColliders(barrelGroundColliders);
    bossPtr->update(deltaTime, enemyGroundColliders);

   

    for (const auto& enemy : enemies) {
        if (enemy->shouldGiveScore()) {
            scoreManager.addPoints(100);
            enemy->hasGivenScore = true;
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return enemy->canBeRemoved();
            }),
        enemies.end()
    );

    for (auto& coin : coins) {
        coin.update(deltaTime);

        if (!coin.isCollected() && coin.getBounds().intersects(player.getBounds())) {
            coin.collect();
            scoreManager.addPoints(50);
        }
    }

    for (auto& rune : runes) {
        rune.update(deltaTime);

        if (!rune.isCollected() && rune.getBounds().intersects(player.getBounds())) {
            rune.collect();
            player.applySpeedBuff(10.f);
        }
    }


    if (bossPtr && bossPtr->shouldGiveScore()) {
        scoreManager.addPoints(500);
        bossPtr->hasGivenScore = true;
    }

    if (bossPtr && bossPtr->canBeRemoved()) {
        currentLevel++;
        enemySpeed *= 1.2f;
        spawnDelay *= 0.85f;
        restartLevel();
    }

    updateUI();
}

void Game::render() {
    window.clear();

    if (gameState == GameState::MainMenu) {
        window.draw(menuBackground);
        window.draw(titleText);
        window.draw(playText);
        window.draw(exitText);
        window.display();
        return;
    }

    level.draw(window);
    player.draw(window);
    bossPtr->draw(window);
    window.draw(levelText);
    window.draw(scoreText);

    for (auto& enemy : enemies) {
        enemy->draw(window);
    }

    for (const auto& coin : coins) {
        coin.draw(window);
    }

    for (const auto& rune : runes) {
        rune.draw(window);
    }


    if (showDeathMenu) {
        window.draw(deathOverlay);
        window.draw(gameOverText);
        window.draw(retryText);
        window.draw(quitText);
    }

    window.display();
}

void Game::restartLevel() {
    std::string projectPath = getProjectPath();

    enemies.clear();
    coins.clear();

    spawnEnemyAt(EnemyType::Boss, 0);
    spawnEnemyAt(EnemyType::Basic, 14);
    spawnEnemyAt(EnemyType::Basic, 35);
    spawnEnemyAt(EnemyType::Basic, 21);
    spawnEnemyAt(EnemyType::Venom, 17);

    spawnRandomCoin();
    spawnRandomCoin();
    spawnRandomCoin();

    spawnRandomRune();

    {
        sf::FloatRect base = enemyGroundColliders[32];
        sf::Vector2f position = {
            base.left + base.width / 2.f,
            base.top + base.height / 2.f - 32.f
        };

        player.reset(position);
    }
}

void Game::restartLevelFromBeginning() {
    currentLevel = 1;
    enemySpeed = 100.f;
    spawnDelay = 3.f;
    scoreManager.reset();

    restartLevel();
}

void Game::loadLevel() {
    std::string rootPath = getProjectPath();
    std::string tmxPath = rootPath + "/assets/maps/world.tmx";

    if (!level.loadFromFile(tmxPath, getProjectPath())) {
        std::cerr << "Error loading level\n";
        exit(1);
    }

    baseColliders = level.getCollisionRects();
    barrelGroundColliders = baseColliders;
    enemyGroundColliders = level.getCollisionRects();

    for (const auto& c : level.getLadderBlockersRects()) {
        enemyGroundColliders.push_back(c);
    }

    player.loadTexture(getProjectPath());
    player.setLevel(&level);

    {
        sf::FloatRect base = enemyGroundColliders[32];
        sf::Vector2f position = {
            base.left + base.width / 2.f,
            base.top + base.height / 2.f - 32.f
        };

        player.reset(position);
    }

    spawnEnemyAt(EnemyType::Boss, 0);
    spawnEnemyAt(EnemyType::Basic, 14);
    spawnEnemyAt(EnemyType::Basic, 35);
    spawnEnemyAt(EnemyType::Basic, 21);
    spawnEnemyAt(EnemyType::Venom, 17);

    spawnRandomCoin();
    spawnRandomCoin();
    spawnRandomCoin();

    spawnRandomRune();
}

void Game::updateUI() {
    scoreText.setString("Score: " + std::to_string(scoreManager.getScore()));
    levelText.setString("Level: " + std::to_string(currentLevel));
}

void Game::initUI() {

    std::string bgPath = getProjectPath() + "/assets/sprites/UI/menu_background.png";

    if (!menuBackgroundTexture.loadFromFile(bgPath)) {
        std::cerr << "Failed to load menu_backgorund\n";
    }
    else {
        menuBackground.setTexture(menuBackgroundTexture);
        sf::Vector2u textureSize = menuBackgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        menuBackground.setScale(scaleX, scaleY);
    }

    if (!font.loadFromFile(getProjectPath() + "/assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
        return;
    }

    levelText.setFont(font);
    levelText.setCharacterSize(28);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(20.f, 20.f);
    levelText.setString("Level: 1");

    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20.f, 60.f);
    scoreText.setString("Score: 0");

    deathOverlay.setSize(sf::Vector2f(1280.f, 1024.f));
    deathOverlay.setFillColor(sf::Color(50, 50, 50, 150));

    retryText.setFont(font);
    retryText.setCharacterSize(36);
    retryText.setFillColor(sf::Color::White);
    retryText.setString("Retry");
    retryText.setPosition(560.f, 400.f);

    quitText.setFont(font);
    quitText.setCharacterSize(36);
    quitText.setFillColor(sf::Color::White);
    quitText.setString("Exit");
    quitText.setPosition(570.f, 460.f);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over");
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    gameOverText.setPosition(640.f, 250.f);

    titleText.setFont(font);
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Magic Forest");
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    titleText.setPosition(640.f, 200.f);

    playText.setFont(font);
    playText.setCharacterSize(36);
    playText.setFillColor(sf::Color::White);
    playText.setString("Play");
    playText.setPosition(500.f, 400.f);

    exitText.setFont(font);
    exitText.setCharacterSize(36);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Exit");
    exitText.setPosition(700.f, 400.f);

}

//Spawn
void Game::spawnEnemyAt(EnemyType type, int colliderIndex) {
    if (colliderIndex < 0 || colliderIndex >= static_cast<int>(enemyGroundColliders.size())) {
        return;
    }

    const auto& collider = enemyGroundColliders[colliderIndex];

    switch (type) {
    case EnemyType::Basic: {
        auto enemy = std::make_unique<BasicEnemy>(collider, getProjectPath());
        enemy->setSpeed(enemySpeed);
        enemies.push_back(std::move(enemy));
        break;
    }
    case EnemyType::Venom: {
        auto venom = std::make_unique<VenomEnemy>(collider, getProjectPath());

        float adjustedCooldown = std::max(2.f, 10.f - (currentLevel * 0.5f));
        venom->setShootCooldown(adjustedCooldown);

        enemies.push_back(std::move(venom));
        break;
    }
    case EnemyType::Boss: {
        bossPtr = std::make_unique<BossEnemy>(collider, getProjectPath());

        float baseCooldown = 5.f;
        float minCooldown = 1.2f;
        float reductionPerLevel = 0.5f;

        float newCooldown = std::max(minCooldown, baseCooldown - (currentLevel - 1) * reductionPerLevel);
        bossPtr->setShootCooldown(newCooldown);
        break;
    }
    }
}

void Game::spawnRandomCoin() {
    const std::string path = getProjectPath();

    float minX = 0.f;
    float maxX = static_cast<float>(window.getSize().x - 16);
    float minY = 0.f;
    float maxY = static_cast<float>(window.getSize().y - 16);

    const int maxAttempts = 100;
    int attempts = 0;
    sf::FloatRect coinBounds(0, 0, 16.f, 16.f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    while (attempts < maxAttempts) {
        attempts++;

        sf::Vector2f pos(distX(gen), distY(gen));
        coinBounds.left = pos.x;
        coinBounds.top = pos.y;

        bool intersectsCollider = false;
        for (const auto& collider : baseColliders) {
            if (collider.intersects(coinBounds)) {
                intersectsCollider = true;
                break;
            }
        }

        if (!intersectsCollider) {
            coins.emplace_back(pos, path);
            return;
        }
    }
}

void Game::spawnRandomRune()
{
    const std::string path = getProjectPath();

    float minX = 0.f;
    float maxX = static_cast<float>(window.getSize().x - 16);
    float minY = 0.f;
    float maxY = static_cast<float>(window.getSize().y - 16);

    const int maxAttempts = 100;
    int attempts = 0;
    sf::FloatRect runeBounds(0, 0, 16.f, 16.f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    while (attempts < maxAttempts) {
        attempts++;

        sf::Vector2f pos(distX(gen), distY(gen));
        runeBounds.left = pos.x;
        runeBounds.top = pos.y;

        bool intersectsCollider = false;
        for (const auto& collider : baseColliders) {
            if (collider.intersects(runeBounds)) {
                intersectsCollider = true;
                break;
            }
        }

        if (!intersectsCollider) {
            runes.emplace_back(pos, path);
            return;
        }
    }
}

std::string Game::getProjectPath() {
    auto path = std::filesystem::current_path();
    while (!std::filesystem::exists(path / "CMakeLists.txt") && path.has_parent_path()) {
        path = path.parent_path();
    }
    return path.string();
}
