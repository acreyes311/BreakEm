#ifndef BRICK_H
#define BRICK_H

#include <SFML/Graphics.hpp>
#include "Constants.h"
class Brick
{

public:
    Brick(float mX, float mY);

    // get functions
    float getWidth() { return blockWidth; }
    float getHeight() { return blockHeight; }

    // Property functions
    float x() { return brickShape.getPosition().x; }
    float y() { return brickShape.getPosition().y; }
    float left() { return x() - brickShape.getSize().x / 2.f; }
    float right() { return x() + brickShape.getSize().x / 2.f; }
    float top() { return y() - brickShape.getSize().y / 2.f; }
    float bottom() { return y() + brickShape.getSize().y / 2.f; }

    // Flag to see if brick has been hit
    bool hasBeenHit = false;

    // Brick shape of rectangle
    sf::RectangleShape brickShape;
};
#endif