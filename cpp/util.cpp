// (c) Nikolaus Howe 2018

#include <iostream>
#include <cstdlib>
#include <vector>
#include <limits.h>
#include <random>
#include "util.hpp"

void printVector(std::vector<int> path) {
    for (std::vector<int>::const_iterator i = path.begin();
            i != path.end(); ++i) {
        std::cout << *i << ' ';
    }
    std::cout << std::endl;
}

void printDoubleVector(std::vector<double> path) {
    for (int i = 0; i < path.size(); i++) {
        std::cout << path[i] << ' ';
    }
    std::cout << std::endl;
}

void printVectorToDim(std::vector<int> path, int d) {
    for (int i = 0; i < path.size(); i++) {
        if (i % d == 0) std::cout << std::endl;    
        std::cout << path[i] << ' ';
    }
    std::cout << std::endl;
}

void print2dVector(std::vector< std::vector<int> > path) {
    std::vector< std::vector<int> >::iterator row;
    std::vector<int>::iterator col;
    for (row = path.begin(); row != path.end(); row++) {
        for (col = row->begin(); col != row->end(); col++) {
            std::cout << *col << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printArray(int *path, int length) {
    std::cout << "[ ";
    for (int i = 0; i < length; i++) {
        std::cout << path[i] << " ";
    }
    std::cout << "]" << std::endl;
}

BitGenerator::BitGenerator() {
    std::random_device device;
    numReads = 0;
    int dev = device();
    e.seed(dev);
    randNum = e();
}

uint32_t BitGenerator::getNum() {
    return e();
}

uint32_t BitGenerator::max() {
    return e.max();
}

double BitGenerator::getDouble() {
    return e() * 1.0 / e.max();
}

unsigned int BitGenerator::getBit() {
    if (numReads >= sizeof(uint32_t)*CHAR_BIT)  {
        randNum = getNum();
        numReads = 0;
    }
    // Get the rightmost bit
    int result = randNum & 1;
    numReads++;
    randNum >>= 1;
    return result;
}

int chooseBestNeighbour(std::vector< std::vector<int> > &g,
        std::vector<int> &isFull, BitGenerator &bg,
        int node) {
    std::vector<int> available;
    for (int i = 0; i < g[node].size(); i++) {
        if (!isFull[g[node][i]]) {
            available.push_back(i);
        }
    }
    if (available.empty()) {
        return positiveModulo(bg.getNum(), g[node].size());
    } else {
        int choice = positiveModulo(bg.getNum(), available.size());
        return available[choice];
    }
}

// We have a momentum between 0 and 1.
// Continue forwards with probability momentum,
// else choose randomly
// We might change it to be able to 
// ride edges as well
int momentumChoice(
        std::vector< std::vector<int> > &g,
        std::vector<int> &isFull,
        BitGenerator &bitGen,
        int current,
        int lastDirection,
        double momentum) {

    int nextDirection;

    // Decide if we're continuing in the same direction,
    // or if we're choosing randomly
    bool sameDirection = lastDirection >= 0 && bitGen.getDouble() < momentum;

    // If we can continue in the same direction
    if (sameDirection && g[current].size() == 4) {
        nextDirection = lastDirection;
    } else {
        nextDirection = positiveModulo(bitGen.getNum(), g[current].size());
    }

    return nextDirection;
}

void copyPaste(int j, std::vector<int> &appendFrom, std::vector<int> &appendTo) {
    if (appendTo != appendFrom) {
        appendTo.insert(appendTo.end(),
                std::make_move_iterator(appendFrom.begin() + j),
                std::make_move_iterator(appendFrom.end()));

        appendFrom.erase(appendFrom.begin() + j, appendFrom.end());
    }
}
