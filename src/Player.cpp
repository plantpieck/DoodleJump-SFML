#include "../include/Player.hpp"

Player::Player(sf::Texture& leftTex, sf::Texture& rightTex, sf::Texture& shootTex) 
    : mSprite(rightTex), mLeftTexture(&leftTex), mRightTexture(&rightTex), mShootTexture(&shootTex),
      mSpeed(300.f), mGravity(800.f), mJumpVelocity(-600.f), mIsShooting(false) {
    mSprite.setPosition({250.f, 400.f});
    mSprite.setScale({0.55f, 0.55f});
}

void Player::handleInput() {
    mVelocity.x = 0.f;
    mIsShooting = false;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        mIsShooting = true;
        mSprite.setTexture(*mShootTexture);
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            mVelocity.x = -mSpeed;
            mSprite.setTexture(*mLeftTexture);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            mVelocity.x = mSpeed;
            mSprite.setTexture(*mRightTexture);
        } else {
            if (mSprite.getTexture() == mShootTexture) {
                mSprite.setTexture(*mRightTexture); 
            }
        }
    }
}

void Player::update(float dt) {
    mVelocity.y += mGravity * dt;
    mSprite.move({mVelocity.x * dt, mVelocity.y * dt});

    sf::Vector2f pos = mSprite.getPosition();
    if (pos.x < -50.f) {
        mSprite.setPosition({500.f, pos.y});
    } else if (pos.x > 500.f) {
        mSprite.setPosition({-50.f, pos.y});
    }
}

void Player::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

void Player::jump() {
    mVelocity.y = mJumpVelocity;
}

void Player::superJump() {
    mVelocity.y = mJumpVelocity * 1.8f;
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

bool Player::isShooting() const {
    return mIsShooting;
}