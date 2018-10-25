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
#include "standardIDLA.hpp"
#include "exampleSimulations.hpp"
#include "visualization.hpp"

// Compare the clustering on the path
// of rr and IDLA and random-start IDLA
void observePathClustering(void) {

    int numNodes = 4000;
    int startNode = 0;

    std::vector< std::vector<int> > g = makePath(numNodes); 

    std::cout << "PATH SIZE: " << numNodes << std::endl << std::endl;

    // Fill the path from the left, as would happen
    // with standard IDLA or rotor-router
    {
        std::cout << "Non-random fill clustering" << std::endl;
        std::vector<int> isFull(numNodes, 0);

        int numFree = numNodes;
        int p = 10;
        for (int i = 0; i < numNodes - 1; i++, numFree--) {
            isFull[i] = 1;
            if (numFree == int(floor(pow(numNodes, p * 1.0 / 10)))) {
                std::cout << numFree - 1 << " (n^" << p * 1.0 / 10 << ") free: " << pathClustering(isFull) << std::endl;
                p--;
                if (pathClustering(isFull) >= 1 - 10e-10) {
                    //break;
                }
            }
        }
        std::cout << std::endl;
    }
    
    // Run the IDLA from random location
    {
        std::cout << "IDLA from random location" << std::endl;

        int nRuns = 10;
        std::vector<double> clustering(10, 0);
        for (int i = 0; i < nRuns; i++) {
            std::vector<int> listOfLengths;
            std::vector<int> isFull(numNodes, 0);
            BitGenerator bitGen = BitGenerator();

            int numFree = numNodes;
            int p = 10;
            for (int i = 0; i < numNodes; i++, numFree--) {
                int startLoc = positiveModulo(bitGen.getNum(), numNodes);
                generateAndSaveLength(g, isFull, bitGen, startLoc);
                if (numFree == int(floor(pow(numNodes, p * 1.0 / 10)))) {
                    p--;
                    clustering[p] += pathClustering(isFull);
                    //std::cout << numFree - 1 << " (n^" << p * 1.0 / 10 << ") free: " << pathClustering(isFull) << std::endl;
                }
            }
        }
        for (int i = 0; i < clustering.size(); i++) {
            clustering[i] = clustering[i] * 1.0 / nRuns;
            int numFree = int(floor(pow(numNodes, (10-i)/10.0)));
            std::cout << numFree << " (n^" << (10-i)/10.0 << ") free: " << clustering[i] << std::endl;
        }
    }
}

void observeFill(bool random) {
    int width = 121;
    std::vector< std::vector<int> > graph = makeGrid(width, true);

    int nNodes = graph.size();
    std::vector<int> listOfLengths;
    std::vector< std::vector<int> > isFullList;
    std::vector<int> isFull(nNodes, 0);
    BitGenerator bitGen = BitGenerator();
    int startNode = nNodes/2;
    int minSteps = 0;
    int firstReach = -1;

    if (!random) {
        for (int i = 0; i < nNodes; i++) {
            listOfLengths.push_back(generateAndSaveLength(
                        graph, isFull, bitGen, startNode,
                        minSteps, firstReach));
            isFullList.push_back(isFull); // should copy
        }
    } else {
        for (int i = 0; i < nNodes; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), nNodes);
            listOfLengths.push_back(generateAndSaveLength(
                        graph, isFull, bitGen, startLoc,
                        minSteps, firstReach));
            isFullList.push_back(isFull);
        }
    }
    //so 1/2*n, 1/2*n^(0.9), 1/2*n^(0.8),...,1/2*n^(0.1)
    std::vector<double> clusters;
    for (int i = 10; i > 0; i--) {
        int index = std::floor(0.5 * pow(nNodes, i / 10.0));
        std::cout << index << " ";
        printVectorToDim(isFullList[nNodes - index - 1], width);
        std::cout << std::endl;
        clusters.push_back(
                naiveGridClustering(isFullList[nNodes - index - 1], width));
    }

    std::cout << "printing" << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << clusters[i] << std::endl;
    }
}

