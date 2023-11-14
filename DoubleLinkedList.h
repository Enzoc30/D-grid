//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_DOUBLELINKEDLIST_H
#define D_GRID_DOUBLELINKEDLIST_H


#include "Bucket.h"

class DoubleLinkedList{

public:
    Bucket* head;
    Bucket* tail;
    DoubleLinkedList(): head(nullptr), tail(nullptr) { };
    void pushback(Bucket* newBucket){
        if(tail == nullptr){
            head = tail = newBucket;
        }
        else{
            tail->nextBucket = newBucket;
            newBucket->prevBucket = tail;
            tail = newBucket;
        }
    }

    void popback(){
        if(tail != nullptr){
            Bucket* prevTail = tail->prevBucket;
            delete tail;
            tail = prevTail;
            if(tail != nullptr){
                tail->nextBucket = nullptr;
            }else{
                head = nullptr;
            }
        }
    }

    void printDLL() const {
        Bucket* cu = head;
        while(cu != nullptr){
            cu->printBucket();
            cu = cu->nextBucket;
        }
    }


};


#endif //D_GRID_DOUBLELINKEDLIST_H
