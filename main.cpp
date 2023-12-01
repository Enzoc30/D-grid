#include <iostream>
#include <random>
#include "Grid.h"

void insertRandomPoint(Grid& grid) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> disCoord(0, grid.getgridSize() - 1);

    int randomX = disCoord(gen);
    int randomY = disCoord(gen);

    std::uniform_int_distribution<int> disData(1, 100);
    int randomData = disData(gen);

    grid.insertIntoCell(randomX, randomY, randomData);
}


int main() {

    Grid myGrid(100, 10, 5);

    for (int i = 0; i < 5; ++i) {
        insertRandomPoint(myGrid);
    }

    std::cout << "Cuadrícula antes de la actualización:" << std::endl;
    myGrid.printGrid();

    insertRandomPoint(myGrid);  // Simula una inserción aleatoria
    myGrid.printGrid();

    myGrid.cleanup();

    std::cout << "------------------------" << std::endl;
    return 0;
}
