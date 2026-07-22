#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite mSprite;
    sf::Texture* mLeftTexture;
    sf::Texture* mRightTexture;
    sf::Vector2f mVelocity;
    sf::Texture* mShootTexture;
    bool mIsShooting;
    float mSpeed;
    float mGravity;
    float mJumpVelocity;

public:
    Player(sf::Texture& leftTex, sf::Texture& rightTex, sf::Texture& shootTex);
    bool isShooting() const;
    void handleInput();
    void update(float dt);
    void render(sf::RenderWindow& window);
    void jump();
    void superJump();
    float getVelocityY() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
};