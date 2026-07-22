#include "../include/Bullet.hpp"

Bullet::Bullet(sf::Texture& texture, sf::Vector2f startPosition) 
    : mSprite(texture), mSpeed(500.f) {
    mSprite.setPosition(startPosition);
    mSprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
}

void Bullet::update(float dt) {
    mSprite.move({0.f, -mSpeed * dt});
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

sf::FloatRect Bullet::getBounds() const {
    return mSprite.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() const {
    return mSprite.getPosition();
}