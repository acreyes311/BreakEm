/*
 * Andrew Reyes
 * Simple brick breaker game project using SFML.
 * date: 7-5-26
 * SFML doc: https://www.sfml-dev.org/documentation/2.5.1/index.php
 */

#include "Ball.h"
#include "Platform.h"
#include "Brick.h"
#include "Constants.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <optional>
#include <vector>

// https://i.stack.imgur.com/bgjGx.png

template <class T1, class T2>
bool intersect(T1 &objA, T2 &objB);

void testCollision(Ball &myBall, Platform &myPlatform);
void testCollision(Ball &myBall, Brick &myBrick);
void createBricks(std::vector<Brick> &bricks);
void processEvents(sf::RenderWindow &window);
// void updateGame();
// void render();

int main()
{
    // Create Window 800x600, that can serve as 2D drawing
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "BreakEm");
    window.setFramerateLimit(60);

    // Background Wallpaper
    sf::Texture texture;
    if (!texture.loadFromFile("background.png"))
    {
        return 1;
    }

    sf::RectangleShape wallpaper;
    wallpaper.setSize({windowWidth, windowHeight});
    wallpaper.setTexture(&texture);

    // Create ball at the center of window
    Ball ball(windowWidth / 2.f, windowHeight / 2.f);

    // Create platform at bottom of window
    Platform platform(windowWidth / 2.f, windowHeight - 65.f);

    // Vector of Bricks.
    std::vector<Brick> bricks;

    // Fill in bricks vector in a grid pattern with an offset between them.
    // 11 columns x 5 rows. 55 bricks for now
    const int columns = 11;
    const int rows = 5;

    for (int xCoord = 0; xCoord < columns; ++xCoord)
    {
        for (int yCoord = 0; yCoord < rows; ++yCoord)
        {
            const float x = (xCoord + 1) * (blockWidth + 3.f) + 22.f;
            const float y = (yCoord + 2) * (blockHeight + 3.f);
            bricks.emplace_back(x, y);
        }
    }

    // Load Font from File and set Text.
    // Position at bottom right.
    sf::Font font;
    if (!font.openFromFile("ArcheryBlack.ttf"))
        return 1;

    sf::Text scoreText(font, "Lives: ", 14);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({windowWidth - 100, windowHeight - 25});

    sf::Text escText(font, "Press 'Esc' to exit.", 14);
    escText.setFillColor(sf::Color::White);
    escText.setPosition({0, windowHeight - 25});

    // Game Loop
    while (window.isOpen())
    {
        // Clear window with color
        // window.clear();

        // Press Escape to break loop and Exit game. Per SFML doc
        while (const std::optional event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
                window.close();
        }

        // Update and move the ball and platform every loop iteration
        ball.moveBall();
        platform.movePlatform();

        // Test collision every loop iteration
        testCollision(ball, platform);

        for (auto &brick : bricks)
            testCollision(ball, brick);

        /** Using erase-move idiom to remove all hit bricks from bricks vector
         *   effectively remove range from first destroyed brick to last.
         *   use of erase() and remove_if()
         *   auto iterator = remove_if(begin(bricks), end(bricks), [](const Brick& mBrick) return mBrick.destroyed;})
         */
        bricks.erase(std::remove_if(bricks.begin(), bricks.end(), [](const Brick &b)
                                    { return b.hasBeenHit; }),
                     bricks.end());

        // draw onto window
        window.clear();
        window.draw(wallpaper);
        window.draw(ball.ballShape);
        window.draw(platform.rectangle);
        window.draw(scoreText);
        window.draw(escText);

        // draw every brick onto window
        for (auto &brick : bricks)
            window.draw(brick.brickShape);

        // display rendered window
        window.display();
    }

    return 0;
}

/* to check if two shapes are intersecting (colliding).
 * Check if right side of first object is inside of second object
 * and left coordinate is  less than right coordinate of second object there is a intersection
 * repeat with top and bottom.
 */
template <class T1, class T2>
bool intersect(T1 &objA, T2 &objB)
{
    return objB.right() >= objA.left() && objB.left() <= objA.right() &&
           objB.bottom() >= objA.top() && objB.top() <= objA.bottom();
}

// test platform/ball collision.
void testCollision(Ball &myBall, Platform &myPlatform)
{
    // If no intersection -> return
    if (!intersect(myBall, myPlatform))
        return;

    // Else there is a collision -> move ball upwards
    // -1 for negative velocity +1 for positive.
    myBall.setYVelocity(-1);

    // direct it dependently on the position where platform was hit
    // if hit on left side push ball towards left, else towards right
    if (myBall.x() < myPlatform.x())
    {
        myBall.setXVelocity(-1);
    }
    else
    {
        myBall.setXVelocity(1);
    }
}

// test ball and brick collision
void testCollision(Ball &myBall, Brick &myBrick)
{
    // No intersection -> return
    if (!intersect(myBall, myBrick))
    {
        return;
    }

    // Otherwise brick has been hit.
    myBrick.hasBeenHit = true;

    // Calculate the magnitude of difference
    // how much the ball intersects the brick in every direction.
    // -> magnitude of overlap
    float magnitudeTop = myBall.bottom() - myBrick.top();
    float magnitudeBottom = myBrick.bottom() - myBall.top();
    float magnitudeLeft = myBall.right() - myBrick.left();
    float magnitudeRight = myBrick.right() - myBall.left();

    // if left < right then ball hit brick from left, else false and hit from right
    bool hitFromLeft(std::abs(magnitudeLeft) < std::abs(magnitudeRight));

    // if Top < Bottom then ball hit brick from top
    bool hitFromTop(std::abs(magnitudeTop) < std::abs(magnitudeBottom));

    // Minimum overlap for the X and Y axis
    float minX;
    float minY;

    if (hitFromLeft)
    {
        minX = magnitudeLeft;
    }
    else
    {
        minX = magnitudeRight;
    }

    if (hitFromTop)
    {
        minY = magnitudeTop;
    }
    else
    {
        minY = magnitudeBottom;
    }

    // If the magnitude of the X overlap is less than the magnitude
    // of the Y overlap, we can safely assume the ball hit the brick
    // horizontally - otherwise, the ball hit the brick vertically.

    // Then, upon our assumptions, we change either the X or Y velocity
    // of the ball, creating a "realistic" response for the collision.
    if (std::abs(minX) < std::abs(minY))
    {
        if (hitFromLeft)
        {
            myBall.setXVelocity(-1);
        }
        else
        {
            myBall.setXVelocity(1);
        }
    }
    else
    {
        if (hitFromTop)
        {
            myBall.setYVelocity(-1);
        }
        else
        {
            myBall.setYVelocity(1);
        }
    }
}
