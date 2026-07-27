#pragma once
#include <SFML/Graphics.hpp>

class BlackHole {
private:
    sf::Sprite mSprite;

public:
    BlackHole(const sf::Texture& texture, sf::Vector2f position);
    void update(float dt);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
};