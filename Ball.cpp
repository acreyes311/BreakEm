#include "Ball.h"

// starting x and y position
Ball::Ball(float mX, float mY)
{
    // Apply ball properties
    ballShape.setPosition(mX, mY);
    ballShape.setRadius(ballRadius);
    ballShape.setFillColor(sf::Color(234, 255, 230));
    //ballShape.setOutlineColor(sf::Color::Red);
    // ballShape.setOutlineThickness(5);
    ballShape.setOrigin(ballRadius, ballRadius);
}

void Ball::moveBall()
{
    // move takes in Vector2f
    ballShape.move(velocity);

    // To keep ball inside screen
    // SFML starts top-left (0,0)
    // If leaving toward left, set horizontal velocity to positive value
    if (left() < 0)
        velocity.x = ballVelocity;
    // Else if leaving towards right, set horizontal velocity to negative
    else if (right() > windowWidth)
        velocity.x = -ballVelocity;

    // Apply same idea for top/bottom
    if (top() < 0)
        velocity.y = ballVelocity;
    else if (bottom() > windowHeight)
        velocity.y = -ballVelocity;
}

void Ball::setXVelocity(int vel)
{
    if (vel > 0)
        this->velocity.x = ballVelocity;
    else
        this->velocity.x = -ballVelocity;
}

void Ball::setYVelocity(int vel)
{
    if (vel > 0)
        this->velocity.y = ballVelocity;
    else
        this->velocity.y = -ballVelocity;
}

// Property Methods
float Ball::x() { return ballShape.getPosition().x; }

float Ball::y() { return ballShape.getPosition().y; }

float Ball::left() { return x() - ballShape.getRadius(); }

float Ball::right() { return x() + ballShape.getRadius(); }

float Ball::top() { return y() - ballShape.getRadius(); }

float Ball::bottom() { return y() + ballShape.getRadius(); }
