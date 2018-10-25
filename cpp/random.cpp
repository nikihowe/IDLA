// (c) Nikolaus Howe 2018

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <cmath>
#include "util.hpp"
#include "graphConstructors.hpp"
#include "standardTransformations.hpp"
#include "otherTransformations.hpp"
#include "standardIDLA.hpp"
#include "exampleSimulations.hpp"
#include "random.hpp"
#include "visualization.hpp"

void freeNRandom(std::vector<int> &isFull, int n) {
    assert (n < isFull.size()); 

    BitGenerator bg = BitGenerator();

    while (n > 0) {
        int nextBit = positiveModulo(bg.getNum(), isFull.size());
        if (isFull[nextBit]) {
            isFull[nextBit] = 0;
            n--;
        }
    }
}

int hitSqrtRandom(int width, bool torus, bool random) {

    std::vector< std::vector<int> > g = makeGrid(width, torus);

    std::vector<int> lengths;

    int total = 0;
    int nRuns = 100;
    for (int i = 0; i < 100; i++) {
        std::vector<int> isFull(width * width, 1);
        freeNRandom(isFull, pow(width * width, 0.5));
        
        BitGenerator bg = BitGenerator();
        int startNode = 0;

        if (random) {
            startNode = positiveModulo(bg.getNum(), width * width);
        }
        int len = generateAndSaveLength(g, isFull, bg, startNode);
        total += len;
        lengths.push_back(len);
    }

    // Remove outliers (use total / nRuns if you want to keep outliers)
    std::sort(lengths.begin(), lengths.end());

    int btotal = 0;

    for (int i = 5.0/100 * nRuns; i < nRuns - 5.0/100 * nRuns; i++) {
        btotal += lengths[i];
    }
    int bave = btotal / (nRuns - 1.0/10 * nRuns);
    // end of remove outliers

    //std::cout << "ave length " << total / nRuns << std::endl;
    std::cout << "balanced ave length " << bave << std::endl;
    return bave;
}

void runRandomHits(void) {
    int nRuns = 100;
    std::vector<int> grids;
    std::vector<int> tori;
    std::vector<int> rgrids;
    std::vector<int> rtori;
    for (int i = 100; i < 601; i += 100) {
        grids.push_back(hitSqrtRandom(i));
        tori.push_back(hitSqrtRandom(i, true));
        rgrids.push_back(hitSqrtRandom(i, false, true));
        rtori.push_back(hitSqrtRandom(i, true, true));
    }
    std::cout << "for i = 100, ..., 600" << std::endl;
    std::cout << "top-left start grids: " << std::endl;
    printVector(grids);
    std::cout << "top-left start tori: " << std::endl;
    printVector(tori);
    std::cout << "random start grids: " << std::endl;
    printVector(rgrids);
    std::cout << "random start tori: " << std::endl;
    printVector(rtori);
}
