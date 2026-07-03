#pragma once
#include "Platform.hpp"

class BreakablePlatform : public Platform {
private:
    bool mIsBroken;
    float mFallSpeed;

public:
    BreakablePlatform(sf::Texture& texture, sf::Vector2f position);
    void update(float dt) override;
    void breakPlatform();
    bool isBroken() const;
};