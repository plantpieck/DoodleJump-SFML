#include "../include/SpringPlatform.hpp"

SpringPlatform::SpringPlatform(sf::Texture& platTex, sf::Texture& springTex, sf::Vector2f position)
    : NormalPlatform(platTex, position), mSpringSprite(springTex) {
}

void SpringPlatform::render(sf::RenderWindow& window) {
    NormalPlatform::render(window);
    
    sf::Vector2f platPos = getPosition();
    sf::FloatRect platBounds = getBounds();
    sf::FloatRect springBounds = mSpringSprite.getGlobalBounds();
    
    float springX = platPos.x + (platBounds.size.x - springBounds.size.x) / 2.f;
    float springY = platPos.y - springBounds.size.y + 5.f;
    
    mSpringSprite.setPosition({springX, springY});
    
    window.draw(mSpringSprite);
}

sf::FloatRect SpringPlatform::getSpringBounds() const {
    return mSpringSprite.getGlobalBounds();
}