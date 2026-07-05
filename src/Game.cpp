#include "../include/Game.hpp"
#include "../include/NormalPlatform.hpp"
#include "../include/MovingPlatform.hpp"
#include "../include/BreakablePlatform.hpp"
#include "../include/SpringPlatform.hpp"
#include <SFML/Window/Event.hpp>
#include <fstream>
#include <random>
#include <iostream>

Game::Game() : mWindow(sf::VideoMode({500, 800}), "Doodle Jump - Phase 1"), mState(GameState::Menu), mScore(0), mHighScore(0) {
    mWindow.setFramerateLimit(60);
    mWorldView = mWindow.getDefaultView();
    loadResources();
    loadHighScore();
    mPlayer = std::make_unique<Player>(mTextures.get("doodle_left"), mTextures.get("doodle_right"));
    resetGame();
}

Game::~Game() {
    saveHighScore();
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
    
    if (!mFont.openFromFile("fonts/ariblk.ttf")) {
        std::cerr << "Failed to load font!\n";
    }

    mBackground = std::make_unique<sf::Sprite>(mTextures.get("background"));
    auto bgSize = mTextures.get("background").getSize();
    mBackground->setScale({500.f / static_cast<float>(bgSize.x), 800.f / static_cast<float>(bgSize.y)});

    mStartButton = std::make_unique<sf::Sprite>(mTextures.get("button_start"));
    mStartButton->setPosition({250.f - mStartButton->getGlobalBounds().size.x / 2.f, 400.f});

    mRestartButton = std::make_unique<sf::Sprite>(mTextures.get("button_restart"));
    mRestartButton->setPosition({250.f - mRestartButton->getGlobalBounds().size.x / 2.f, 420.f});

    mMenuButton = std::make_unique<sf::Sprite>(mTextures.get("button_menu"));
    mMenuButton->setPosition({250.f - mMenuButton->getGlobalBounds().size.x / 2.f, 520.f});
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> mHighScore;
        file.close();
    }
}

void Game::saveHighScore() {
    if (mScore > mHighScore) {
        mHighScore = mScore;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << mHighScore;
            file.close();
        }
    }
}

void Game::resetGame() {
    mWorldView.setCenter({250.f, 400.f});
    mScore = 0;
    mPlatforms.clear();
    mPlayer->setPosition({250.f, 600.f});
    
    mPlatforms.push_back(std::make_unique<NormalPlatform>(mTextures.get("platform_normal"), sf::Vector2f({250.f, 750.f})));
    generatePlatforms(750.f);
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
        float newX = xDist(gen);
        currentY -= yDist(gen);
        
        int r = typeDist(gen);
        if (r <= 65) {
            mPlatforms.push_back(std::make_unique<NormalPlatform>(mTextures.get("platform_normal"), sf::Vector2f({newX, currentY})));
            lastWasBreakable = false;
        } else if (r <= 80) {
            mPlatforms.push_back(std::make_unique<MovingPlatform>(mTextures.get("platform_moving"), sf::Vector2f({newX, currentY})));
            lastWasBreakable = false;
        } else if (r <= 85) {
            mPlatforms.push_back(std::make_unique<SpringPlatform>(mTextures.get("platform_normal"), mTextures.get("spring"), sf::Vector2f({newX, currentY})));
            lastWasBreakable = false;
        } else {
            if (lastWasBreakable) {
                mPlatforms.push_back(std::make_unique<NormalPlatform>(mTextures.get("platform_normal"), sf::Vector2f({newX, currentY})));
                lastWasBreakable = false;
            } else {
                mPlatforms.push_back(std::make_unique<BreakablePlatform>(mTextures.get("platform_breakable"), sf::Vector2f({newX, currentY})));
                lastWasBreakable = true;
            }
        }
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
                        resetGame();
                    }
                } else if (mState == GameState::GameOver) {
                    if (mRestartButton->getGlobalBounds().contains(mousePos)) {
                        mState = GameState::Playing;
                        resetGame();
                    } else if (mMenuButton->getGlobalBounds().contains(mousePos)) {
                        mState = GameState::Menu;
                    }
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (mState == GameState::Playing) {
        mPlayer->handleInput();
        mPlayer->update(dt);

        for (auto& platform : mPlatforms) {
            platform->update(dt);
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

        float bottomEdge = mWorldView.getCenter().y + 400.f;

        while (!mPlatforms.empty() && mPlatforms.front()->getPosition().y > bottomEdge) {
            mPlatforms.erase(mPlatforms.begin());
            generatePlatforms(mPlatforms.back()->getPosition().y);
        }

        if (playerY > bottomEdge) {
            saveHighScore();
            mState = GameState::GameOver;
        }
    }
}

void Game::handleCollisions() {
    if (mPlayer->getVelocityY() > 0.f) {
        sf::FloatRect playerBounds = mPlayer->getBounds();
        
        for (auto& platform : mPlatforms) {
            sf::FloatRect platformBounds = platform->getBounds();
            
            if (playerBounds.findIntersection(platformBounds).has_value()) {
                if (playerBounds.position.y + playerBounds.size.y < platformBounds.position.y + platformBounds.size.y) {
                    
                    if (auto springPlat = dynamic_cast<SpringPlatform*>(platform.get())) {
                        if (playerBounds.findIntersection(springPlat->getSpringBounds()).has_value()) {
                            mPlayer->superJump();
                        } else {
                            mPlayer->jump();
                        }
                        break;
                    } 
                    else if (auto breakable = dynamic_cast<BreakablePlatform*>(platform.get())) {
                        if (!breakable->isBroken()) {
                            breakable->breakPlatform();
                        }
                    } 
                    else {
                        mPlayer->jump();
                        break;
                    }
                }
            }
        }
    }
}

void Game::render() {
    mWindow.clear(sf::Color::White);
    
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(*mBackground);

    if (mState == GameState::Playing) {
        mWindow.setView(mWorldView);
        
        for (auto& platform : mPlatforms) {
            platform->render(mWindow);
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
        
        sf::Text titleText(mFont, "DOODLE JUMP", 44);
        titleText.setFillColor(sf::Color({20, 80, 120}));
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition({250.f - titleText.getGlobalBounds().size.x / 2.f, 200.f});
        mWindow.draw(titleText);

        sf::Text highText(mFont, "HIGH SCORE: " + std::to_string(mHighScore), 24);
        highText.setFillColor(sf::Color({50, 50, 50}));
        highText.setPosition({250.f - highText.getGlobalBounds().size.x / 2.f, 300.f});
        mWindow.draw(highText);

        mWindow.draw(*mStartButton);

        sf::Text hintText(mFont, "Use Left / Right arrows to move", 18);
        hintText.setFillColor(sf::Color({80, 80, 80}));
        hintText.setPosition({250.f - hintText.getGlobalBounds().size.x / 2.f, 550.f});
        mWindow.draw(hintText);
        
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

        sf::Text highText(mFont, "HIGH SCORE: " + std::to_string(mHighScore), 24);
        highText.setFillColor(sf::Color({50, 50, 50}));
        highText.setPosition({250.f - highText.getGlobalBounds().size.x / 2.f, 350.f});
        mWindow.draw(highText);

        mWindow.draw(*mRestartButton);
        mWindow.draw(*mMenuButton);
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