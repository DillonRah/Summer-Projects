#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> 
#include <cstdlib>
#include <random>
#include <ctime>  
#include<windows.h>     
using namespace std;

class Item {
public:
    sf::RectangleShape shape;
    float height;
    float position;
};


void swapPositions(Item& item1, Item& item2) {
    float a = item1.position;
    float b = item2.position;
    item1.shape.setPosition(sf::Vector2f(b, 720 - item1.height));
    item1.position = b;
    item2.shape.setPosition(sf::Vector2f(a, 720 - item2.height));
    item2.position = a;
}


void swapItems(Item& item1, Item& item2) {
    Item temp = item1;
    item1 = item2;
    item2 = temp;
    swapPositions(item1, item2);
}

void shuffleBlocks(vector<Item>& blocks) {
    // Seed the random number generator with current time
    srand(static_cast<unsigned int>(std::time(nullptr)));

    // Get the size of the vector
    int n = blocks.size();

    // Perform Fisher-Yates shuffle
    for (int i = n - 1; i > 0; --i) {
        // Generate a random index in the range [0, i]
        int j = std::rand() % (i + 1);

        // swapPositions the current element with the randomly selected element
        if (i != j) {
            swapItems(blocks[i], blocks[j]);
        }
    }
}

void bubbleSort(vector<Item>& list, int &startingposition) {
    int n = list.size();
    for (; startingposition < n - 1; startingposition++) {
        if (list[startingposition].height > list[startingposition + 1].height) {
			swapItems(list[startingposition], list[startingposition + 1]);
            return;
		}
	}
    startingposition = 0;
    return;
}

void shuttleSort(vector<Item>& list) {
    int n = list.size();

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (list[j].height > list[j + 1].height) {
                // Swap elements if they are in the wrong order
                swapItems(list[j], list[j + 1]);
                swapped = true;
                break;  // Return after making one swap
            }
        }

        if (swapped) {
            return;  // Return if a swap was made
        }
    }
}



int main() {
    int width = 1280;
    int height = 720;
    int number = 500;
    float temp_width = static_cast<float>(width) / number; // Use static_cast for float conversion
    float temp_height = static_cast<float>(height) / number;
    int startingpos = 0;
    vector<Item> blocks;

    for (int i = 0; i < number; i++) {
        Item item;
        sf::RectangleShape rect;
        rect.setPosition(sf::Vector2f(startingpos, height - temp_height));
        rect.setSize(sf::Vector2f(temp_width, temp_height));
        rect.setFillColor(sf::Color::White);
        item.height = temp_height;
        item.position = startingpos;
        item.shape = rect;
        blocks.push_back(item);
        temp_height += static_cast<float>(height) / number;
        startingpos += static_cast<int>(temp_width);
    }
    shuffleBlocks(blocks);
    //swapPositions(blocks[0], blocks[40]);
    //swapPositions(blocks[0], blocks[40]);
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

        /*
        for (int i = 0; i < blocks.size() - 1; i++) {
            if (blocks[i].height > blocks[i + 1].height) {
                swapPositions(blocks[i], blocks[i + 1]);
            }
        }
        */
        
        int starting = 0;
        //shuttleSort(blocks);
        bubbleSort(blocks, starting);
 
        // Clear window
        window.clear(sf::Color::Black);

        // Draw items
        for (const auto& block : blocks) {
            window.draw(block.shape);
        }

        // Display everything
        window.display();
    }

    return 0;
}
