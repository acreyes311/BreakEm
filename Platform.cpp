#include "Platform.h"

Platform::Platform(float mX, float mY)
{
    // Apply platform properties
    rectangle.setPosition(mX, mY);
    rectangle.setSize({platformWidth, platformHeight});
    rectangle.setFillColor(sf::Color(230, 251, 255));
    rectangle.setOrigin(platformWidth / 2.f, platformHeight / 2.f);
}

void Platform::movePlatform()
{
    rectangle.move(velocity);
    // User move platform with left and right arrow key.
    //  -> change velocity
    // To keep paddle inside window change velocity only if pos. is inside window.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
        velocity.x = -platformVelocity;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < windowWidth)
        velocity.x = platformVelocity;
    else
        velocity.x = 0;
}