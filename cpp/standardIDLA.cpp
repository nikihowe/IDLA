// (c) Nikolaus Howe 2018

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include "util.hpp"
#include "graphConstructors.hpp"
#include "standardTransformations.hpp"
#include "standardIDLA.hpp"
#include "exampleSimulations.hpp"
#include "random.hpp"
#include "visualization.hpp"

#define POOL_SIZE 100
#define NRUNS 100

std::vector<int> runSimpleGraphGetLengths(
        std::vector< std::vector<int> > &g) {

    BitGenerator bitGen = BitGenerator();
    return runSequentialLengths(g, bitGen);
}

std::vector< std::vector<int> > runSimpleGraph(
        std::vector< std::vector<int> > &g) {

    BitGenerator bitGen = BitGenerator();
    return runSequential(g, bitGen);
}

int runSimpleGraphGetMax(
        std::vector< std::vector<int> > &g) {

    BitGenerator bitGen = BitGenerator();
    std::vector<int> j = runSequentialLengths(g, bitGen);

    return *std::max_element(j.begin(), j.end());
}

std::vector< std::vector<int> > runSequential(
        std::vector< std::vector<int> > &graph,
        BitGenerator &bitGen, int startNode,
        bool random, int minSteps, int firstReach) {

    int nNodes = graph.size();
    std::vector< std::vector<int> > listOfPaths;
    std::vector<int> isFull(nNodes, 0);

    if (!random) {
        for (int i = 0; i < nNodes; i++) {
            listOfPaths.push_back(generateAndSave(
                        graph, isFull, bitGen, startNode,
                        minSteps, firstReach));
        }
    } else {
        for (int i = 0; i < nNodes; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), nNodes);
            listOfPaths.push_back(generateAndSave(
                        graph, isFull, bitGen, startLoc,
                        minSteps, firstReach));
        }
    }

    return listOfPaths;
}

std::vector<int> runSequentialLengths(
        std::vector< std::vector<int> > &graph,
        BitGenerator &bitGen, int startNode,
        bool random, int minSteps, int firstReach) {

    int nNodes = graph.size();
    std::vector<int> listOfLengths;
    std::vector<int> isFull(nNodes, 0);

    if (!random) {
        for (int i = 0; i < nNodes; i++) {
            listOfLengths.push_back(generateAndSaveLength(
                        graph, isFull, bitGen, startNode,
                        minSteps, firstReach));
        }
    } else {
        for (int i = 0; i < nNodes; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), nNodes);
            listOfLengths.push_back(generateAndSaveLength(
                        graph, isFull, bitGen, startLoc,
                        minSteps, firstReach));
        }
    }

    return listOfLengths;
}

int runSequentialGetMax(
        std::vector< std::vector<int> > &graph,
        BitGenerator &randBits, int startNode,
        bool random, int minSteps,
        int firstReach) {

    std::vector<int> temp = runSequentialLengths(
            graph, randBits, startNode, random,
            minSteps, firstReach);

    return *std::max_element(temp.begin(), temp.end());
}

std::vector<int> generateAndSave(
        std::vector< std::vector<int> > &graph,
        std::vector<int> &isFull,
        BitGenerator &bitGen, int startNode,
        int minSteps, int firstReach) {

    int nNodes = graph.size();
    
    // Record location and path
    int current = startNode;

    std::vector<int> pathTaken;
    pathTaken.push_back(0);
    bool reached = false;
    if (firstReach == -1) {
        reached = true;
    }

    while (minSteps > 0 || !reached || isFull[current]) {

        int numNeighbours = graph[current].size();
        int choice = positiveModulo(bitGen.getNum(), numNeighbours);
        current = graph[current][choice];

        if (current == firstReach) {
            reached = true;
        }
        pathTaken.push_back(current);
        minSteps--;
    }

    // Fill the hole
    isFull[current] = 1;
    return pathTaken;
}

int generateAndSaveLength(
        std::vector< std::vector<int> > &graph,
        std::vector<int> &isFull,
        BitGenerator &bitGen, int startNode,
        int minSteps, int firstReach) {

    int nNodes = graph.size();

    // Record location and path
    int current = startNode;
    int pathLength = 0;
    bool reached = false;
    if (firstReach == -1) {
        reached = true;
    }

    while (pathLength < minSteps || !reached || isFull[current]) {
        
        int numNeighbours = graph[current].size();
        int choice = positiveModulo(bitGen.getNum(), numNeighbours);
        current = graph[current][choice];

        if (current == firstReach) {
            reached = true;
        }
        pathLength++;
    }

    // Fill the hole
    isFull[current] = 1;
    return pathLength;
}

// Run the parallel simulation
std::vector< std::vector<int> > runParallel(
        std::vector< std::vector<int> > &g,
        BitGenerator &bitGen,
        int startNode, bool random) {

    int numNodes = g.size();
    std::vector< std::vector<int> > listOfPaths(numNodes);

    std::vector<int> isFull(numNodes, 0);

    // Store the location of each particle
    // This can be set randomly or not
    std::vector<int> particleLocation(numNodes);
    if (!random) {
        std::fill(particleLocation.begin(), particleLocation.end(), startNode);
    } else {
        for (int i = 0; i < numNodes; i++) {
            particleLocation[i] = positiveModulo(bitGen.getNum(), numNodes);
        }
    }

    // Store a queue of which particle has to move next
    std::queue<int> partQ;
    for (int i = 0; i < numNodes; i++) {
        partQ.push(i); // lower index moves first
    }

    while (!partQ.empty()) {

        // Get the next particle and location, and record it
        int part = partQ.front();
        partQ.pop();

        int loc = particleLocation[part];
        listOfPaths[part].push_back(loc);
        // Settle if you can
        if (!isFull[loc]) {
            isFull[loc] = 1;
        } else {
            int nextLoc = positiveModulo(bitGen.getNum(), g[loc].size());
            particleLocation[part] = g[loc][nextLoc];
            partQ.push(part);
        }
    }
    return listOfPaths;
}

// Run the parallel simulation, but record only the lengths
// of the walks
std::vector<int> runParallelLengths(
        std::vector< std::vector<int> > &g,
        BitGenerator &bitGen,
        int startNode, bool random) {

    int numNodes = g.size();
    std::vector<int> listOfPathLengths(numNodes, 0);
    std::vector<int> isFull(numNodes, 0);

    // Store the location of each particle
    // This can be set randomly or not
    std::vector<int> particleLocation(numNodes);
    if (!random) {
        std::fill(particleLocation.begin(), particleLocation.end(), startNode);
    } else {
        for (int i = 0; i < numNodes; i++) {
            particleLocation[i] = positiveModulo(bitGen.getNum(), numNodes);
        }
    }

    // Store a queue of which particle has to move next
    std::queue<int> partQ;
    for (int i = 0; i < numNodes; i++) {
        partQ.push(i); // lower index moves first
    }

    while (!partQ.empty()) {

        // Get the next particle and location, and record it
        int part = partQ.front();
        partQ.pop();

        int loc = particleLocation[part];
        // Settle if you can
        if (!isFull[loc]) {
            isFull[loc] = 1;
        } else {
            int nextLoc = positiveModulo(bitGen.getNum(), g[loc].size());
            particleLocation[part] = g[loc][nextLoc];

            listOfPathLengths[part]++;
            partQ.push(part);
        }
    }
    return listOfPathLengths;
}
