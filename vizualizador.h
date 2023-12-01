//
// Created by enzoc on 30/11/23.
//

#ifndef D_GRID_VIZUALIZADOR_H
#define D_GRID_VIZUALIZADOR_H

/*
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

class GameObject {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    GameObject(float x, float y, float width, float height)
            : shape(sf::Vector2f(width, height)) {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Blue);

        // Generar una orientación aleatoria
        srand(static_cast<unsigned>(time(0)));
        float angle = static_cast<float>(rand() % 360);
        velocity.x = std::cos(angle);
        velocity.y = std::sin(angle);
    }

    void update() {
        shape.move(velocity);

        // Manejar rebote en los límites de la ventana
        if (shape.getPosition().x < 0 || shape.getPosition().x + shape.getSize().x > 800) {
            velocity.x = -velocity.x; // Invertir la velocidad en x
        }
        if (shape.getPosition().y < 0 || shape.getPosition().y + shape.getSize().y > 800) {
            velocity.y = -velocity.y; // Invertir la velocidad en y
        }
    }
};

// Define the Grid structure
class Grid {
private:
    int gridSize;
    int cellSize;
    int bucketSize;
    std::vector<std::vector<std::vector<GameObject>>> grid; // 3D array to represent the grid

public:
    // Constructor
    Grid(int area, int cell, int bucket)
            : gridSize(area), cellSize(cell), bucketSize(bucket) {
        // Calculate the number of cells in each dimension
        int numCells = gridSize / cellSize;

        // Initialize the grid with empty buckets
        grid.resize(numCells, std::vector<std::vector<GameObject>>(numCells));
    }

    // Function to insert a game object into a specific cell
    void insertIntoCell(int x, int y, GameObject& gameObject) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;

        // Insert the game object at the end of the cell's list
        grid[cellX][cellY].push_back(gameObject);
    }

    // Function to update the grid and game objects
    void update() {
        for (auto& row : grid) {
            for (auto& cell : row) {
                for (auto& gameObject : cell) {
                    // Update the game objects
                    gameObject.update();
                }
            }
        }
    }

    // Function to draw the grid on the window
    void draw(sf::RenderWindow& window) {
        // Draw grid lines
        for (int i = 0; i <= gridSize; i += cellSize) {
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(i, 0)),
                    sf::Vertex(sf::Vector2f(i, gridSize)),
                    sf::Vertex(sf::Vector2f(0, i)),
                    sf::Vertex(sf::Vector2f(gridSize, i))
            };
            window.draw(line, 4, sf::Lines);
        }

        // Draw game objects
        for (auto& row : grid) {
            for (auto& cell : row) {
                for (auto& gameObject : cell) {
                    window.draw(gameObject.shape);
                }
            }
        }
    }
};

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Grid Example");

    // Create a grid
    Grid myGrid(800, 40, 5); // Assuming a grid area of 800x800, cell size of 40, and bucket size of 5

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Crear un nuevo objeto en la posición del clic con orientación aleatoria
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                GameObject newObject(mouseX, mouseY, 20, 20);
                myGrid.insertIntoCell(mouseX, mouseY, newObject);
            }
        }

        // Clear the window
        window.clear();

        // Update the grid and game objects
        myGrid.update();

        // Draw the grid and game objects
        myGrid.draw(window);

        // Display the window
        window.display();
    }

    return 0;
}

*/

#endif //D_GRID_VIZUALIZADOR_H
