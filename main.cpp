#include <iostream>
#include <random>
#include "uGrid.h"

void insertRandomPoint(uGrid& grid) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> disCoord(0, grid.getgridSize() - 1);

    int randomX = disCoord(gen);
    int randomY = disCoord(gen);

    std::uniform_int_distribution<int> disData(15, 10000);
    int randomData = disData(gen);

    grid.insertIntoCell(randomX, randomY, randomData);
}


int main() {

    uGrid myGrid(100, 10, 8);

    for (int i = 0; i < 5555; i++) {
        insertRandomPoint(myGrid);
    }

    std::cout << "Cuadrícula despues de la actualización:" << std::endl;
    myGrid.printGrid();

<<<<<<< HEAD
=======
    // Ignorar la primera línea si contiene encabezados
    std::string line;
    std::getline(file, line);
    uGrid Grid(15500,800,20);
    long long cont = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> fields;

        // Leer los campos separados por comas
        while (std::getline(iss, token, ',')) {
            fields.push_back(token);
        }

        // Extraer campos específicos y construir una Entry
        int taxi_id = std::stoi(fields[0]);
        double time = std::stod(fields[1]);
        int latitude = std::stoi(fields[2]);
        int longitude = std::stoi(fields[3]);
        int speed_x = std::stoi(fields[5]);
        int speed_y = std::stoi(fields[6]);
        cout << taxi_id << " " << latitude << " " << time << endl;
        Entry pp (taxi_id, time, latitude, longitude, speed_x, speed_y);
        Grid.insertIntoCell(pp);
        if(cont == 15635555){break;}
        cont++;
    }

    //Grid.printGrid();
>>>>>>> 216e0d8 (tempo)

    return 0;
}
