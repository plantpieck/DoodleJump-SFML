#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include "../include/ResourceManager.hpp"
#include "../include/Player.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Doodle Jump - Phase 1");
    window.setFramerateLimit(60);

    ResourceManager<sf::Texture, std::string> textures;
    textures.load("doodle_left", "assets/left_doodle.png");
    textures.load("doodle_right", "assets/right_doodle.png");

    Player player(textures.get("doodle_left"), textures.get("doodle_right"));
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

        window.clear(sf::Color::White);
        player.render(window);
        window.display();
    }

    return 0;
}