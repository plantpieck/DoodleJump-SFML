#include "../include/Platform.hpp"

Platform::Platform(sf::Texture& texture, sf::Vector2f position) : mSprite(texture) {
    mSprite.setPosition(position);
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

sf::FloatRect Platform::getBounds() const {
    return mSprite.getGlobalBounds();
}