// Thomas' clustering idea: divide path
// up into sqrt(n) boxes and return the proportion
// of filled boxes
double oldPathClustering(std::vector<int> &isFull) {
    int numNodes = isFull.size();
    int stepSize = floor(sqrt(numNodes));
    int numBins = numNodes / stepSize;

    std::vector<int> bins(numBins, 0); 
    //std::cout << "initial bins " << std::endl;
    //printVector(bins);

    // Make each bin true if it holds a node
    for (int i = 0; i < numNodes; i++) {
        if (isFull[i]) {
            int binNum = floor(i * 1.0 / stepSize);
            //std::cout << "i " << i << std::endl;
            //std::cout << "bin num " << binNum << std::endl;
            if (binNum >= numBins) {
                bins[numBins - 1] = 1;
            } else {
                bins[binNum] = 1;
            }
        }
    }

    int total = 0;
    for (int i = 0; i < numBins; i++) {
        total += bins[i];
    }

    //std::cout << total << std::endl;

    double totalFraction = total * 1.0 / numBins;

    //std::cout << totalFraction << std::endl;

    return totalFraction;
}

double pathClustering(std::vector<int> &isFull) {
    int numNodes = isFull.size();
    int numBins = numNodes;
    for (int i = 0; i < isFull.size(); i++) {
        numBins -= isFull[i];
    }
    double stepSize = numNodes * 1.0 / numBins;

    std::vector<int> bins(numBins, 0); 
    //std::cout << "initial bins " << std::endl;
    //printVector(bins);

    // Make each bin true if it holds a hole
    for (int i = 0; i < numNodes; i++) {
        //std::cout << "i " << i << std::endl;
        int binNum = floor(i * 1.0 / stepSize);
        //std::cout << "bin num " << binNum << std::endl;
        if (!isFull[i]) {
            if (binNum >= numBins) {
                bins[numBins - 1] = 1;
            } else {
                bins[binNum] = 1;
            }
        }
    }

    int total = 0;
    for (int i = 0; i < numBins; i++) {
        total += bins[i];
    }

    //std::cout << total << std::endl;

    double totalFraction = total * 1.0 / numBins;

    //std::cout << "fraction " <<  totalFraction << std::endl;

    return totalFraction;
}

double naiveGridClustering(std::vector<int> &isFull, int width) {
    unsigned long totalDist = 0;
    unsigned long totalUnfilled = 0;

    std::vector<int> emptyLocs;
    for (int i = 0; i < isFull.size(); i++) {
        if (!isFull[i]) {
            totalUnfilled++;
            emptyLocs.push_back(i);
        }
    }

    //printVectorToDim(isFull, width);
    if (emptyLocs.size() <= 1) { return -1.0; }

    for (int i = 0; i < emptyLocs.size() - 1; i++) {
        int pi = emptyLocs[i];

        int hi = pi / width;
        int wi = pi % width;
        assert (wi >= 0);
        for (int j = i + 1; j < emptyLocs.size(); j++) {
            int pj = emptyLocs[j];
            if (i != j) {
                int hj = pj / width;
                int wj = pj % width;
                totalDist += abs(hj - hi);
                totalDist += abs(wj - wi);
            }
        }
    }
    //std::cout << "total distance " << totalDist << std::endl;
    return totalDist * 1.0 / (totalUnfilled * (totalUnfilled - 1));
}

// Divide square up into sites of size sqrt(n/k)
double gridClustering(std::vector<int> &isFull, int width, int numSites) {
    int numSitesWithParticle = 0;
    int boxWidth = floor(sqrt(isFull.size() / numSites));
    std::vector<int> squares(boxWidth * boxWidth, 0);
    
    // Check each pixel, and on hit, record which square we fall in
    for (int i = 0; i < isFull.size(); i++) {
        if (!isFull[i]) {
            int w = i % boxWidth;
            int h = i / boxWidth;

            squares[h * boxWidth + w] = 1;
        }
    } 
    printVectorToDim(squares, boxWidth);
    int total = 0;
    for (int i = 0; i < squares.size(); i++) {
        total += squares[i];
    }
    std::cout << total * 1.0 / squares.size() << std::endl;
    return total * 1.0 / squares.size();
}
