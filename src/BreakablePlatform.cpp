#include "../include/BreakablePlatform.hpp"

BreakablePlatform::BreakablePlatform(sf::Texture& texture, sf::Vector2f position) 
    : Platform(texture, position), mIsBroken(false), mFallSpeed(0.f) {}

void BreakablePlatform::update(float dt) {
    if (mIsBroken) {
        mFallSpeed += 800.f * dt;
        move(0.f, mFallSpeed * dt);
    }
}

void BreakablePlatform::breakPlatform() {
    mIsBroken = true;
}

bool BreakablePlatform::isBroken() const {
    return mIsBroken;
}