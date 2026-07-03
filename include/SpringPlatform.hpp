#pragma once
#include "NormalPlatform.hpp"

class SpringPlatform : public NormalPlatform {
private:
    sf::Sprite mSpringSprite;

public:
    SpringPlatform(sf::Texture& platTex, sf::Texture& springTex, sf::Vector2f position);
    void render(sf::RenderWindow& window) override;
    sf::FloatRect getSpringBounds() const;
};