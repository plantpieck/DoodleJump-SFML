#include "../include/Player.hpp"

Player::Player(sf::Texture& leftTex, sf::Texture& rightTex) 
    : mSprite(rightTex), mLeftTexture(&leftTex), mRightTexture(&rightTex), mSpeed(300.f), mGravity(800.f), mJumpVelocity(-600.f) {
    mSprite.setPosition({400.f, 300.f});
}

void Player::handleInput() {
    mVelocity.x = 0.f;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        mVelocity.x = -mSpeed;
        mSprite.setTexture(*mLeftTexture);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        mVelocity.x = mSpeed;
        mSprite.setTexture(*mRightTexture);
    }
}

void Player::update(float dt) {
    mVelocity.y += mGravity * dt;
    mSprite.move({mVelocity.x * dt, mVelocity.y * dt});

    sf::Vector2f pos = mSprite.getPosition();
    if (pos.x < -50.f) {
        mSprite.setPosition({800.f, pos.y});
    } else if (pos.x > 800.f) {
        mSprite.setPosition({-50.f, pos.y});
    }
}

void Player::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

void Player::jump() {
    mVelocity.y = mJumpVelocity;
}

float Player::getVelocityY() const {
    return mVelocity.y;
}

sf::FloatRect Player::getBounds() const {
    return mSprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return mSprite.getPosition();
}

void Player::setPosition(sf::Vector2f pos) {
    mSprite.setPosition(pos);
}