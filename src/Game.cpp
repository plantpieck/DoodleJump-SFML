#include "../include/Game.hpp"
#include "../include/NormalPlatform.hpp"
#include "../include/MovingPlatform.hpp"
#include "../include/BreakablePlatform.hpp"
#include "../include/SpringPlatform.hpp"
#include <SFML/Window/Event.hpp>
#include <fstream>
#include <random>
#include <iostream>
#include <algorithm>

Game::Game() : mWindow(sf::VideoMode({500, 800}), "Doodle Jump - Phase 2"), mState(GameState::Menu), mScore(0) {
    mWindow.setFramerateLimit(60);
    mWorldView = mWindow.getDefaultView();
    
    loadSettings();
    loadHighScores();
    loadResources();
    
    mPlayer = new Player(mTextures.get("doodle_left"), mTextures.get("doodle_right"), mTextures.get("doodle_shoot"));
    resetGame();
}

Game::~Game() {
    saveHighScores();
    saveSettings();
    
    delete mPlayer;
    delete mBackground;
    delete mStartButton;
    delete mRestartButton;
    delete mMenuButton;
    delete mSettingsButton;
    delete mBackButton;
    
    for (auto platform : mPlatforms) {
        delete platform;
    }
    mPlatforms.clear();

    for (auto monster : mMonsters) {
        delete monster;
    }
    mMonsters.clear();

    for (auto* bh : mBlackHoles) {
        delete bh;
    }
    mBlackHoles.clear();

    for (auto bullet : mBullets) {
        delete bullet;
    }
    mBullets.clear();
    
    mFireTimer = 0.f;
}

void Game::loadSettings() {
    mVolume = 50.f;
    mDifficulty = Difficulty::Medium;

    std::ifstream file("settings.txt");
    if (file.is_open()) {
        float v;
        int diff;
        if (file >> v >> diff) {
            mVolume = v;
            mDifficulty = static_cast<Difficulty>(diff);
        }
        file.close();
    }
}

void Game::saveSettings() {
    std::ofstream file("settings.txt");
    if (file.is_open()) {
        file << mVolume << "\n";
        file << static_cast<int>(mDifficulty) << "\n";
        file.close();
    }
}

void Game::loadHighScores() {
    mHighScores[Difficulty::Easy] = 0;
    mHighScores[Difficulty::Medium] = 0;
    mHighScores[Difficulty::Hard] = 0;

    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        int e, m, h;
        if (file >> e >> m >> h) {
            mHighScores[Difficulty::Easy] = e;
            mHighScores[Difficulty::Medium] = m;
            mHighScores[Difficulty::Hard] = h;
        }
        file.close();
    }
}
void Game::saveHighScores() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << mHighScores[Difficulty::Easy] << "\n";
        file << mHighScores[Difficulty::Medium] << "\n";
        file << mHighScores[Difficulty::Hard] << "\n";
        file.close();
    }
}

