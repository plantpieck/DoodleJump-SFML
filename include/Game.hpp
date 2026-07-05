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
    sf::View mWorldView;
    ResourceManager<sf::Texture, std::string> mTextures;
    sf::Font mFont;
    
    GameState mState;
    std::unique_ptr<Player> mPlayer;
    std::vector<std::unique_ptr<Platform>> mPlatforms;
    
    std::unique_ptr<sf::Sprite> mBackground;
    std::unique_ptr<sf::Sprite> mStartButton;
    std::unique_ptr<sf::Sprite> mRestartButton;
    std::unique_ptr<sf::Sprite> mMenuButton;
    
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
    ~Game();
    void run();
};