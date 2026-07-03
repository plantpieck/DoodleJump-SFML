#include "../include/MovingPlatform.hpp"

MovingPlatform::MovingPlatform(sf::Texture& texture, sf::Vector2f position) 
    : Platform(texture, position), mSpeed(150.f), mDirection(1) {}

void MovingPlatform::update(float dt) {
    float moveX = mSpeed * mDirection * dt;
    move(moveX, 0.f);
    
    sf::Vector2f pos = getPosition();
    if (pos.x < 0.f) {
        mDirection = 1;
    } else if (pos.x + getBounds().size.x > 800.f) {
        mDirection = -1;
    }
}