void Game::loadResources() {
    mTextures.load("doodle_left", "assets/left_doodle.png");
    mTextures.load("doodle_right", "assets/right_doodle.png");
    mTextures.load("platform_normal", "assets/normal_platform.png");
    mTextures.load("platform_moving", "assets/moving_platform.png");
    mTextures.load("platform_breakable", "assets/broken_platform.png");
    mTextures.load("background", "assets/background.png");
    mTextures.load("button_start", "assets/start_button.png");
    mTextures.load("button_restart", "assets/restart_button.png");
    mTextures.load("button_menu", "assets/menu_button.png");
    mTextures.load("spring", "assets/spring_sprite.png");
    mTextures.load("monster1", "assets/BlueMonster.png");
    mTextures.load("monster2", "assets/green_monster.png");
    mTextures.load("doodle_shoot", "assets/hahahaha.png");
    mTextures.load("bullet", "assets/Nose.png");
    mTextures.load("button_settings", "assets/Settings_button.png");
    mTextures.load("button_back", "assets/back_button.png");
    mTextures.load("black_hole", "assets/hole.png");
    
    if (!mFont.openFromFile("fonts/ariblk.ttf")) {
        std::cerr << "Failed to load font!\n";
    }

    mBackground = new sf::Sprite(mTextures.get("background"));
    auto bgSize = mTextures.get("background").getSize();
    mBackground->setScale({500.f / static_cast<float>(bgSize.x), 800.f / static_cast<float>(bgSize.y)});

    mTitleText.setFont(mFont);
    mTitleText.setString("DOODLE JUMP");
    mTitleText.setCharacterSize(44);
    mTitleText.setFillColor(sf::Color(20, 80, 120));
    mTitleText.setStyle(sf::Text::Bold);
    mTitleText.setPosition({250.f - mTitleText.getGlobalBounds().size.x / 2.f, 100.f});

    mStartButton = new sf::Sprite(mTextures.get("button_start"));
    float scaleStart = 200.f / static_cast<float>(mTextures.get("button_start").getSize().x);
    mStartButton->setScale({scaleStart, scaleStart});
    mStartButton->setPosition({250.f - mStartButton->getGlobalBounds().size.x / 2.f, 250.f});

    mSettingsButton = new sf::Sprite(mTextures.get("button_settings"));
    float scaleSettings = 200.f / static_cast<float>(mTextures.get("button_settings").getSize().x);
    mSettingsButton->setScale({scaleSettings, scaleSettings});
    mSettingsButton->setPosition({250.f - mSettingsButton->getGlobalBounds().size.x / 2.f, 330.f});

    mModeText.setFont(mFont);
    mModeText.setCharacterSize(16);
    mModeText.setFillColor(sf::Color(50, 50, 50));
    mModeText.setStyle(sf::Text::Bold);

    mInstructionText1.setFont(mFont);
    mInstructionText1.setString("Use Left / Right arrows to move");
    mInstructionText1.setCharacterSize(28);
    mInstructionText1.setScale({0.5f, 0.5f});
    mInstructionText1.setFillColor(sf::Color(50, 50, 50));
    mInstructionText1.setStyle(sf::Text::Bold);
    mInstructionText1.setPosition({250.f - mInstructionText1.getGlobalBounds().size.x / 2.f, 450.f});

    mInstructionText2.setFont(mFont);
    mInstructionText2.setString("Hold Space to shoot the monsters");
    mInstructionText2.setCharacterSize(28);
    mInstructionText2.setScale({0.5f, 0.5f});
    mInstructionText2.setFillColor(sf::Color(50, 50, 50));
    mInstructionText2.setStyle(sf::Text::Bold);
    mInstructionText2.setPosition({250.f - mInstructionText2.getGlobalBounds().size.x / 2.f, 480.f});

    mSettingsTitleText.setFont(mFont);
    mSettingsTitleText.setString("SETTINGS");
    mSettingsTitleText.setCharacterSize(35);
    mSettingsTitleText.setFillColor(sf::Color(20, 70, 110));
    mSettingsTitleText.setStyle(sf::Text::Bold);
    mSettingsTitleText.setPosition({250.f - mSettingsTitleText.getGlobalBounds().size.x / 2.f, 80.f});

    mVolumeText.setFont(mFont);
    mVolumeText.setString("Volume");
    mVolumeText.setCharacterSize(20);
    mVolumeText.setFillColor(sf::Color(20, 70, 110));
    mVolumeText.setPosition({250.f - mVolumeText.getGlobalBounds().size.x / 2.f, 180.f});

    mSliderTrack.setSize({250.f, 5.f});
    mSliderTrack.setFillColor(sf::Color(200, 200, 200));
    mSliderTrack.setPosition({125.f, 230.f});

    mSliderHandle.setSize({15.f, 15.f});
    mSliderHandle.setFillColor(sf::Color(20, 70, 110));
    mSliderHandle.setOrigin({7.5f, 7.5f});

    mVolumeValueText.setFont(mFont);
    mVolumeValueText.setString("0%");
    mVolumeValueText.setCharacterSize(16);
    mVolumeValueText.setFillColor(sf::Color(100, 100, 100));
    mVolumeValueText.setPosition({250.f - mVolumeValueText.getGlobalBounds().size.x / 2.f, 250.f});

    mDifficultyTitleText.setFont(mFont);
    mDifficultyTitleText.setString("Difficulty");
    mDifficultyTitleText.setCharacterSize(20);
    mDifficultyTitleText.setFillColor(sf::Color(20, 70, 110));
    mDifficultyTitleText.setPosition({250.f - mDifficultyTitleText.getGlobalBounds().size.x / 2.f, 320.f});

    mEasyBox.setSize({90.f, 40.f});
    mEasyBox.setOutlineThickness(2.f);
    mEasyBox.setPosition({100.f, 380.f});

    mMediumBox.setSize({90.f, 40.f});
    mMediumBox.setOutlineThickness(2.f);
    mMediumBox.setPosition({205.f, 380.f});

    mHardBox.setSize({90.f, 40.f});
    mHardBox.setOutlineThickness(2.f);
    mHardBox.setPosition({310.f, 380.f});

    mEasyText.setFont(mFont);
    mEasyText.setString("EASY");
    mEasyText.setCharacterSize(16);
    mEasyText.setPosition({100.f + (90.f - mEasyText.getGlobalBounds().size.x) / 2.f, 380.f + (40.f - mEasyText.getGlobalBounds().size.y) / 2.f - 5.f});

    mMediumText.setFont(mFont);
    mMediumText.setString("MEDIUM");
    mMediumText.setCharacterSize(16);
    mMediumText.setPosition({205.f + (90.f - mMediumText.getGlobalBounds().size.x) / 2.f, 380.f + (40.f - mMediumText.getGlobalBounds().size.y) / 2.f - 5.f});

    mHardText.setFont(mFont);
    mHardText.setString("HARD");
    mHardText.setCharacterSize(16);
    mHardText.setPosition({310.f + (90.f - mHardText.getGlobalBounds().size.x) / 2.f, 380.f + (40.f - mHardText.getGlobalBounds().size.y) / 2.f - 5.f});

    mRestartButton = new sf::Sprite(mTextures.get("button_restart"));
    float scaleRestart = 200.f / static_cast<float>(mTextures.get("button_restart").getSize().x);
    mRestartButton->setScale({scaleRestart, scaleRestart});
    mRestartButton->setPosition({250.f - mRestartButton->getGlobalBounds().size.x / 2.f, 420.f});

    mMenuButton = new sf::Sprite(mTextures.get("button_menu"));
    float scaleMenu = 200.f / static_cast<float>(mTextures.get("button_menu").getSize().x);
    mMenuButton->setScale({scaleMenu, scaleMenu});
    mMenuButton->setPosition({250.f - mMenuButton->getGlobalBounds().size.x / 2.f, 500.f});

    mBackButton = new sf::Sprite(mTextures.get("button_back"));
    float scaleBack = 200.f / static_cast<float>(mTextures.get("button_back").getSize().x);
    mBackButton->setScale({scaleBack, scaleBack});
    mBackButton->setPosition({250.f - mBackButton->getGlobalBounds().size.x / 2.f, 600.f});

    mSoundBuffers.load("jump", "sounds/Jumping_Sound.wav");
    mSoundBuffers.load("shoot", "sounds/Shooting_Sound.wav");
    mSoundBuffers.load("lose", "sounds/Loosing_Sound.wav");

    mJumpSound.setBuffer(mSoundBuffers.get("jump"));
    mShootSound.setBuffer(mSoundBuffers.get("shoot"));
    mLoseSound.setBuffer(mSoundBuffers.get("lose"));

    if (!mBgMusic.openFromFile("sounds/MainMenu_Song.flac")) {
        std::cerr << "Failed to load background music!\n";
    }
    mBgMusic.setLooping(true);

    applyVolume();
    mBgMusic.play();
}

