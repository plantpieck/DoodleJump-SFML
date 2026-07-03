#include "../include/Game.hpp"
#include "../include/NormalPlatform.hpp"
#include "../include/MovingPlatform.hpp"
#include "../include/BreakablePlatform.hpp"
#include <SFML/Window/Event.hpp>
#include <fstream>
#include <random>
#include <iostream>

Game::Game() : mWindow(sf::VideoMode({800, 600}), "Doodle Jump - Phase 1"), mState(GameState::Menu), mScore(0), mHighScore(0) {
    mWindow.setFramerateLimit(60);
    loadResources();
    loadHighScore();
    mPlayer = std::make_unique<Player>(mTextures.get("doodle_left"), mTextures.get("doodle_right"));
    resetGame();
}

void Game::loadResources() {
    mTextures.load("doodle_left", "assets/left_doodle.png");
    mTextures.load("doodle_right", "assets/right_doodle.png");
    mTextures.load("platform_normal", "assets/normal_platform.png");
    mTextures.load("platform_moving", "assets/moving_platform.png");
    mTextures.load("platform_breakable", "assets/breakable_platform.png");
    
    if (!mFont.openFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load font!\n";
    }
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
    mScore = 0;
    mPlatforms.clear();
    mPlayer->setPosition({400.f, 300.f});
    
    mPlatforms.push_back(std::make_unique<NormalPlatform>(mTextures.get("platform_normal"), sf::Vector2f({400.f, 550.f})));
    generatePlatforms(550.f);
}

void Game::generatePlatforms(float startY) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0.f, 700.f);
    std::uniform_real_distribution<float> yDist(80.f, 130.f);
    std::uniform_int_distribution<int> typeDist(1, 100);

    float currentY = startY;
    for (int i = 0; i < 15; ++i) {
        float newX = xDist(gen);
        currentY -= yDist(gen);
        
        int r = typeDist(gen);
        if (r <= 60) {
            mPlatforms.push_back(std::make_unique<NormalPlatform>(mTextures.get("platform_normal"), sf::Vector2f({newX, currentY})));
        } else if (r <= 85) {
            mPlatforms.push_back(std::make_unique<MovingPlatform>(mTextures.get("platform_moving"), sf::Vector2f({newX, currentY})));
        } else {
            mPlatforms.push_back(std::make_unique<BreakablePlatform>(mTextures.get("platform_breakable"), sf::Vector2f({newX, currentY})));
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
                if (mState == GameState::Menu) {
                    mState = GameState::Playing;
                    resetGame();
                } else if (mState == GameState::GameOver) {
                    mState = GameState::Menu;
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

        if (mPlayer->getPosition().y < 300.f) {
            float diff = 300.f - mPlayer->getPosition().y;
            mPlayer->setPosition({mPlayer->getPosition().x, 300.f});
            mScore += static_cast<int>(diff);
            
            for (auto& platform : mPlatforms) {
                platform->move(0.f, diff);
            }
        }

        while (!mPlatforms.empty() && mPlatforms.front()->getPosition().y > 600.f) {
            mPlatforms.erase(mPlatforms.begin());
            generatePlatforms(mPlatforms.back()->getPosition().y);
        }

        if (mPlayer->getPosition().y > 600.f) {
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
                    
                    if (auto breakable = dynamic_cast<BreakablePlatform*>(platform.get())) {
                        if (!breakable->isBroken()) {
                            breakable->breakPlatform();
                        }
                    } else {
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

    if (mState == GameState::Playing) {
        for (auto& platform : mPlatforms) {
            platform->render(mWindow);
        }
        mPlayer->render(mWindow);
        
        sf::Text scoreText(mFont, "Score: " + std::to_string(mScore), 24);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition({10.f, 10.f});
        mWindow.draw(scoreText);
        
    } else if (mState == GameState::Menu) {
        sf::Text titleText(mFont, "DOODLE JUMP\nClick to Start\nHigh Score: " + std::to_string(mHighScore), 36);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition({250.f, 250.f});
        mWindow.draw(titleText);
        
    } else if (mState == GameState::GameOver) {
        sf::Text overText(mFont, "YOU LOST!\nScore: " + std::to_string(mScore) + "\nHigh Score: " + std::to_string(mHighScore) + "\nClick to Menu", 36);
        overText.setFillColor(sf::Color::Red);
        overText.setPosition({250.f, 250.f});
        mWindow.draw(overText);
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