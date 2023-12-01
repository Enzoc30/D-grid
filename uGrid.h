//
// Created by enzoc on 30/11/23.
//

#ifndef D_GRID_UGRID_H
#define D_GRID_UGRID_H
#include <iostream>
#include "Bucket.h"
#include <vector>
#include "unordered_map"
#include <algorithm>

using namespace std;

struct SecondaryEntry{
    Bucket* ptr1;
    vector<Bucket*>* ptr2;
    int index;
    int oid;
    SecondaryEntry(){
        ptr1 = nullptr;
        ptr2 = nullptr;
        index = 0;
        oid = 0;
    }
    SecondaryEntry(vector<Bucket*>* gridcell, Bucket* bucket, int id){
        ptr1 = bucket;
        ptr2 = gridcell;
        for (auto i = 0; i < bucket->objectData.size(); i++){
            if(bucket->objectData[i].id == id){
                index = i;
                break;
            }
        }
        oid = id;
    }
};

class uGrid {
private:

    int gridSize;
    int cellSize;
    int bucketSize;

    vector<vector<vector<Bucket*>>> grid;
    unordered_map<int,SecondaryEntry> secondaryIndex;

public:
    int getgridSize() const{return gridSize;}
    int getcellSize() const {return cellSize;}

    auto getGrid(){return grid;}

    uGrid() : gridSize(0), cellSize(0), bucketSize(0) {};
    uGrid(int area, int cell, int bucket)
            : gridSize(area), cellSize(cell), bucketSize(bucket) {
        int numCells = gridSize / cellSize;
        grid.resize(numCells, vector<vector<Bucket*>>(numCells));

    }

    vector<Bucket*> getCell(int x, int y) const {
        int cellX = x / cellSize;
        int cellY = y / cellSize;
        return grid[cellX][cellY];
    }

    void localUpdate(Entry newEntry) {
        int cellX = newEntry.p.getX() / cellSize;
        int cellY = newEntry.p.getY() / cellSize;
        auto newgridcell = grid[cellX][cellY];
        SecondaryEntry oldEntry = *SearchbyIS(newEntry.id);
        auto oldgridcell = *oldEntry.ptr2;
        if (oldgridcell == newgridcell) {
            Bucket* currentBucket = oldEntry.ptr1;
            Point newPoint(newEntry.p.getX(), newEntry.p.getY());
            currentBucket->objectData[oldEntry.index].p = newPoint;
        } else {
            deleteFromCell(oldEntry);
            insertIntoCell(newEntry);
        }
    }

    void deleteFromCell(SecondaryEntry &e) {
        e.ptr1->deleteEntry(e.oid);
        secondaryIndex.erase(e.oid);
    }

    void insertIntoCell(Entry &data) {
        int cellX = data.p.getX() / cellSize;
        int cellY = data.p.getY() / cellSize;
        if(grid[cellX][cellY].empty()){
            Bucket* b = new Bucket(bucketSize);
            grid[cellX][cellY].push_back(b);
        }

        auto ins = grid[cellX][cellY].back()->insert(data.id);
        if(!ins){
            Bucket* b = new Bucket(bucketSize);
            b->insert(data.id);
            grid[cellX][cellY].push_back(b);
        }
        SecondaryEntry minieentrie(&grid[cellX][cellY], grid[cellX][cellY].back(), data.id);
        secondaryIndex[data.id] = minieentrie;
    }

    SecondaryEntry* SearchbyIS(int idx){
        auto it = secondaryIndex.find(idx);
        return (it != secondaryIndex.end()) ? &it->second : nullptr;
    }

    void printIndexSecondary(){
        for(auto &i : secondaryIndex){
            cout << "*******************" << endl;
            cout << " oid : " << i.second.oid << endl;
            cout << " Index : " << i.second.index << endl;
            cout << " size :  "<< i.second.ptr1->entries << endl;
            for(auto &iem : i.second.ptr1->objectData ){
                cout << "ID " << iem.id << endl;
            }
            cout << "*******************" << endl;
        }
    }

    void printGrid() const {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                std::cout << "Cell (" << i * cellSize << ", " << j * cellSize << "): ";
                vector<Bucket*> currentBucket = grid[i][j];
                cout << endl <<"---------------------------- " << endl;
                for(auto &oo : currentBucket) {
                    if (oo != nullptr) {
                        for (auto &entry: oo->objectData) {
                            std::cout << "ID : " << entry.id << " - ";
                        }
                    }
                    std::cout << std::endl;
                }
                cout << "---------------------------- " << endl;
                std::cout << std::endl;
            }
        }
    }
    void cleanup() {
        for (auto& row : grid) {
            for (auto& layer : row) {
                for (auto& bucket : layer) {
                    delete bucket;
                }
            }
        }
    }
};

#endif //D_GRID_UGRID_H