void Game::resetGame() {
    mWorldView.setCenter({250.f, 400.f});
    mScore = 0;
    
    for (auto platform : mPlatforms) {
        delete platform;
    }
    mPlatforms.clear();
    
    mPlayer->reset();
    mPlayer->setPosition({250.f, 600.f});
    
    mPlatforms.push_back(new NormalPlatform(mTextures.get("platform_normal"), sf::Vector2f({250.f, 750.f})));
    generatePlatforms(750.f);

    for (auto monster : mMonsters) {
        delete monster;
    }
    mMonsters.clear();
    spawnMonster(200.f);

    for (auto* bh : mBlackHoles) {
        delete bh;
    }
    mBlackHoles.clear();

    for (auto bullet : mBullets) {
        delete bullet;
    }
    mBullets.clear();
    
    mFireTimer = 0.f;
}

void Game::generatePlatforms(float startY) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0.f, 400.f);
    std::uniform_real_distribution<float> yDist(70.f, 110.f);
    std::uniform_int_distribution<int> typeDist(1, 100);

    float currentY = startY;
    bool lastWasBreakable = false;

    for (int i = 0; i < 15; ++i) {
        currentY -= yDist(gen);
        
        bool placed = false;
        int attempts = 0;
        Platform* newPlat = nullptr;
        
        while (!placed && attempts < 10) {
            float newX = xDist(gen);
            int r = typeDist(gen);
            
            if (r <= 65) {
                newPlat = new NormalPlatform(mTextures.get("platform_normal"), sf::Vector2f({newX, currentY}));
                lastWasBreakable = false;
            } else if (r <= 80) {
                newPlat = new MovingPlatform(mTextures.get("platform_moving"), sf::Vector2f({newX, currentY}));
                lastWasBreakable = false;
            } else if (r <= 85) {
                newPlat = new SpringPlatform(mTextures.get("platform_normal"), mTextures.get("spring"), sf::Vector2f({newX, currentY}));
                lastWasBreakable = false;
            } else {
                if (lastWasBreakable) {
                    newPlat = new NormalPlatform(mTextures.get("platform_normal"), sf::Vector2f({newX, currentY}));
                    lastWasBreakable = false;
                } else {
                    newPlat = new BreakablePlatform(mTextures.get("platform_breakable"), sf::Vector2f({newX, currentY}));
                    lastWasBreakable = true;
                }
            }
            
            if (isPositionValid(newPlat->getBounds())) {
                placed = true;
            } else {
                delete newPlat;
                newPlat = nullptr;
                attempts++;
            }
        }
        
        if (placed && newPlat != nullptr) {
            mPlatforms.push_back(newPlat);
        }

        spawnBlackHole(currentY);
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        
        if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseBtn->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                
                if (mState == GameState::Menu) {
                    if (mStartButton->getGlobalBounds().contains(mousePos)) {
                        mState = GameState::Playing;
                        mBgMusic.stop(); 
                        resetGame();
                    } else if (mSettingsButton->getGlobalBounds().contains(mousePos)) {
                        mState = GameState::Settings; 
                    }
                } else if (mState == GameState::GameOver) {
                    if (mRestartButton->getGlobalBounds().contains(mousePos)) {
                        mState = GameState::Playing;
                        mBgMusic.stop();
                        resetGame();
                    } else if (mMenuButton->getGlobalBounds().contains(mousePos)) {
                        mState = GameState::Menu;
                        mBgMusic.play(); 
                    }
                } else if (mState == GameState::Settings) {
                    processSettingsEvents(mousePos); 
                }
            }
        }
        
        if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mState == GameState::Settings) {
                sf::Vector2f mousePos(static_cast<float>(mouseMove->position.x), static_cast<float>(mouseMove->position.y));
                
                sf::FloatRect trackBounds = mSliderTrack.getGlobalBounds();
                trackBounds.position.y -= 20.f;
                trackBounds.size.y += 40.f;
                trackBounds.position.x -= 20.f;
                trackBounds.size.x += 40.f;
                
                if (trackBounds.contains(mousePos)) {
                    float newVolume = (mousePos.x - mSliderTrack.getPosition().x) / mSliderTrack.getSize().x * 100.f;
                    mVolume = std::clamp(newVolume, 0.f, 100.f);
                    applyVolume(); 
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (mState == GameState::Playing) {
        mPlayer->handleInput();
        mPlayer->update(dt);

        for (auto platform : mPlatforms) {
            platform->update(dt);
        }

        float topEdge = mWorldView.getCenter().y - 400.f;
        float bottomEdge = mWorldView.getCenter().y + 400.f;

        for (auto it = mBlackHoles.begin(); it != mBlackHoles.end(); ) {
            (*it)->update(dt);
            if ((*it)->getPosition().y > bottomEdge) {
                delete *it;
                it = mBlackHoles.erase(it);
            } else {
                ++it;
            }
        }

        handleCollisions();

        float playerY = mPlayer->getPosition().y;
        if (playerY < mWorldView.getCenter().y) {
            mWorldView.setCenter({250.f, playerY});
        }

        int currentScore = static_cast<int>(600.f - playerY);
        if (currentScore > mScore) {
            mScore = currentScore;
        }

        if (mFireTimer > 0.f) {
            mFireTimer -= dt;
            if (mFireTimer <= 0.f) {
                mPlayer->setTexture(mTextures.get("doodle_right")); 
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && mFireTimer <= 0.f) {
            float fireRate = (mDifficulty == Difficulty::Easy) ? 0.2f : 0.4f;
            mFireTimer = fireRate;

            sf::FloatRect playerBounds = mPlayer->getBounds();
            sf::Vector2f spawnPos(playerBounds.position.x + playerBounds.size.x / 2.f, playerBounds.position.y);
            
            mBullets.push_back(new Bullet(spawnPos));
            
            mShootSound.play();
            mPlayer->setTexture(mTextures.get("doodle_shoot"));
        }

        for (auto it = mBullets.begin(); it != mBullets.end(); ) {
            (*it)->update(dt);
            if ((*it)->getPosition().y < topEdge) {
                delete *it;
                it = mBullets.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = mMonsters.begin(); it != mMonsters.end(); ) {
            (*it)->update(dt);
            
            if ((*it)->isDead() || (*it)->getPosition().y > bottomEdge) {
                delete *it;
                it = mMonsters.erase(it);
            } else {
                ++it;
            }
        }

        if (mMonsters.empty()) {
            spawnMonster(topEdge - 200.f);
        }

        while (!mPlatforms.empty() && mPlatforms.front()->getPosition().y > bottomEdge) {
            delete mPlatforms.front();
            mPlatforms.erase(mPlatforms.begin());
            
            if (!mPlatforms.empty()) {
                generatePlatforms(mPlatforms.back()->getPosition().y);
            } else {
                generatePlatforms(bottomEdge - 200.f);
            }
        }

        if (playerY > bottomEdge) {
            if (mScore > mHighScores[mDifficulty]) {
                mHighScores[mDifficulty] = mScore;
                saveHighScores();
            }
            mState = GameState::GameOver;
            mLoseSound.play(); 
        }
        
    } else if (mState == GameState::Dying) {
        mDeathTimer -= dt;
        
        sf::Vector2f pos = mPlayer->getPosition();
        pos.x += (mDeathTarget.x - pos.x) * dt * 5.0f;
        pos.y += (mDeathTarget.y - pos.y) * dt * 5.0f;
        mPlayer->setPosition(pos);
        
        mPlayer->rotate(dt * 1500.f); 
        
        float currentScale = (mDeathTimer / 1.0f) * 0.55f;
        if (currentScale < 0.f) currentScale = 0.f;
        mPlayer->setScale(currentScale);

        if (mDeathTimer <= 0.f) {
            if (mScore > mHighScores[mDifficulty]) {
                mHighScores[mDifficulty] = mScore;
                saveHighScores();
            }
            mState = GameState::GameOver;
        }
    }
}

void Game::handleCollisions() {
    sf::FloatRect playerBounds = mPlayer->getBounds();
    
    for (auto* bh : mBlackHoles) {
        if (playerBounds.findIntersection(bh->getBounds()).has_value()) {
            mLoseSound.play();
            mState = GameState::Dying; 
            mDeathTarget = bh->getPosition();
            mDeathTimer = 1.0f;
            return;
        }
    }
    
    for (auto bulletIt = mBullets.begin(); bulletIt != mBullets.end(); ) {
        bool bulletDestroyed = false;
        for (auto monsterIt = mMonsters.begin(); monsterIt != mMonsters.end(); ) {
            if ((*bulletIt)->getBounds().findIntersection((*monsterIt)->getBounds()).has_value()) {
                (*monsterIt)->takeDamage();
                bulletDestroyed = true;
                
                if ((*monsterIt)->isDead()) {
                    delete *monsterIt;
                    monsterIt = mMonsters.erase(monsterIt);
                    mScore += 100;
                } else {
                    ++monsterIt;
                }
                break;
            } else {
                ++monsterIt;
            }
        }
        
        if (bulletDestroyed) {
            delete *bulletIt;
            bulletIt = mBullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    for (auto monster : mMonsters) {
        sf::FloatRect monsterBounds = monster->getBounds();
        if (playerBounds.findIntersection(monsterBounds).has_value()) {
            
            if (mPlayer->getVelocityY() > 0.f && 
                playerBounds.position.y + playerBounds.size.y < monsterBounds.position.y + monsterBounds.size.y * 0.5f) {
                
                mJumpSound.play();
                mPlayer->superJump(); 
            } else {
                mState = GameState::GameOver;
                mLoseSound.play(); 
                if (mScore > mHighScores[mDifficulty]) {
                    mHighScores[mDifficulty] = mScore;
                    saveHighScores();
                }
                return; 
            }
        }
    }

    if (mPlayer->getVelocityY() > 0.f) {
        for (auto platform : mPlatforms) {
            sf::FloatRect platformBounds = platform->getBounds();
            
            if (playerBounds.findIntersection(platformBounds).has_value()) {
                if (playerBounds.position.y + playerBounds.size.y < platformBounds.position.y + platformBounds.size.y) {
                    
                    if (auto springPlat = dynamic_cast<SpringPlatform*>(platform)) {
                        if (playerBounds.findIntersection(springPlat->getSpringBounds()).has_value()) {
                            mJumpSound.play();
                            mPlayer->superJump();
                            springPlat->compress(); 
                        } else {
                            mJumpSound.play();
                            mPlayer->jump();
                        }
                        break;
                    } 
                    else if (auto breakable = dynamic_cast<BreakablePlatform*>(platform)) {
                        if (!breakable->isBroken()) {
                            breakable->breakPlatform();
                        }
                    } 
                    else {
                        mJumpSound.play();
                        mPlayer->jump();
                        break;
                    }
                }
            }
        }
    }
}

void Game::renderSettings() {
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(*mBackground);

    mWindow.draw(mSettingsTitleText);
    mWindow.draw(mVolumeText);
    mWindow.draw(mSliderTrack);
    
    mSliderHandle.setPosition({125.f + (mVolume / 100.f) * 250.f, 232.5f});
    mWindow.draw(mSliderHandle);

    mVolumeValueText.setString(std::to_string(static_cast<int>(mVolume)) + "%");
    mVolumeValueText.setPosition({250.f - mVolumeValueText.getGlobalBounds().size.x / 2.f, 250.f});
    mWindow.draw(mVolumeValueText);

    mWindow.draw(mDifficultyTitleText);

    auto applyStyle = [](sf::RectangleShape& box, sf::Text& text, bool isSelected) {
        if (isSelected) {
            box.setFillColor(sf::Color(20, 70, 110));
            box.setOutlineColor(sf::Color(20, 70, 110));
            text.setFillColor(sf::Color::White);
        } else {
            box.setFillColor(sf::Color(240, 240, 240));
            box.setOutlineColor(sf::Color(150, 150, 150));
            text.setFillColor(sf::Color(20, 70, 110));
        }
    };

    applyStyle(mEasyBox, mEasyText, mDifficulty == Difficulty::Easy);
    applyStyle(mMediumBox, mMediumText, mDifficulty == Difficulty::Medium);
    applyStyle(mHardBox, mHardText, mDifficulty == Difficulty::Hard);

    mWindow.draw(mEasyBox); 
    mWindow.draw(mEasyText);
    mWindow.draw(mMediumBox); 
    mWindow.draw(mMediumText);
    mWindow.draw(mHardBox); 
    mWindow.draw(mHardText);
    
    mWindow.draw(*mBackButton);
}

void Game::render() {
    mWindow.clear(sf::Color::White);
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(*mBackground);

    if (mState == GameState::Playing || mState == GameState::Dying) {
        mWindow.setView(mWorldView);
        
        for (auto platform : mPlatforms) {
            platform->render(mWindow);
        }

        for (auto monster : mMonsters) {
            monster->render(mWindow);
        }

        for (auto* bh : mBlackHoles) {
            bh->render(mWindow);
        }

        for (auto bullet : mBullets) {
            bullet->render(mWindow);
        }

        mPlayer->render(mWindow);
        
        mWindow.setView(mWindow.getDefaultView());
        
        sf::Text scoreText(mFont, std::to_string(mScore), 28);
        scoreText.setFillColor(sf::Color::Red);
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setPosition({15.f, 15.f});
        mWindow.draw(scoreText);
        
    } else if (mState == GameState::Menu) {
        mWindow.setView(mWindow.getDefaultView());
        
        mWindow.draw(mTitleText);

        std::string modeStr = (mDifficulty == Difficulty::Easy) ? "EASY" : 
                              (mDifficulty == Difficulty::Medium) ? "MEDIUM" : "HARD";
                              
        sf::Text highText(mFont, "HIGH SCORE (" + modeStr + "): " + std::to_string(mHighScores[mDifficulty]), 20);
        highText.setFillColor(sf::Color({50, 50, 50}));
        highText.setStyle(sf::Text::Bold);
        highText.setPosition({250.f - highText.getGlobalBounds().size.x / 2.f, 180.f});
        mWindow.draw(highText);

        mWindow.draw(*mStartButton);
        mWindow.draw(*mSettingsButton);
        
        mModeText.setString("Mode: " + modeStr);
        mModeText.setPosition({250.f - mModeText.getGlobalBounds().size.x / 2.f, 400.f});
        
        mWindow.draw(mModeText);
        mWindow.draw(mInstructionText1);
        mWindow.draw(mInstructionText2);
    } else if (mState == GameState::GameOver) {
        mWindow.setView(mWindow.getDefaultView());
        
        sf::Text overText(mFont, "YOU LOST", 48);
        overText.setFillColor(sf::Color::Red);
        overText.setStyle(sf::Text::Bold);
        overText.setPosition({250.f - overText.getGlobalBounds().size.x / 2.f, 200.f});
        mWindow.draw(overText);

        sf::Text scoreText(mFont, "SCORE: " + std::to_string(mScore), 24);
        scoreText.setFillColor(sf::Color({50, 50, 50}));
        scoreText.setPosition({250.f - scoreText.getGlobalBounds().size.x / 2.f, 300.f});
        mWindow.draw(scoreText);

        sf::Text highText(mFont, "HIGH SCORE: " + std::to_string(mHighScores[mDifficulty]), 24);
        highText.setFillColor(sf::Color({50, 50, 50}));
        highText.setPosition({250.f - highText.getGlobalBounds().size.x / 2.f, 350.f});
        mWindow.draw(highText);

        mWindow.draw(*mRestartButton);
        mWindow.draw(*mMenuButton);
    }
    else if (mState == GameState::Settings) {
        renderSettings();
    }

    mWindow.display();
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

bool Game::isPositionValid(sf::FloatRect bounds) {
    sf::FloatRect paddedBounds = bounds;
    paddedBounds.position.x -= 15.f;
    paddedBounds.position.y -= 15.f;
    paddedBounds.size.x += 30.f;
    paddedBounds.size.y += 30.f;

    for (auto platform : mPlatforms) {
        if (paddedBounds.findIntersection(platform->getBounds()).has_value()) {
            return false;
        }
    }
    for (auto monster : mMonsters) {
        if (paddedBounds.findIntersection(monster->getBounds()).has_value()) {
            return false;
        }
    }
    for (auto bh : mBlackHoles) {
        if (paddedBounds.findIntersection(bh->getBounds()).has_value()) {
            return false;
        }
    }
    return true;
}

void Game::spawnMonster(float baseY) {
    if (baseY > -300.f) return;

    float bestSafeY = baseY - 80.f;
    bool foundSafeY = false;

    // Search for a Y level that is completely clear of all platforms and black holes
    for (float offsetY = 20.f; offsetY <= 200.f; offsetY += 5.f) {
        float testY = baseY - offsetY;
        bool isClear = true;
        
        // Must not share Y space with ANY platform (static or moving) to avoid sweeping intersections
        for (auto plat : mPlatforms) {
            // A distance of 30.f is enough to prevent vertical intersection 
            // (monster height ~40, platform height ~15 -> 20 + 7.5 = 27.5)
            if (std::abs(plat->getPosition().y - testY) < 30.f) {
                isClear = false;
                break;
            }
        }
        
        for (auto bh : mBlackHoles) {
            if (std::abs(bh->getPosition().y - testY) < 90.f) {
                isClear = false;
                break;
            }
        }

        if (isClear) {
            bestSafeY = testY;
            foundSafeY = true;
            break;
        }
    }

    if (!foundSafeY) return;

    float safeY = bestSafeY;

    int solidPlatformCount = 0;
    const Platform* singlePlat = nullptr;
    for (auto plat : mPlatforms) {
        if (std::abs(plat->getPosition().y - baseY) < 120.f) {
            if (!dynamic_cast<BreakablePlatform*>(plat) && !dynamic_cast<MovingPlatform*>(plat)) {
                solidPlatformCount++;
                singlePlat = plat;
            }
        }
    }

    float platformX = mPlatforms.back()->getPosition().x;
    float safeX = 0.f;
    
    if (platformX < 200.f) {
        safeX = platformX + 160.f + (rand() % 50);
    } else {
        safeX = platformX - 160.f - (rand() % 50);
    }

    if (safeX < 20.f) safeX = 20.f;
    if (safeX > 380.f) safeX = 380.f;

    if (solidPlatformCount <= 1 && singlePlat != nullptr) {
        float platX = singlePlat->getPosition().x;
        if (std::abs(safeX - platX) < 120.f) {
            return;
        }
    }

    sf::Vector2f pos(safeX, safeY);
    
    int initialHealth = (mDifficulty == Difficulty::Easy) ? 3 : 5;
    std::string monsterTex = (rand() % 2 == 0) ? "monster1" : "monster2";
    int frames = (monsterTex == "monster1") ? 2 : 1;
    float scale = (monsterTex == "monster1") ? 0.8f : 1.0f;
    Monster* newMonster = new Monster(mTextures.get(monsterTex), pos, initialHealth, frames, scale);

    if (isPositionValid(newMonster->getBounds())) {
        mMonsters.push_back(newMonster);
    } else {
        delete newMonster;
    }
}

void Game::applyVolume() {
    mBgMusic.setVolume(mVolume);
    mJumpSound.setVolume(mVolume);
    mShootSound.setVolume(mVolume);
    mLoseSound.setVolume(mVolume);
}

void Game::processSettingsEvents(sf::Vector2f mousePos) {
    if (mEasyBox.getGlobalBounds().contains(mousePos)) {
        mDifficulty = Difficulty::Easy;
    } else if (mMediumBox.getGlobalBounds().contains(mousePos)) {
        mDifficulty = Difficulty::Medium;
    } else if (mHardBox.getGlobalBounds().contains(mousePos)) {
        mDifficulty = Difficulty::Hard;
    } else if (mBackButton->getGlobalBounds().contains(mousePos)) {
        mState = GameState::Menu;
    }

    sf::FloatRect sliderArea({100.f, 210.f}, {300.f, 40.f});
    if (sliderArea.contains(mousePos)) {
        mVolume = ((mousePos.x - 125.f) / 250.f) * 100.f;
        if (mVolume < 0.f) mVolume = 0.f;
        if (mVolume > 100.f) mVolume = 100.f;
        applyVolume();
    }
}

void Game::spawnBlackHole(float baseY) {
    if (mDifficulty != Difficulty::Hard) return; 
    if (baseY > -300.f) return; 

    if (rand() % 100 < 15) { 
        float safeY = baseY - 80.f;

        // Prevent spawning black hole on the same Y level as moving platforms or monsters
        for (auto plat : mPlatforms) {
            if (dynamic_cast<MovingPlatform*>(plat)) {
                if (std::abs(plat->getPosition().y - safeY) < 70.f) return;
            }
        }
        for (auto monster : mMonsters) {
            if (std::abs(monster->getPosition().y - safeY) < 80.f) return;
        }

        bool placed = false;
        int attempts = 0;
        BlackHole* newBH = nullptr;

        while (!placed && attempts < 10) {
            float safeX = (rand() % 360) + 20.f; // 20 to 380
            sf::Vector2f pos(safeX, safeY);
            
            newBH = new BlackHole(mTextures.get("black_hole"), pos);
            if (rand() % 2 == 0) {
                newBH->setScale(0.6f);
            } else {
                newBH->setScale(1.0f);
            }

            bool tooClose = false;
            int solidPlatformCount = 0;
            const Platform* singlePlat = nullptr;

            for (auto plat : mPlatforms) {
                if (!dynamic_cast<MovingPlatform*>(plat)) {
                    sf::Vector2f platPos = plat->getPosition();
                    if (std::abs(platPos.y - baseY) < 120.f && !dynamic_cast<BreakablePlatform*>(plat)) {
                        solidPlatformCount++;
                        singlePlat = plat;
                    }

                    if (std::abs(platPos.y - safeY) < 100.f) {
                        // Ensure it's horizontally far away to avoid intersection
                        if (std::abs(platPos.x - safeX) < 110.f) { 
                            tooClose = true;
                            break;
                        }
                    }
                }
            }

            // Ensure path up is not blocked if only 1 solid platform
            if (!tooClose && solidPlatformCount <= 1 && singlePlat != nullptr) {
                float platX = singlePlat->getPosition().x;
                if (std::abs(safeX - platX) < 110.f) {
                    tooClose = true;
                }
            }

            if (!tooClose && isPositionValid(newBH->getBounds())) {
                placed = true;
            } else {
                delete newBH;
                newBH = nullptr;
                attempts++;
            }
        }

        if (placed && newBH != nullptr) {
            mBlackHoles.push_back(newBH);
        }
    }
}