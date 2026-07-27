#include "BlackHole.hpp"

BlackHole::BlackHole(const sf::Texture& texture, sf::Vector2f position) {
    mSprite.setTexture(texture);
    mSprite.setPosition(position);
}

void BlackHole::update(float dt) {
    mSprite.rotate(50.f * dt);
}

void BlackHole::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

sf::FloatRect BlackHole::getBounds() const {
    return mSprite.getGlobalBounds();
}

sf::Vector2f BlackHole::getPosition() const {
    return mSprite.getPosition();
}