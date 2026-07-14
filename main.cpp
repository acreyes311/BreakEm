/*
 * Andrew Reyes
 * Simple brick breaker game project using SFML 3.0.1.
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
void processEvents(sf::RenderWindow &window, bool &shouldExit);
void updateGame(Ball &ball, Platform &platform, std::vector<Brick> &bricks);
void drawMenu(sf::RenderWindow &window,
              const sf::Text &titleText,
              const sf::Text &startText,
              const sf::Text &exitText,
              int selectedOption);
void renderGame(sf::RenderWindow &window,
                sf::RectangleShape &wallpaper,
                const Ball &ball,
                const Platform &platform,
                const std::vector<Brick> &bricks,
                const sf::Text &scoreText,
                const sf::Text &escText);

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

    // Load Font from File and set Text.
    // Position at bottom right.
    sf::Font font;
    if (!font.openFromFile("ArcheryBlack.ttf"))
        return 1;

    // Menu text objects
    sf::Text titleText(font, "BreakEm", 48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({windowWidth / 2.f - 90.f, windowHeight / 3.f});

    sf::Text startText(font, "Start", 30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({windowWidth / 2.f - 45.f, windowHeight / 2.f});

    sf::Text exitText(font, "Exit", 30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({windowWidth / 2.f - 35.f, windowHeight / 2.f + 60.f});

    bool inMenu = true;
    int selectedOption = 0; // 0 for Start, 1 for Exit

    // Game Loop
    while (window.isOpen())
    {
        bool shouldExit = false;
        processEvents(window, shouldExit);
        if (shouldExit)
            break;

        if (inMenu)
        {
            // Move the menu selection with the arrow keys or W/S.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                selectedOption = 1;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                selectedOption = 0;
            }

            drawMenu(window, titleText, startText, exitText, selectedOption);

            // Menu selection with Enter key or select exit.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                if (selectedOption == 0)
                {
                    inMenu = false;
                }
                else
                {
                    window.close();
                    break;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
                break;
            }

            continue;
        }

        Ball ball(windowWidth / 2.f, windowHeight / 2.f);
        Platform platform(windowWidth / 2.f, windowHeight - 65.f);
        std::vector<Brick> bricks;
        createBricks(bricks);

        sf::Text scoreText(font, "Lives: ", 14);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition({windowWidth - 100.f, windowHeight - 25.f});

        sf::Text escText(font, "Press 'Esc' to exit.", 14);
        escText.setFillColor(sf::Color::White);
        escText.setPosition({0.f, windowHeight - 25.f});

        while (window.isOpen())
        {
            bool shouldExitGame = false;
            processEvents(window, shouldExitGame);
            if (shouldExitGame)
                break;

            updateGame(ball, platform, bricks);
            renderGame(window, wallpaper, ball, platform, bricks, scoreText, escText);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                break;
        }

        inMenu = true;
    }

    return 0;
}

// Create the brick grid and center it horizontally inside the window.
void createBricks(std::vector<Brick> &bricks)
{
    constexpr float spacing = 3.f;
    constexpr float offsetY = 40.f;
    constexpr int columns = 11;
    constexpr int rows = 5;

    const float totalWidth = columns * blockWidth + (columns - 1) * spacing;
    const float startX = (windowWidth - totalWidth) / 2.f + blockWidth / 2.f;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            const float x = startX + col * (blockWidth + spacing);
            const float y = offsetY + row * (blockHeight + spacing);
            bricks.emplace_back(x, y);
        }
    }
}

// Handle close events and the Escape key so the game can shut down cleanly.
void processEvents(sf::RenderWindow &window, bool &shouldExit)
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>() ||
            (event->is<sf::Event::KeyPressed>() &&
             event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
        {
            shouldExit = true;
            window.close();
        }
    }
}

// Move the ball and platform, check for collisions, and remove hit bricks from the vector.
void updateGame(Ball &ball, Platform &platform, std::vector<Brick> &bricks)
{
    ball.moveBall();
    platform.movePlatform();

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
}

// Draw the main menu with a simple highlighted selection.
void drawMenu(sf::RenderWindow &window,
              const sf::Text &titleText,
              const sf::Text &startText,
              const sf::Text &exitText,
              int selectedOption)
{
    window.clear(sf::Color::Black);
    window.draw(titleText);

    sf::Text selectedStart = startText;
    sf::Text selectedExit = exitText;

    selectedStart.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    selectedExit.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);

    window.draw(selectedStart);
    window.draw(selectedExit);
    window.display();
}

void renderGame(sf::RenderWindow &window,
                sf::RectangleShape &wallpaper,
                const Ball &ball,
                const Platform &platform,
                const std::vector<Brick> &bricks,
                const sf::Text &scoreText,
                const sf::Text &escText)
{
    window.clear();
    window.draw(wallpaper);
    window.draw(ball.ballShape);
    window.draw(platform.rectangle);
    window.draw(scoreText);
    window.draw(escText);

    for (const auto &brick : bricks)
        window.draw(brick.brickShape);

    window.display();
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
