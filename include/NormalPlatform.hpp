#pragma once
#include "Platform.hpp"

class NormalPlatform : public Platform {
public:
    NormalPlatform(sf::Texture& texture, sf::Vector2f position);
    void update(float dt) override;
};