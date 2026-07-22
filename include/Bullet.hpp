#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::Sprite mSprite;
    float mSpeed;

public:
    Bullet(sf::Texture& texture, sf::Vector2f startPosition);
    void update(float dt);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
};