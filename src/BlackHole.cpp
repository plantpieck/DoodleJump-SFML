#include "BlackHole.hpp"

BlackHole::BlackHole(const sf::Texture& texture, sf::Vector2f position) : mSprite(texture) {
    mSprite.setPosition(position);
    mSprite.setOrigin({mSprite.getLocalBounds().size.x / 2.f, mSprite.getLocalBounds().size.y / 2.f});
}

void BlackHole::update(float dt) {
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