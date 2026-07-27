#pragma once
#include <SFML/Graphics.hpp>

class Monster {
private:
    sf::Sprite mSprite;
    float mSpeed;
    int mDirection;
    int mHealth;
    bool mIsDead;

public:
    Monster(sf::Texture& texture, sf::Vector2f position, int initialHealth, int frames = 1);
    
    void update(float dt);
    void render(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void move(float offsetX, float offsetY);
    
    void takeDamage();
    void dieInstantly(); 
    bool isDead() const;
};