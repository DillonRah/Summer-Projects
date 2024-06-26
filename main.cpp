#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> 
#include <cstdlib>
#include <random>
#include <ctime>  
#include <windows.h>     
#include <math.h>

using namespace std;

class Ball {
private:

public:
	sf::CircleShape shape;
	float radius;
	float position;
    float angle;
    float xvelocity;
    float yvelocity;
    float magnitude;

    void calculate_velocity(int magnitude, int scale) {
        float radians = angle * (3.14159265358979323846 / 180);
        // Using a scale because trying to edit the value directly from rand call was being buggy
        xvelocity = (cos(radians) * magnitude) / scale;
        yvelocity = (sin(radians) * magnitude) / scale;
    }

    void set_velocity(float x, float y) {
		xvelocity = x;
		yvelocity = y;
	}

    void update_position() {
		shape.move(xvelocity, yvelocity);
		// Update position
    }   

    void set_speed_and_direction(float magnitude, float degrees, float scale) {
        angle = degrees;
		calculate_velocity(magnitude, scale);
	}

};

int main() {
    int width = 1280;
    int height = 720;
    int numBalls = 100;
    vector<Ball> balls;

    for (int i = 0; i < numBalls; i++) {
        Ball particle;
        sf::CircleShape ball;
        ball.setRadius(rand() % 20 + 5);
        ball.setFillColor(sf::Color::White);
        ball.setPosition(rand() % width, rand() % height);
        particle.shape = ball;
        balls.push_back(particle);
	}
    for (auto& ball : balls) {
		ball.set_speed_and_direction((rand() % 10 + 5), rand() % 360, 30);
	}
    sf::RenderWindow window(sf::VideoMode(width, height), "My window");
    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        // Apply Sorting Algorithm here
        for (auto& ball : balls) {
            ball.update_position();

            if (ball.shape.getPosition().x < 0 || ball.shape.getPosition().x > width - ball.shape.getRadius() * 2) {
				ball.xvelocity *= -1;
            }
            if (ball.shape.getPosition().y < 0 || ball.shape.getPosition().y > height - ball.shape.getRadius() * 2) {
                ball.yvelocity *= -1;
            }
        }
        // Clear window
        window.clear(sf::Color::Black);

        // Draw items
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
        // Display everything
        window.display();
    }

    return 0;
}
