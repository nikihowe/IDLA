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
#include "visualization.hpp"
#include "modifiedIDLA.hpp"
#include "random.hpp"

double studyRandomPathFillViaSimulation(int numNodes, double power) {

    int nRuns = 100;

    std::vector< std::vector<int> > g = makePath(numNodes);
    std::vector<int> isFull(numNodes, 1);
    double numberToFree = floor(pow(numNodes, power));
    freeNRandom(isFull, floor(pow(numNodes, power)));
    BitGenerator bg = BitGenerator();

    // See how long it takes each node to settle
    std::vector<double> averageSettlingTimes;
    for (int startNode = 0; startNode < numNodes; startNode++) {
        // Average over nRuns runs for each node
        int total = 0;
        std::vector<int> isFullBackup = isFull;
        for (int j = 0; j < nRuns; j++) {
            //printVector(isFull);
            total += generateAndSaveLength(g, isFull, bg, startNode);
            isFull = isFullBackup;
        }
        averageSettlingTimes.push_back(total * 1.0 / nRuns);
    }

    double overallAverage = 0;
    for (int i = 0; i < averageSettlingTimes.size(); i++) {
        overallAverage += averageSettlingTimes[i];
    }
    overallAverage /= (1.0 * (averageSettlingTimes.size()));

    return overallAverage;
}

double studyRandomPathFillViaExpectationPower(int numNodes, double power) {

    std::vector< std::vector<int> > g = makePath(numNodes);
    std::vector<int> isFull(numNodes, 1);
    double numberToFree = floor(pow(numNodes, power));
    freeNRandom(isFull, floor(pow(numNodes, power)));
    BitGenerator bg = BitGenerator();
    
    // Expected-value version
    unsigned long runLength = 0;
    unsigned long squaredTotal = 0;
    for (int i = 0; i < isFull.size(); i++) {
        if (isFull[i] == 0 || i == isFull.size() - 1) {
            squaredTotal += runLength == 1 ? 1 :
                1.0/6 * (runLength-1) * runLength * (runLength+1);
            runLength = 0;
        } else {
            runLength++;
        }
    }
    double estimatedAverage = squaredTotal * 1.0 / numNodes;
    return estimatedAverage;
}

std::vector<double> studyRandomPathFillGamblersRuin(int numNodes) {

    std::vector< std::vector<int> > g = makePath(numNodes);
    std::vector<int> isFull(numNodes, 0);
    BitGenerator bg = BitGenerator();

    std::vector<double> gRuins;
    for (int i = 0; i < numNodes; i++) {
        int startNode = positiveModulo(bg.getNum(), numNodes);
        generateAndSaveLength(g, isFull, bg, startNode);
        gRuins.push_back(getGamblersRuin(isFull));
    }
    return gRuins;
}

double getGamblersRuin(std::vector<int> &isFull) {
    
    // Expected-value version
    unsigned long runLength = 0;
    unsigned long squaredTotal = 0;
    for (int i = 0; i < isFull.size(); i++) {
        if (isFull[i] == 0 || i == isFull.size() - 1) {
            squaredTotal += runLength == 1 ? 1 :
                1.0/6 * (runLength-1) * runLength * (runLength+1);
            runLength = 0;
        } else {
            runLength++;
        }
    }
    return squaredTotal * 1.0 / isFull.size();
}

// Run IDLA with momentum
std::vector< std::vector<int> > runMomentumSequential(
        std::vector< std::vector<int> > &g, double momentum) {

    int numNodes = g.size();
    std::vector<int> isFull(numNodes, 0);
    BitGenerator bg = BitGenerator();
    std::vector< std::vector<int> > pathList;
    int lastDirection = -1;

    for (int i = 0; i < numNodes; i++) {
        std::vector<int> path = singleParticleMomentum(g, isFull,
                bg, lastDirection, momentum);
        pathList.push_back(path);
    }
    return pathList;
}

// Run one particle's random walk in IDLA on graph
// Returns the vector of nodes in the walk
std::vector<int> singleParticleMomentum(
        std::vector< std::vector<int> > &g,
        std::vector<int> &isFull,
        BitGenerator &bg,
        int lastDirection,
        double momentum) {

    int numNodes = g.size();
    
    // Record location and path
    int current = 0;

    std::vector<int> pathTaken;
    pathTaken.push_back(current);
    while (isFull[current]) {

        int choice = momentumChoice(g, isFull,
                bg, current, lastDirection, momentum);
        lastDirection = choice;

        current = g[current][choice];

        pathTaken.push_back(current);
    }

    // Fill the hole
    isFull[current] = 1;
    return pathTaken;
}

