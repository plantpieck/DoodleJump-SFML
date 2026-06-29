#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite mSprite;
    sf::Texture* mLeftTexture;
    sf::Texture* mRightTexture;
    sf::Vector2f mVelocity;
    float mSpeed;

public:
    Player(sf::Texture& leftTex, sf::Texture& rightTex);
    void handleInput();
    void update(float dt);
    void render(sf::RenderWindow& window);
};