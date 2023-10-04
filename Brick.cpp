#include "Brick.h"

Brick::Brick(float mX, float mY)
{
    // Apply brick properties
    brickShape.setPosition(mX, mY);
    brickShape.setSize({blockWidth, blockHeight});
    brickShape.setFillColor(sf::Color(0, 153, 25));
    brickShape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
}