// Run IDLA with one-step lookahead
std::vector< std::vector<int> > runOneStepLookaheadSequential(
        std::vector< std::vector<int> > &g) {

    int numNodes = g.size();
    std::vector<int> isFull(numNodes, 0);
    BitGenerator bg = BitGenerator();
    std::vector< std::vector<int> > pathList;

    for (int i = 0; i < numNodes; i++) {
        std::vector<int> path = singleParticleLookahead(g, isFull, bg);
        pathList.push_back(path);
    }
    return pathList;
}

// Run IDLA with one-step lookahead lengths
std::vector<int> runOneStepLookaheadSequentialLength(
        std::vector< std::vector<int> > &g) {

    int numNodes = g.size();
    std::vector<int> isFull(numNodes, 0);
    BitGenerator bg = BitGenerator();
    std::vector<int> pathList;

    for (int i = 0; i < numNodes; i++) {
        std::vector<int> path = singleParticleLookahead(g, isFull, bg);
        pathList.push_back(path.size());
    }
    return pathList;
}

// Run one particle's random walk in IDLA on graph
// Returns the vector of nodes in the walk
std::vector<int> singleParticleLookahead(
        std::vector< std::vector<int> > &g,
        std::vector<int> &isFull,
        BitGenerator &bitGen) {

    int numNodes = g.size();
    
    // Record location and path
    int current = 0;

    std::vector<int> pathTaken;
    pathTaken.push_back(current);
    while (isFull[current]) {
        //printVector(isFull);

        int choice = chooseBestNeighbour(g,
                isFull, bitGen, current);
        current = g[current][choice];

        //std::cout << current << std::endl << std::flush;
        pathTaken.push_back(current);
    }

    // Fill the hole
    isFull[current] = 1;
    return pathTaken;
}

// Run g.size() particles sequentially on the graph,
// using randomly-initialized rotor-router,
// and return the lengths of the runs
std::vector<int> runSequentialRotorRouterLengths(
        std::vector< std::vector<int> > &g,
        int startNode, bool randStart) {

    int nNodes = g.size();
    std::vector<int> listOfLengths;
    std::vector<int> isFull(nNodes, 0);
    std::vector<int> nextMove(nNodes, 0);

    // Randomize the starting direction of each node
    BitGenerator bg = BitGenerator();
    for (int i = 0; i < nNodes; i++) {
        int numNeighbours = g[i].size();
        nextMove[i] = positiveModulo(bg.getNum(), numNeighbours);
    }

    if (randStart) {
        for (int i = 0; i < nNodes; i++) {
            startNode = positiveModulo(bg.getNum(), g.size());
            listOfLengths.push_back(followPath(g, isFull, nextMove, startNode));
        }
    } else {
        for (int i = 0; i < nNodes; i++) {
            listOfLengths.push_back(followPath(g, isFull, nextMove, startNode));
        }
    }

    return listOfLengths;
}

// Run a single particle on an initialized rotor-router graph
// Returns length of the run
int followPath(std::vector< std::vector<int> > &g,
        std::vector<int> &isFull, std::vector<int> &nextMove,
        int startNode) {

    int nNodes = g.size();

    // Record location and path
    int current = startNode;
    int pathLength = 0;

    while (isFull[current]) {
        
        int numNeighbours = g[current].size();
        int choice = nextMove[current];

        nextMove[current] = positiveModulo(choice + 1, numNeighbours); // advance by 1
        current = g[current][choice];
        pathLength++;
    }

    // Fill the hole
    isFull[current] = 1;
    return pathLength;
}

std::vector<int> generateFastLocations(int width) {

    std::vector< std::vector<int> > g = makeGrid(width);

    std::vector<int> taken(width, 0);
    std::vector<int> locations;

    int place = 0;
    for (int i = 0; i < width; i++) {
        while (true) {
            int add = width / 2 % 2 == 0 ? 1 : 2;
            place = positiveModulo(place + width / 2 + add, width);
            if (!taken[place]) {
                taken[place] = 1;
                locations.push_back(i * width + place);
                break;
            }
        }
    }
    return locations;
}

std::vector<int> fillFromPoints(std::vector< std::vector<int> > &g,
        std::vector<int> &startPoints, BitGenerator &bitGen) {
    
    int nNodes = g.size();
    std::vector<int> listOfLengths;
    std::vector<int> isFull(nNodes, 0);

    for (int i = 0; i < nNodes; i++) {
        int startLocIndex = positiveModulo(
                bitGen.getNum(), startPoints.size());
        startLocIndex = positiveModulo(i, startPoints.size());
        int startLoc = startPoints[startLocIndex];
        listOfLengths.push_back(generateAndSaveLength(
                    g, isFull, bitGen, startLoc));
    }

    return listOfLengths;
}
