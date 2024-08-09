#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> 
#include <cstdlib>
#include <random>
#include <ctime>  
#include <windows.h>     
#include <math.h>
#include <map>

using namespace std;

// this class is horrendous I know, I don't care enough atm 
class Ball {

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
public:
    Ball(int radius, sf::Vector2f initialPosition, sf::Vector2f initialVelocity) : shape(radius), velocity(initialVelocity) {
		shape.setFillColor(sf::Color::White);
		shape.setPosition(initialPosition);
	}

    sf::CircleShape& getShape() {
        return shape;
    }

    sf::Vector2f getVelocity() {
		return velocity;
	}

    sf::Vector2f getPosition() {
        return shape.getPosition();
    }

    void setPosition(sf::Vector2f position) {
		shape.setPosition(position);
	}

    void setVelocity(sf::Vector2f newVelocity) {
        velocity = newVelocity;
    }

    float get_mass() {
        return 4 * 3.14159265358979323846 * shape.getRadius() * shape.getRadius() * shape.getRadius() / 3;
    }

    void update_position() {
		shape.move(velocity);
	}

    void update_position(sf::Vector2f correction) {
		shape.move(correction);
	}
};

/*


This is my attempt at the colllsion function, which works, but it is very janky and there are some bugs and without validation.
Below/ collision function being used is a mix of my code and Chat GPT's code, which is much more elegant and works perfectly.
They also add a nice overlapping section which removes the effect of rubberbanding when the balls collide.
Most importantly I really can't be asked to debug my own code and straighte it out, so I'm just going to use the one below.
WILL CHANGE AT A LATER DATE/ UPDATE GIVEN THE TIME

void collision(Ball& ball1, Ball& ball2) {
    tuple<float, float> normal = make_tuple(ball2.shape.getPosition().x - ball1.shape.getPosition().x, ball2.shape.getPosition().y - ball1.shape.getPosition().y);
    float magnitude = sqrt(get<0>(normal) * get<0>(normal) + get<1>(normal) * get<1>(normal));
    tuple<float, float> unitnormal = make_tuple(get<0>(normal) / magnitude, get<1>(normal) / magnitude);
    tuple<float, float> unittangent = make_tuple(-get<1>(unitnormal), get<0>(unitnormal));
    float v1n = get<0>(unitnormal) * ball1.xvelocity + get<1>(unitnormal) * ball1.yvelocity;
    float v1t = get<0>(unittangent) * ball1.xvelocity + get<1>(unittangent) * ball1.yvelocity;
    float v2n = get<0>(unitnormal) * ball2.xvelocity + get<1>(unitnormal) * ball2.yvelocity;
    float v2t = get<0>(unittangent) * ball2.xvelocity + get<1>(unittangent) * ball2.yvelocity;
    float v1nprime = v1n * (ball1.get_mass() - ball2.get_mass()) + 2 * ball2.get_mass() * v2n / (ball1.get_mass() + ball2.get_mass());
    float v2nprime = v2n * (ball2.get_mass() - ball1.get_mass()) + 2 * ball1.get_mass() * v1n / (ball1.get_mass() + ball2.get_mass());
    tuple <float, float> v1nprimevector = make_tuple(get<0>(unitnormal) * v1nprime, get<1>(unitnormal) * v1nprime);
    tuple <float, float> v1tvector = make_tuple(get<0>(unittangent) * v1t, get<1>(unittangent) * v1t);
    tuple <float, float> v2nprimevector = make_tuple(get<0>(unitnormal) * v2nprime, get<1>(unitnormal) * v2nprime);
    tuple <float, float> v2tvector = make_tuple(get<0>(unittangent) * v2t, get<1>(unittangent) * v2t);
    tuple <float, float> v1prime = make_tuple(get<0>(v1nprimevector) + get<0>(v1tvector), get<1>(v1nprimevector) + get<1>(v1tvector));
    tuple <float, float> v2prime = make_tuple(get<0>(v2nprimevector) + get<0>(v2tvector), get<1>(v2nprimevector) + get<1>(v2tvector));
    ball1.set_velocity(get<0>(v1prime), get<1>(v1prime));
    ball2.set_velocity(get<0>(v2prime), get<1>(v2prime));
}

big up: https://youtu.be/eED4bSkYCB8?si=1na57oI5UtTldZTa which helped my 1 month thought experiment.
led me onto: https://www.vobarian.com/collisions/2dcollisions2.pdf - goated research paper

*/

