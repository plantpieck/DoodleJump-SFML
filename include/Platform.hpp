#pragma once
#include <SFML/Graphics.hpp>

class Platform {
protected:
    sf::Sprite mSprite;
public:
    Platform(sf::Texture& texture, sf::Vector2f position);
    virtual ~Platform() = default;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void move(float offsetX, float offsetY);
    sf::Vector2f getPosition() const;
};