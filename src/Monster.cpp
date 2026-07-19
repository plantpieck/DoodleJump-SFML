#include "../include/Monster.hpp"

Monster::Monster(sf::Texture& texture, sf::Vector2f position, int initialHealth) 
    : mSprite(texture), mSpeed(120.f), mDirection(1), mHealth(initialHealth) {
    mSprite.setPosition(position);
}

void Monster::update(float dt) {
    float moveX = mSpeed * mDirection * dt;
    move(moveX, 0.f);
    
    sf::Vector2f pos = getPosition();
    if (pos.x < 0.f) {
        mDirection = 1;
    } else if (pos.x + getBounds().size.x > 500.f) {
        mDirection = -1;
    }
}

void Monster::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

sf::FloatRect Monster::getBounds() const {
    return mSprite.getGlobalBounds();
}

sf::Vector2f Monster::getPosition() const {
    return mSprite.getPosition();
}

void Monster::move(float offsetX, float offsetY) {
    mSprite.move({offsetX, offsetY});
}

void Monster::takeDamage() {
    if (mHealth > 0) {
        mHealth--;
    }
}

bool Monster::isDead() const {
    return mHealth <= 0;
}