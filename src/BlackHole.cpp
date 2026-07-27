#include "BlackHole.hpp"

BlackHole::BlackHole(const sf::Texture& texture, sf::Vector2f position)
    : mSprite(texture) {
    mSprite.setOrigin({mSprite.getLocalBounds().size.x / 2.f, mSprite.getLocalBounds().size.y / 2.f});
    mSprite.setPosition(position);
}

void BlackHole::update(float dt) {
    mSprite.rotate(sf::degrees(45.f * dt));
}

void BlackHole::render(sf::RenderWindow& window) {
    window.draw(mSprite);
}

void BlackHole::setScale(float factor) {
    mSprite.setScale({factor, factor});
}

sf::Vector2f BlackHole::getPosition() const {
    return mSprite.getPosition();
}

sf::FloatRect BlackHole::getBounds() const {
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    
    // Reduce the hitbox size by 40% (20% padding on each side) to lower the sucking range
    float shrinkX = bounds.size.x * 0.20f;
    float shrinkY = bounds.size.y * 0.20f;
    
    bounds.position.x += shrinkX;
    bounds.position.y += shrinkY;
    bounds.size.x -= shrinkX * 2.f;
    bounds.size.y -= shrinkY * 2.f;
    
    return bounds;
}