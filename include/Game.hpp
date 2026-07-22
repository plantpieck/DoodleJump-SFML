#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "ResourceManager.hpp"
#include "Player.hpp"
#include "Platform.hpp"
#include "Monster.hpp"
#include "Bullet.hpp"
#include <SFML/Audio.hpp>

enum class GameState { Menu, Playing, GameOver, Settings };
enum class Difficulty { Easy, Medium, Hard };

class Game {
private:
    sf::RenderWindow mWindow;
    sf::View mWorldView;
    ResourceManager<sf::Texture, std::string> mTextures;
    sf::Font mFont;
    
    GameState mState;
    Difficulty mDifficulty;
    float mVolume;
    Player* mPlayer;
    std::vector<Platform*> mPlatforms;

    std::vector<Monster*> mMonsters;
    std::vector<Bullet*> mBullets;
    float mFireTimer;
    
    bool isPositionValid(sf::FloatRect bounds);
    void spawnMonster(float baseY);
    
    sf::Sprite* mBackground;
    sf::Sprite* mStartButton;
    sf::Sprite* mRestartButton;
    sf::Sprite* mMenuButton;
    sf::Sprite* mSettingsButton; 
    sf::Sprite* mBackButton;
    
    int mScore;
    std::map<Difficulty, int> mHighScores;
    
    void loadResources();
    void resetGame();
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions();
    void generatePlatforms(float startY);
    void loadHighScores();
    void saveHighScores();
    void loadSettings();
    void saveSettings();

    void renderSettings();
    void processSettingsEvents(sf::Vector2f mousePos);

    sf::RectangleShape mSliderTrack;
    sf::RectangleShape mSliderHandle;
    sf::Text mSettingsTitleText;
    sf::Text mVolumeText;
    sf::Text mEasyText;
    sf::Text mMediumText;
    sf::Text mHardText;
    sf::Text mBackText;
    sf::Text mSettingsButtonText;

    ResourceManager<sf::SoundBuffer, std::string> mSoundBuffers;
    sf::Sound mJumpSound;
    sf::Sound mShootSound;
    sf::Sound mLoseSound;
    sf::Music mBgMusic;
    
    void applyVolume(); 

public:
    Game();
    ~Game();
    void run();
};