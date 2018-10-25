// (c) Nikolaus Howe 2018
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <cmath>
#include "util.hpp"
#include "graphConstructors.hpp"
#include "exampleSimulations.hpp"
#include "visualization.hpp"
#include "standardTransformations.hpp"
#include "otherTransformations.hpp"
#include "standardIDLA.hpp"
#include "modifiedIDLA.hpp"
#include "optimizedIDLA.hpp"
#include "random.hpp"

int main(void) {

    BitGenerator bg = BitGenerator();
    for (int length = 1000; length < 5001; length += 1000) {
        unsigned long total = 0;
        for (int i = 0; i < 100; i++) {
            std::vector<int> lens = runSequentialPathLengths(
                    length, bg, 0, true);
            total += *std::max_element(lens.begin(), lens.end());
        }
        std::cout << total / 100.0 << std::endl;
    }

    //std::vector<double> bleh = studyRandomPathFillGamblersRuin(5000);
    //printDoubleVector(bleh);
    /*
    for (int nNodes = 1000; nNodes < 6000; nNodes += 1000) {
        std::cout << "NNODES: " << nNodes << std::endl;
        for (double i = 0.9; i > 0.05; i -= 0.1) {
            std::cout << "n^" << i << "(" << (int) pow(nNodes, i) << "): " << studyRandomPathFill(nNodes, i) << std::endl;
        }
    }
    */
   
    /*
    std::vector<int> isFull;
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(0);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(0);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(0);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(1);
    isFull.push_back(1);
    printVector(isFull);
    std::cout << pathClustering(isFull) << std::endl;
    */
}
