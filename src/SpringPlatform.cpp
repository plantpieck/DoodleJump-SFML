#include "../include/SpringPlatform.hpp"

SpringPlatform::SpringPlatform(sf::Texture& platTex, sf::Texture& springTex, sf::Vector2f position)
    : NormalPlatform(platTex, position), mSpringSprite(springTex), mIsCompressed(false) {
    
    int width = springTex.getSize().x;
    int height = springTex.getSize().y / 2;
    mSpringSprite.setTextureRect(sf::IntRect({0, 0}, {width, height}));
}

void SpringPlatform::render(sf::RenderWindow& window) {
    NormalPlatform::render(window);
    
    sf::Vector2f platPos = getPosition();
    sf::FloatRect platBounds = getBounds();
    sf::FloatRect springBounds = mSpringSprite.getGlobalBounds();
    
    float springX = platPos.x + (platBounds.size.x - springBounds.size.x) / 2.f;
    float springY = platPos.y - springBounds.size.y + 2.f;
    
    mSpringSprite.setPosition({springX, springY});
    window.draw(mSpringSprite);
}

sf::FloatRect SpringPlatform::getSpringBounds() const {
    return mSpringSprite.getGlobalBounds();
}

void SpringPlatform::compress() {
    if (!mIsCompressed) {
        mIsCompressed = true;
        int width = mSpringSprite.getTexture().getSize().x;
        int height = mSpringSprite.getTexture().getSize().y / 2;
        mSpringSprite.setTextureRect(sf::IntRect({0, height}, {width, height}));
    }
}