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
#include "BlackHole.hpp"

enum class GameState { Menu, Playing, Dying, GameOver, Settings };
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
    std::vector<BlackHole*> mBlackHoles;
    std::vector<Bullet*> mBullets;
    float mFireTimer;
    
    bool isPositionValid(sf::FloatRect bounds);
    void spawnMonster(float baseY);
    void spawnBlackHole(float baseY);
    
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
    
    sf::Text mTitleText{mFont}; 
    sf::Text mSettingsTitleText{mFont};
    sf::Text mVolumeText{mFont};
    sf::Text mEasyText{mFont};
    sf::Text mMediumText{mFont};
    sf::Text mHardText{mFont};
    sf::Text mBackText{mFont};
    sf::Text mSettingsButtonText{mFont};
    sf::Text mModeText{mFont};
    sf::Text mInstructionText1{mFont};
    sf::Text mInstructionText2{mFont};
    sf::Text mVolumeValueText{mFont};
    sf::Text mDifficultyTitleText{mFont};
    sf::Text mLostText{mFont};
    sf::Text mCurrentScoreText{mFont};

    ResourceManager<sf::SoundBuffer, std::string> mSoundBuffers;
    sf::SoundBuffer mDummyBuffer; 
    sf::Sound mJumpSound{mDummyBuffer};
    sf::Sound mShootSound{mDummyBuffer};
    sf::Sound mLoseSound{mDummyBuffer};
    sf::Music mBgMusic;
    
    void applyVolume(); 

    sf::RectangleShape mEasyBox;
    sf::RectangleShape mMediumBox;
    sf::RectangleShape mHardBox;

    sf::Vector2f mDeathTarget;
    float mDeathTimer;

public:
    Game();
    ~Game();
    void run();
};