#include <iostream>
#include <SFML/Graphics.hpp>

void changeColour(sf::RectangleShape& shape) {
    int x = rand() % 7 + 1; // Generate a random number from 1 to 7

    switch (x) {
    case 1:
        shape.setFillColor(sf::Color::Red);
        break;
    case 2:
        shape.setFillColor(sf::Color::Cyan);
        break;
    case 3:
        shape.setFillColor(sf::Color::Blue);
        break;
    case 4:
        shape.setFillColor(sf::Color::Green);
        break;
    case 5:
        shape.setFillColor(sf::Color::Magenta);
        break;
    case 6:
        shape.setFillColor(sf::Color::Yellow);
        break;
    case 7:
        shape.setFillColor(sf::Color::White);
        break;
    default:
        break;
    }
}


int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "My window");
    sf::RectangleShape rect;
    sf::Vector2f rectanglePosition(600, 350);
    rect.setPosition(rectanglePosition);
    rect.setSize(sf::Vector2f(100, 100));
    rect.setFillColor(sf::Color::Red);

    float xVelocity = 0.1;
    float yVelocity = 0.1;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }
        //Physics

        if (rectanglePosition.x < 0 || rectanglePosition.x > 1280 - 100) {
            xVelocity *= -1;
            //changeColour(rect);
        }
        if (rectanglePosition.y < 0 || rectanglePosition.y > 720 - 100){
            yVelocity *= -1;
            //changeColour(rect);
        }
        rectanglePosition.x += xVelocity;
        rectanglePosition.y += yVelocity;
        rect.setPosition(rectanglePosition);
        // clear the window with black color
        window.clear(sf::Color::Black);
        // draw everything here...
        // window.draw(...);
        window.draw(rect);
        // end the current frame
        window.display();
    }

    return 0;
}

