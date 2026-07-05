#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
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
    Player* mPlayer;
    std::vector<Platform*> mPlatforms;
    
    sf::Sprite* mBackground;
    sf::Sprite* mStartButton;
    sf::Sprite* mRestartButton;
    sf::Sprite* mMenuButton;
    
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