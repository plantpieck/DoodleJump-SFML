#include "../include/Monster.hpp"

Monster::Monster(sf::Texture& texture, sf::Vector2f position, int initialHealth, int frames) 
    : mSprite(texture), mSpeed(120.f), mDirection(1), mHealth(initialHealth), mIsDead(false) {
    
    int texWidth = texture.getSize().x / frames;
    int texHeight = texture.getSize().y;
    
    mSprite.setTextureRect(sf::IntRect({0, 0}, {texWidth, texHeight}));
    mSprite.setOrigin({texWidth / 2.f, texHeight / 2.f});
    
    mSprite.setPosition(position);
    mSprite.setScale({1.f, 1.f});
}

void Monster::update(float dt) {
    float moveX = mSpeed * mDirection * dt;
    move(moveX, 0.f);
    
    sf::Vector2f pos = getPosition();
    float halfWidth = mSprite.getGlobalBounds().size.x / 2.f;

    if (pos.x - halfWidth < 0.f) {
        mDirection = 1;
        mSprite.setScale({1.f, 1.f}); 
        mSprite.setPosition({halfWidth, pos.y}); 
    } else if (pos.x + halfWidth > 500.f) {
        mDirection = -1;
        mSprite.setScale({-1.f, 1.f}); 
        mSprite.setPosition({500.f - halfWidth, pos.y}); 
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
    if (mHealth <= 0) {
        mIsDead = true;
    }
}

void Monster::dieInstantly() {
    mIsDead = true;
    mHealth = 0;
}

bool Monster::isDead() const {
    return mIsDead;
}