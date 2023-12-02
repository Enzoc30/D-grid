#include <iostream>
#include <random>
#include "uGrid.h"
#include <fstream>
#include <sstream>
#include "timePartition.h"

#include "prettyprint.hpp"

//
void solve(){

//    ifstream file("/home/enzoc/CLionProjects/D-grid/Data.csv");
//    // Ignorar la primera línea si contiene encabezados
//    std::string line;
//    std::getline(file, line);
//    uGrid Grid(15500,ps,pi,800,20);
//    long long cont = 0;
//    while (std::getline(file, line)) {
//        std::istringstream iss(line);
//        std::string token;
//        std::vector<std::string> fields;
//
//        // Leer los campos separados por comas
//        while (std::getline(iss, token, ',')) {
//            fields.push_back(token);
//        }
//
//        // Extraer campos específicos y construir una Entry
//        int taxi_id = std::stoi(fields[0]);
//        double time = std::stod(fields[1]);
//        int latitude = std::stoi(fields[2]);
//        int longitude = std::stoi(fields[3]);
//        int speed_x = std::stoi(fields[5]);
//        int speed_y = std::stoi(fields[6]);
//        cout << taxi_id << " " << latitude << " " << time << endl;
//        Entry pp (taxi_id, time, latitude, longitude, speed_x, speed_y);
//        Grid.insertIntoCell(pp);
//        if(cont == 15635555){break;}
//        cont++;
//    }

    //Grid.printGrid();

}

std::vector<double> numerosAleatorios;


// void insertRandomPoint(timePartition& grid, int i) {
//     // Generador de números aleatorios
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     // Rangos para las coordenadas, velocidad, y tiempo
//     std::uniform_int_distribution<int> disCoord3(30, 200);
//     std::uniform_int_distribution<int> disCoord(50, 500);
//     std::uniform_real_distribution<double> disTime(0.0, 1000.0);
//     std::uniform_int_distribution<int> disSpeed(-50, 500);
//     std::uniform_int_distribution<int> disCoord4(30, 100);

//     // Generar valores aleatorios
//     int id = disCoord(gen);  // Usar coordenada aleatoria como ID
//     int lat = disCoord4(gen);
//     int lon = disCoord3(gen);
//     int speedX = disSpeed(gen);
//     int speedY = disSpeed(gen);
// //    cout << time << endl;
//     // Crear un nuevo Entry con valores aleatorios
//     //cout << numerosAleatorios[i] << endl;
//     Entry randomEntry{id, {lat, lon}, {speedX, speedY}, false};

//     // Insertar el nuevo Entry en el timePartition
//     grid.update(randomEntry, 0);
// }

// void solve2(){
//     timePartition myGrid (500.5,ps,pi,100,5);
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     // Crear un conjunto de números aleatorios (doubles)
//     std::uniform_real_distribution<double> dis(0.0, 1000.0);

//     for (int i = 0; i < 554; ++i) {
//         numerosAleatorios.push_back(dis(gen));
//     }
//     std::sort(numerosAleatorios.begin(), numerosAleatorios.end());

//     for (int i = 0; i < 554; ++i) {
//         insertRandomPoint(myGrid,i);

// //        myGrid.print();
//     }

//     std::cout << "Cuadrícula antes de la actualización:" << std::endl;
//     //myGrid.print();

//     insertRandomPoint(myGrid,524);  // Simula una inserción aleatoria
//     myGrid.print();

// }

void disperseData(std::vector<Entry>& data, double latShift, double lonShift) {
    for (auto& entry : data) {
        entry.p.setx(entry.p.getX() + latShift);
        entry.p.setx(entry.p.getX() + lonShift);;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Missing file argument");
        return -1;
    }

    ifstream file(argv[1]);
    // Ignorar la primera línea si contiene encabezados
    std::string line;
    std::getline(file, line);

    Point ps(113.2, 21.5);
    Point pi(115.2, 23.5);
    uGrid Grid(0.1, 20, ps, pi);

    long long to_read = 100000;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> fields;

        // Leer los campos separados por comas
        while (std::getline(iss, token, ',')) {
            fields.push_back(token);
        }

        // Extraer campos específicos y construir una Entry
        std::cerr << fields << "\n";

        int taxi_id = std::stoi(fields[0]);
        double time = std::stod(fields[1]);
        double latitude = std::stod(fields[2]);
        double longitude = std::stod(fields[3]);
        double speed_x = std::stod(fields[5]);
        double speed_y = std::stod(fields[6]);
        //cout << taxi_id << " " << latitude << " " << time << endl;

        Entry pp{taxi_id, {latitude, longitude}, {speed_x, speed_y}};

        Grid.insert_update(pp);

        to_read--;
        if(to_read == 0) {
            break;
        }
    }

    auto c = Grid.rangeQuery({114.0, 114.5}, {22.5, 22.7});
    std::cerr << c << "\n";

    // Grid.printGrid();

}