#include "../include/Player.hpp"

Player::Player(sf::Texture& leftTex, sf::Texture& rightTex) 
    : mSprite(rightTex), mLeftTexture(&leftTex), mRightTexture(&rightTex), mSpeed(300.f) {
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
    mSprite.move({mVelocity.x * dt, mVelocity.y * dt});
}

void Player::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}