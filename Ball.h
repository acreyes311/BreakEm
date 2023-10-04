#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Constants.h"
class Ball
{
private:
    // sf::Vector2 a class that defines a mathematical vector with two coordinates (x and y).
    // Will be used to store Ball Velocity
    // Negative vector that moves to top and left of screen
    sf::Vector2f velocity{-ballVelocity, -ballVelocity};

public:
    // Ball constructor
    Ball(float mX, float mY);

    // update ball (move shape) by current velocity
    void moveBall();

    // set velocity functions. + or -
    void setXVelocity(int);
    void setYVelocity(int);

    // Property Methods
    float x();
    float y();
    float left();
    float right();
    float top();
    float bottom();

    // SMFL class CircleShape defines a renderable circle shape
    sf::CircleShape ballShape;
};
#endif