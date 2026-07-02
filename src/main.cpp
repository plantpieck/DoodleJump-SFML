#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include <vector>
#include <memory>
#include <random>
#include <string>
#include "../include/ResourceManager.hpp"
#include "../include/Player.hpp"
#include "../include/NormalPlatform.hpp"
#include "../include/MovingPlatform.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Doodle Jump - Phase 1");
    window.setFramerateLimit(60);

    ResourceManager<sf::Texture, std::string> textures;
    textures.load("doodle_left", "assets/left_doodle.png");
    textures.load("doodle_right", "assets/right_doodle.png");
    textures.load("platform_normal", "assets/normal_platform.png");
    textures.load("platform_moving", "assets/moving_platform.png");

    sf::Font mainFont;
    mainFont.openFromFile("assets/arial.ttf");

    Player player(textures.get("doodle_left"), textures.get("doodle_right"));
    
    std::vector<std::unique_ptr<Platform>> platforms;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0.f, 700.f);
    std::uniform_real_distribution<float> yDist(80.f, 150.f);
    std::uniform_int_distribution<int> typeDist(1, 10);

    platforms.push_back(std::make_unique<NormalPlatform>(textures.get("platform_normal"), sf::Vector2f({400.f, 550.f})));
    
    for (int i = 0; i < 10; ++i) {
        float newX = xDist(gen);
        float newY = platforms.back()->getPosition().y - yDist(gen);
        platforms.push_back(std::make_unique<NormalPlatform>(textures.get("platform_normal"), sf::Vector2f({newX, newY})));
    }

    int score = 0;
    sf::Text scoreText(mainFont);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition({10.f, 10.f});

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        player.handleInput();
        player.update(dt.asSeconds());

        for (auto& platform : platforms) {
            platform->update(dt.asSeconds());
        }

        if (player.getPosition().y > 600.f) {
            window.close(); 
        }

        if (player.getPosition().y < 300.f) {
            float diff = 300.f - player.getPosition().y;
            player.setPosition({player.getPosition().x, 300.f});
            
            score += static_cast<int>(diff);
            
            for (auto& platform : platforms) {
                platform->move(0.f, diff);
            }
        }

        if (player.getVelocityY() > 0.f) {
            sf::FloatRect playerBounds = player.getBounds();
            for (auto& platform : platforms) {
                sf::FloatRect platformBounds = platform->getBounds();
                if (playerBounds.findIntersection(platformBounds).has_value()) {
                    if (playerBounds.position.y + playerBounds.size.y < platformBounds.position.y + platformBounds.size.y) {
                        player.jump();
                        break;
                    }
                }
            }
        }

        while (!platforms.empty() && platforms.front()->getPosition().y > 600.f) {
            platforms.erase(platforms.begin());
            float newX = xDist(gen);
            float newY = platforms.back()->getPosition().y - yDist(gen);
            
            if (typeDist(gen) > 7) {
                platforms.push_back(std::make_unique<MovingPlatform>(textures.get("platform_moving"), sf::Vector2f({newX, newY})));
            } else {
                platforms.push_back(std::make_unique<NormalPlatform>(textures.get("platform_normal"), sf::Vector2f({newX, newY})));
            }
        }

        scoreText.setString("Score: " + std::to_string(score));

        window.clear(sf::Color::White);
        
        for (auto& platform : platforms) {
            platform->render(window);
        }
        player.render(window);
        window.draw(scoreText);
        
        window.display();
    }

    return 0;
}