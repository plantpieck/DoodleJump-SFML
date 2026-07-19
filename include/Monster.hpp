#pragma once
#include <SFML/Graphics.hpp>

class Monster {
private:
    sf::Sprite mSprite;
    float mSpeed;
    int mDirection;
    int mHealth;

public:
    Monster(sf::Texture& texture, sf::Vector2f position, int initialHealth);
    void update(float dt);
    void render(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void move(float offsetX, float offsetY);
    
    void takeDamage();
    bool isDead() const;
};