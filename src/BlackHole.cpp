#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f position) {
    mShape.setRadius(6.f);
    mShape.setFillColor(sf::Color(255, 215, 0));
    mShape.setOrigin({6.f, 6.f});
    mShape.setPosition(position);
}

void Bullet::update(float dt) {
    mShape.move({0.f, -800.f * dt});
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(mShape);
}

sf::FloatRect Bullet::getBounds() const {
    return mShape.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() const {
    return mShape.getPosition();
}