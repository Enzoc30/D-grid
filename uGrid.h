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

    double gridSize;
    double cellSize;
    int bucketSize;
    Point maxiVel;
    Point minVel;
    vector<vector<vector<Bucket*>>> grid;
    unordered_map<int,SecondaryEntry> secondaryIndex;

public:
    double getgridSize() const{return gridSize;}
    double getcellSize() const {return cellSize;}

    auto getGrid(){return grid;}

    uGrid() : gridSize(0), cellSize(0), bucketSize(0) {};
    uGrid(Point ll, Point llis, double cell, int bucketSizes)
            : gridSize(), cellSize(cell), bucketSize(bucketSizes) ,minVel(1e9+1.0,1e9+1.0), maxiVel(-1e9+1.0,-1e9+1.0){
        double limxx = abs(ll.getX() - llis.getX()) ;
        double limyy = abs(ll.getY() - llis.getY()) ;
        int numCells = ceil(max(limxx,limyy)/cell);
        gridSize = numCells * cell;

        grid.resize(numCells, vector<vector<Bucket*>>(numCells));

    }

    vector<Bucket*> getCell(double x, double y) const {
        int cellX = floor(x/cellSize);
        int cellY = floor(y/cellSize);
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

    double maxVelocity(){
        double maxi = -1e9;
        for(auto &i : grid ){
            for(auto &j : i){
                for(auto &k : j){
                    double z = k->getMaxVelocity();
                    if(maxi < z){
                        maxi = z;
                    }
                }
            }
        }
        return maxi;
    }


    void deleteFromCell(SecondaryEntry &e) {
        e.ptr1->deleteEntry(e.oid);
        secondaryIndex.erase(e.oid);
//        maxVelocity();
    }

    Point getMaxVel() const{
        return maxiVel;
    }
    Point getMinVel() const{
        return minVel;
    }

    void insertOrUpdate(Entry & e){
        if (SearchbyIS(e.id) == nullptr)
            insertIntoCell(e);
        else
            localUpdate(e);
    }

    void insertIntoCell(Entry &data) {
        int cellX = data.p.getX() / cellSize;
        int cellY = data.p.getY() / cellSize;

        if(grid[cellX][cellY].empty()){
            Bucket* b = new Bucket(bucketSize);
            grid[cellX][cellY].push_back(b);
        }

        auto ins = grid[cellX][cellY].back()->insert(data);
        if(!ins){
            Bucket* b = new Bucket(bucketSize);
            b->insert(data);
            grid[cellX][cellY].push_back(b);
        }
        SecondaryEntry minieentrie(&grid[cellX][cellY], grid[cellX][cellY].back(), data.id);
        secondaryIndex[data.id] = minieentrie;
    }

    SecondaryEntry* SearchbyIS(int idx){
        auto it = secondaryIndex.find(idx);
        return (it != secondaryIndex.end()) ? &it->second : nullptr;
    }

    //return elements of grids

    //

    pair<Point,Point> enlargeS(Point q1, Point q2, double tq){
        //Definimos los bordes de S
        double minX = min(q1.getX(),q2.getX());
        double minY = min(q1.getY(),q2.getY());
        double maxX = max(q1.getX(),q2.getX());
        double maxY = max(q1.getY(),q2.getY());

        //Definimos las bordes de S'
        //to-do -> ARREGLAR POR VELOCIDADES INDIVIDUALES POR LADO (NSEW)
        minX += tq * maxVelocity();
        minY += tq * maxVelocity();
        maxX += tq * maxVelocity();
        maxY += tq * maxVelocity();
        return {Point(minX,minY),Point(maxX,maxY)};
    }

    vector<Entry> rangeQuery(Point minC, Point maxC){
        vector<Entry> result;
        double minX = minC.getX();
        double minY = minC.getY();
        double maxX = maxC.getX();
        double maxY = maxC.getY();

        //Definimos ST -> minimum rectangle with grid cell boundaries containing S
        for (auto x=minX; x<=maxX; x+=cellSize){ //iterando en ST
            for (auto y=minY; y<=maxY; y+=cellSize){
                auto cell = getCell(x,y); //están en S' ahora
                for (auto bucket : cell){
                    for (auto entry : bucket->objectData) {
                        result.push_back(entry);
                    }
                }
            }
        }
        return result;
    }

    bool entryInWindow(const Entry& candidate, double tq, const Point& minC, const Point& maxC) {
        double x = candidate.p.getX() + candidate.v.getX()*tq;
        double y = candidate.p.getY() + candidate.v.getY()*tq;
        auto predicted = Point(x,y);

        return (x >= minC.getX() && x <= maxC.getX()
                 && y >= minC.getY() && y <= maxC.getY());
    }

    vector<Entry> predictiveRangeQuery(Point q1, Point q2, double tq){
        pair<Point,Point> Sp = enlargeS(q1,q2,tq); //(minC, maxC)
        vector<Entry> candidates = rangeQuery(Sp.first, Sp.second);
        
        vector<Entry> answers;
        for (auto candidate : candidates){
            if (entryInWindow(candidate,tq,Sp.first,Sp.second)){
                answers.push_back(candidate);
            }
        }
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
            for (auto& col : row) {
                for (auto& bucket : col) {
                    if (bucket != nullptr) {
                        delete bucket;
                    }
                }
                col.clear();
            }
        }
        secondaryIndex.clear();
    }
};

#endif //D_GRID_UGRID_H