#include "../include/Monster.hpp"

Monster::Monster(sf::Texture& texture, sf::Vector2f position, int initialHealth, int frames, float scale) 
    : mSprite(texture), mSpeed(120.f), mDirection(1), mHealth(initialHealth), mIsDead(false), mScaleFactor(scale), mFrames(frames) {
    
    mTexWidth = texture.getSize().x / mFrames;
    mTexHeight = texture.getSize().y;
    
    mSprite.setTextureRect(sf::IntRect({0, 0}, {mTexWidth, mTexHeight}));
    mSprite.setOrigin({mTexWidth / 2.f, mTexHeight / 2.f});
    
    mSprite.setPosition(position);
    mSprite.setScale({mScaleFactor, mScaleFactor});
}

void Monster::update(float dt) {
    float moveX = mSpeed * mDirection * dt;
    move(moveX, 0.f);
    
    sf::Vector2f pos = getPosition();
    float halfWidth = mSprite.getGlobalBounds().size.x / 2.f;

    if (pos.x - halfWidth < 0.f) {
        mDirection = 1;
        if (mFrames > 1) {
            mSprite.setTextureRect(sf::IntRect({0, 0}, {mTexWidth, mTexHeight}));
        } else {
            mSprite.setScale({mScaleFactor, mScaleFactor});
        }
        mSprite.setPosition({halfWidth, pos.y}); 
    } else if (pos.x + halfWidth > 500.f) {
        mDirection = -1;
        if (mFrames > 1) {
            mSprite.setTextureRect(sf::IntRect({mTexWidth, 0}, {mTexWidth, mTexHeight}));
        } else {
            mSprite.setScale({-mScaleFactor, mScaleFactor});
        }
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