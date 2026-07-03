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

void Platform::move(float offsetX, float offsetY) {
    mSprite.move({offsetX, offsetY});
}

sf::Vector2f Platform::getPosition() const {
    return mSprite.getPosition();
}