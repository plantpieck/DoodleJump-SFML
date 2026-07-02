#pragma once
#include "Platform.hpp"

class MovingPlatform : public Platform {
private:
    float mSpeed;
    int mDirection;

public:
    MovingPlatform(sf::Texture& texture, sf::Vector2f position);
    void update(float dt) override;
};