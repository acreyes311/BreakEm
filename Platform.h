#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "Constants.h"
class Platform
{
private:
    // Vector2f for platform velocity
    sf::Vector2f velocity;

public:
    // Constructor
    Platform(float mX, float mY);

    // Update and move platform
    void movePlatform();

    // Property methods for Paddle
    float x() { return rectangle.getPosition().x; }
    float y() { return rectangle.getPosition().y; }
    float left() { return x() - rectangle.getSize().x / 2.f; }
    float right() { return x() + rectangle.getSize().x / 2.f; }
    float top() { return y() - rectangle.getSize().y / 2.f; }
    float bottom() { return y() + rectangle.getSize().y / 2.f; }

    // SFML RectangleShape class defines a renderable rectangle shape
    sf::RectangleShape rectangle;
};
#endif