void collision(Ball& ball1, Ball& ball2) {
    // Calculate the normal vector between the balls
    sf::Vector2f normal = ball2.getPosition() - ball1.getPosition();
    float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y);

    // Avoid division by zero by adding a small epsilon
    if (magnitude == 0.0f) {
        return; // If magnitude is zero, skip the collision response to avoid errors.
    }

    // Calculate unit normal and tangent vectors
    sf::Vector2f unitnormal = normal / magnitude;
    sf::Vector2f unittangent(-unitnormal.y, unitnormal.x);

    // Project the velocities onto the normal and tangent vectors
    float v1n = unitnormal.x * ball1.getVelocity().x + unitnormal.y * ball1.getVelocity().y;
    float v1t = unittangent.x * ball1.getVelocity().x + unittangent.y * ball1.getVelocity().y;
    float v2n = unitnormal.x * ball2.getVelocity().x + unitnormal.y * ball2.getVelocity().y;
    float v2t = unittangent.x * ball2.getVelocity().x + unittangent.y * ball2.getVelocity().y;

    // Calculate new normal velocities after collision (tangent velocities remain unchanged)
    float v1nprime = (v1n * (ball1.get_mass() - ball2.get_mass()) + 2.0f * ball2.get_mass() * v2n) / (ball1.get_mass() + ball2.get_mass());
    float v2nprime = (v2n * (ball2.get_mass() - ball1.get_mass()) + 2.0f * ball1.get_mass() * v1n) / (ball1.get_mass() + ball2.get_mass());

    // Convert scalar normal and tangential velocities into vectors
    sf::Vector2f v1nprimevector = unitnormal * v1nprime;
    sf::Vector2f v1tvector = unittangent * v1t;
    sf::Vector2f v2nprimevector = unitnormal * v2nprime;
    sf::Vector2f v2tvector = unittangent * v2t;

    // Update velocities by adding the normal and tangential components
    ball1.setVelocity(v1nprimevector + v1tvector);
    ball2.setVelocity(v2nprimevector + v2tvector);

    float overlap = (ball1.getShape().getRadius() + ball2.getShape().getRadius()) - magnitude;
    if (overlap > 0) {
         sf::Vector2f correction = unitnormal * (overlap / 2.0f);
         ball1.update_position(-correction);
         ball2.update_position(correction);
    }
}

int main() {
    // Not really sure how I feel about the main function, it feels a bit ugly but eh
    int width = 1280;
    int height = 720;
    int numBalls = 100;
    vector<Ball> balls;

    // No clue tf this does
    srand(time(0));

    for (int i = 0; i < numBalls; i++) {
        int radius = (rand() % 5 + 5);
        int xPos = (rand() % (width - 100)) + 50;
        int yPos = (rand() % (height - 100)) + 50;
        sf::Vector2f position(xPos, yPos);
        sf::Vector2f unscaledVelocity(rand() % 20 - 10, rand() % 20 - 10);
        sf::Vector2f velocity = unscaledVelocity / 30.0f; // Scaling down the velocity so it doesn't look like a mess
        Ball particle(radius, position, velocity);
        balls.push_back(particle);
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

            if (ball.getPosition().x < 0 || ball.getPosition().x > width - ball.getShape().getRadius() * 2) {
                sf::Vector2f velocity = ball.getVelocity();
                velocity.x *= -1;
                ball.setVelocity(velocity);
            }
            if (ball.getPosition().y < 0 || ball.getPosition().y > height - ball.getShape().getRadius() * 2) {
                sf::Vector2f velocity = ball.getVelocity();
                velocity.y *= -1;
                ball.setVelocity(velocity);
            }
        }
        //collisions
        // for every other ball in the space, find the magnitude of the distance between the two balls (pythagorean theorem) then compare the value with the sum of the two radii, if it is smaller or equal to, they are colliding.
        // therefore upon collision, the two balls will change direction with an amgle equal to the angle formed between the line going through the centres of the two balls and the original angle of travel.
        for (int i = 0; i < balls.size(); i++) {
            for (int j = i + 1; j < balls.size(); j++) { //kinda scary we will eventually get a range error here
                float magnitudesq = ((balls[i].getPosition().x - balls[j].getPosition().x) * (balls[i].getPosition().x - balls[j].getPosition().x) + (balls[i].getPosition().y - balls[j].getPosition().y) * (balls[i].getPosition().y - balls[j].getPosition().y));
                if (magnitudesq <= (balls[i].getShape().getRadius() + balls[j].getShape().getRadius()) * (balls[i].getShape().getRadius() + balls[j].getShape().getRadius())) {
                   //collision has occured
                    collision(balls[i], balls[j]);
                    cout << "collision" << endl;
                }
            }
        }
        // Clear window
        window.clear(sf::Color::Black);

        // Draw items

        /*
        for (const auto& ball : balls) {
            window.draw(ball.getShape());
        }
        */

        for (int i = 0; i < balls.size(); i++) {
            window.draw(balls[i].getShape());
        }
        // Display everything
        window.display();
    }

    return 0;
}
