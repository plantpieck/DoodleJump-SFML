#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "ResourceManager.hpp"
#include "Player.hpp"
#include "Platform.hpp"

enum class GameState { Menu, Playing, GameOver };

class Game {
private:
    sf::RenderWindow mWindow;
    ResourceManager<sf::Texture, std::string> mTextures;
    sf::Font mFont;
    
    GameState mState;
    std::unique_ptr<Player> mPlayer;
    std::vector<std::unique_ptr<Platform>> mPlatforms;
    
    int mScore;
    int mHighScore;
    
    void loadResources();
    void resetGame();
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions();
    void generatePlatforms(float startY);
    void loadHighScore();
    void saveHighScore();

public:
    Game();
    void run();
};