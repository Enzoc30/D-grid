//
// Created by enzoc on 13/11/23.
//

#ifndef D_GRID_GRID_H
#define D_GRID_GRID_H


#include <iostream>
#include "Point.h"
#include <vector>
#include <algorithm>
#include "params.h"
#include "Bucket.h"

class Grid {
private:

    int gridSize;
    int cellSize;
    int bucketSize;

    std::vector<std::vector<Bucket*>> grid;

public:
    int getgridSize() const{return gridSize;}
    int getcellSize() const {return cellSize;}

    std::vector<std::vector<Bucket*>> getGrid(){return grid;}

    Grid(int area, int cell, int bucket)
            : gridSize(area), cellSize(cell), bucketSize(bucket) {
        int numCells = gridSize / cellSize;

        grid.resize(numCells, std::vector<Bucket*>(numCells, nullptr));
    }

    Bucket* getCell(int x, int y) const {
        int cellX = x / cellSize;
        int cellY = y / cellSize;
        return grid[cellX][cellY];
    }

    void localUpdate(int oldX, int oldY, int newX, int newY, int data) {
        if (oldX == newX && oldY == newY) {
            Bucket* currentBucket = getCell(oldX,oldY);
            for(auto &entry : currentBucket->obejectData){
                if(entry.id == data){
                    Point newPoint(newX, newY);
                    entry.p = newPoint;
                    break;
                }
            }
        } else {
            deleteFromCell(oldX, oldY, data);
            insertIntoCell(newX, newY, data);
        }
    }

    void deleteFromCell(int x, int y, int data) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;

        Bucket* entries = grid[cellX][cellY];

        entries->obejectData.erase(
                remove_if(entries->obejectData.begin(), entries->obejectData.end(),
                          [data](const Entry& entry) {return entry.id == data ;}),
                          entries->obejectData.end()
                );
    }

    void insertIntoCell(int x, int y, int data) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;

        if (grid[cellX][cellY] == nullptr) {
            grid[cellX][cellY] = new Bucket(data);
        } else {
            Entry newEntry(data);
            grid[cellX][cellY]->obejectData.push_back(newEntry);
        }

    }

    void printGrid() const {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                std::cout << "Cell (" << i * cellSize << ", " << j * cellSize << "): ";

                Bucket* currentBucket = grid[i][j];
                if(currentBucket != nullptr) {
                    for (auto &entry: currentBucket->obejectData) {
                        std::cout << "ID : " << entry.id << " - ";
                    }
                }
                std::cout << std::endl;

            }
        }
    }


};


#endif //D_GRID_GRID_H
