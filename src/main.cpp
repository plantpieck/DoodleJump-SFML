#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include <vector>
#include <memory>
#include "../include/ResourceManager.hpp"
#include "../include/Player.hpp"
#include "../include/NormalPlatform.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Doodle Jump - Phase 1");
    window.setFramerateLimit(60);

    ResourceManager<sf::Texture, std::string> textures;
    textures.load("doodle_left", "assets/left_doodle.png");
    textures.load("doodle_right", "assets/right_doodle.png");
    textures.load("platform_normal", "assets/normal_platform.png");

    Player player(textures.get("doodle_left"), textures.get("doodle_right"));
    
    std::vector<std::unique_ptr<Platform>> platforms;
    platforms.push_back(std::make_unique<NormalPlatform>(textures.get("platform_normal"), sf::Vector2f({400.f, 500.f})));
    platforms.push_back(std::make_unique<NormalPlatform>(textures.get("platform_normal"), sf::Vector2f({200.f, 300.f})));
    platforms.push_back(std::make_unique<NormalPlatform>(textures.get("platform_normal"), sf::Vector2f({600.f, 100.f})));

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

        window.clear(sf::Color::White);
        
        for (auto& platform : platforms) {
            platform->render(window);
        }
        player.render(window);
        
        window.display();
    }

    return 0;
}