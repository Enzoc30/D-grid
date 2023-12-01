#include <iostream>
#include <random>
#include <fstream>
#include "sstream"
#include "uGrid.h"

int main() {
    std::ifstream file("/home/enzoc/CLionProjects/D-grid/Data.csv");

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }

    std::vector<Entry> entries;

    // Ignorar la primera línea si contiene encabezados
    std::string line;
    std::getline(file, line);
    uGrid Grid(1000000,100000,20);
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
        std::string time = fields[1];
        int latitude = std::stoi(fields[2]);
        int longitude = std::stoi(fields[3]);
        int speed_x = std::stoi(fields[5]);
        int speed_y = std::stoi(fields[6]);

        Entry pp (taxi_id, time, latitude, longitude, speed_x, speed_y);
        Grid.insertIntoCell(pp);
    }

    Grid.printGrid();

    return 0;
}