// (c) Nikolaus Howe 2018

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include "util.hpp"
#include "optimizedIDLA.hpp"

#define POOL_SIZE 100
#define NRUNS 100

void doRunsAndGetStats(void) {
    std::vector<int> seqResults;
    BitGenerator bitGen = BitGenerator();
    int total = 0;
    int length = 100;
    
    for (int i = 0; i < 100; i++) {
        std::vector<int> j = runSequentialPathLengths(length, bitGen);
        int longest = *std::max_element(j.begin(), j.end());
        seqResults.push_back(longest);
        total += longest;
    }
    int maxx = *std::max_element(seqResults.begin(), seqResults.end());
    int minn = *std::min_element(seqResults.begin(), seqResults.end());

    std::cout << "max: " << maxx << std::endl;
    std::cout << "min: " << minn << std::endl;
    std::cout << "ave: " << total / 100 << std::endl;

}

// Run the sqrt n sneaky fast simulation for a defaut setup
// Note that it will run it for (n-1)*(n-1) + 1
std::vector<int> runFastSequentialPathLengths(int n, BitGenerator &bitGen) {
    // * * * * * * * * * * 0-9 here length = 10
    // * - - * - - * - - * 0-3 here n = 4
    int length = (n-1)*(n-1) + 1;
    std::vector<int> shortPool;

    //std::cout << "Entered function" << std::endl;
    
    // Use 0 as starting location
    int startLoc = 0;
    int minSteps = 0;
    int firstReach = -1;

    // Generate the pool we'll sample from
    std::vector<int> almostFull(n-1, 1);
    for (int i = 0; i < POOL_SIZE; i++) {
        //std::cout << almostFull.back() << std::endl;
        //printVector(almostFull);
        almostFull.back() = 0; // leave the last one empty
        shortPool.push_back(generateAndSavePathLength(n-1,
                    almostFull, bitGen, startLoc, minSteps, -1));
    }
    //std::cout << "finished small. short pool:" << std::endl;
    //printVector(shortPool);

    std::vector<int> listOfLengths;
    std::vector<int> isFull(length, 0);
    // Run the usual simulation
    for (int i = 0; i < length; i++) {
        //std::cout << "in loop. i = " << i << std::endl;
        
        // Record location and path
        int current = 0;
        int pathLength = 0;

        while (isFull[current]) {
            //printVector(isFull);
            int jumpSize;
            
            // Since path, we only need to check to the right
            if (current % (n-1) == 0 && isFull[current + (n-1)]) {
                jumpSize = (n-1); 
            } else {
                jumpSize = 1;
            }

            // If we're on the left-most side, move right
            if (current == 0) {
                current = current + jumpSize;
            } else {
                current = bitGen.getBit() ? current + jumpSize : current - jumpSize;
            }

            if (jumpSize == 1) {
                pathLength++;
            } else {
                // Sample a time from those we generated
                int y = positiveModulo(bitGen.getNum(), POOL_SIZE);
                int jumpTime = shortPool[y];
                pathLength = pathLength + jumpTime;
            }
        }

        // Fill the hole
        isFull[current] = 1;
        listOfLengths.push_back(pathLength);
    }
    //std::cout << "about to return" << std::endl;
    //std::cout << "num nodes: " << listOfLengths.size() << std::endl;
    //printVector(listOfLengths);
    return listOfLengths;
}

// Run the sequential simulation
std::vector< std::vector<int> > runSequentialPaths(int length,
        BitGenerator &bitGen, int startNode,
        bool random, int minSteps, int firstReach) {

    std::vector< std::vector<int> > listOfPaths;
    std::vector<int> isFull(length, 0);

    if (!random) {
        for (int i = 0; i < length; i++) {
            listOfPaths.push_back(generateAndSavePath(
                        length, isFull, bitGen, startNode,
                        minSteps, firstReach));
        }
    } else {
        for (int i = 0; i < length; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), length);
            listOfPaths.push_back(generateAndSavePath(
                        length, isFull, bitGen, startLoc,
                        minSteps, firstReach));
        }
    }

    return listOfPaths;
}

// Run the sequential simulation, but record only the path length
std::vector<int> runSequentialPathLengths(int length,
        BitGenerator &bitGen, int startNode,
        bool random, int minSteps, int firstReach) {

    std::vector<int> listOfLengths;
    std::vector<int> isFull(length, 0);

    if (!random) {
        for (int i = 0; i < length; i++) {
            listOfLengths.push_back(generateAndSavePathLength(
                        length, isFull, bitGen, startNode,
                        minSteps, firstReach));
        }
    } else {
        for (int i = 0; i < length; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), length);
            listOfLengths.push_back(generateAndSavePathLength(
                        length, isFull, bitGen, startLoc,
                        minSteps, firstReach));
        }
    }

    return listOfLengths;
}

// Run one particle's walk
std::vector<int> generateAndSavePath(
        int length, std::vector<int> &isFull,
        BitGenerator &bitGen, int startNode,
        int minSteps, int firstReach) {

    // Record location and path
    int current = startNode;

    std::vector<int> pathTaken;
    pathTaken.push_back(0);
    bool reached = false;
    if (firstReach == -1) {
        reached = true;
    }

    while (minSteps > 0 || !reached || isFull[current]) {
        //printVector(isFull);
        // If we're on the left-most side, move right
        if (current == 0) {
            current++;
        } else if (current == length - 1) {
            current--;
        } else {
            current = bitGen.getBit() ? current + 1 : current - 1;
        }

        if (current == firstReach) {
            reached = true;
        }
        //std::cout << current << std::endl << std::flush;
        pathTaken.push_back(current);
        minSteps--;
    }

    // Fill the hole
    isFull[current] = 1;
    return pathTaken;
}

// Run one particle's walk length
int generateAndSavePathLength(
        int length, std::vector<int> &isFull,
        BitGenerator &bitGen, int startNode,
        int minSteps, int firstReach) {

    // Record location and path
    int current = startNode;
    int pathLength = 0;
    bool reached = false;
    if (firstReach == -1) {
        reached = true;
    }

    while (pathLength < minSteps || !reached || isFull[current]) {
        //printVector(isFull);
        // If we're on the left-most side, move right
        if (current == 0) {
            current++;
        } else if (current == length - 1) {
            current--;
        } else {
            //current = bitGen.getBit() ? current + 1 : current - 1;
            current = bitGen.getBit() ? current + 1 : current - 1;
        }
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
std::vector< std::vector<int> > runParallelPaths(int length,
        BitGenerator &bitGen, int startNode, bool random) {

    std::vector< std::vector<int> > listOfPaths(length);
    listOfPaths.reserve(length);

    std::vector<int> isFull(length, 0);

    // Store the location of each particle
    // This can be set randomly or not
    int particleLocation[length];
    if (!random) {
        memset(particleLocation, startNode, length*sizeof(int));
    } else {
        int startLoc = positiveModulo(bitGen.getNum(), length);
        memset(particleLocation, startNode, length*sizeof(int));
    }

    // Store a queue of which particle has to move next
    std::queue<int> partQ;
    for (int i = 0; i < length; i++) {
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
            int nextLoc;
            if (loc == 0) {
                nextLoc = 1;
            } else {
                nextLoc = bitGen.getBit() ? loc + 1 : loc - 1;
            }
            particleLocation[part] = nextLoc;
            partQ.push(part);
        }
    }
    return listOfPaths;
}

// Run the parallel simulation, but record only the path length
std::vector<int> runParallelPathLengths(int length,
        BitGenerator &bitGen, int startNode, bool random) {
    std::vector<int> listOfPathLengths(length, 0);
    std::vector<int> isFull(length, 0);

    // Store the location of each particle
    int particleLocation[length];
    if (!random) {
        memset(particleLocation, startNode, length*sizeof(int));
    } else {
        int startLoc = positiveModulo(bitGen.getNum(), length);
        memset(particleLocation, startNode, length*sizeof(int));
    }

    // Store a queue of which particle has to move next
    std::queue<int> partQ;
    for (int i = 0; i < length; i++) {
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
            int nextLoc;
            if (loc == 0) {
                nextLoc = 1;
            } else {
                nextLoc = bitGen.getBit() ? loc + 1 : loc - 1;
            }
            listOfPathLengths[part]++;
            particleLocation[part] = nextLoc;
            partQ.push(part);
        }
    }
    return listOfPathLengths;
}

void runPaths(void) {

    int pathLength = 500;
    std::vector< std::vector <int> > distLengths;

    BitGenerator bitGen = BitGenerator();

    for (int i = 0; i < NRUNS; i++) {
        std::vector<int> walkLength = runSequentialPathLengths(pathLength, bitGen);    
        distLengths.push_back(walkLength);
    }

    print2dVector(distLengths);
}

// Run the sequential simulation
std::vector< std::vector<int> > runSequentialCycle(
        int length, BitGenerator &bitGen,
        bool random, int minSteps) {

    std::vector< std::vector<int> > listOfPaths;
    std::vector<int> isFull(length, 0);

    if (!random) {
        int startNode = 0;
        for (int i = 0; i < length; i++) {
            listOfPaths.push_back(generateAndSaveCycle(
                        length, isFull, bitGen, startNode,
                        minSteps));
        }
    } else {
        for (int i = 0; i < length; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), length);
            listOfPaths.push_back(generateAndSaveCycle(
                        length, isFull, bitGen, startLoc,
                        minSteps));
        }
    }

    return listOfPaths;
}

// Run the sequential simulation, but record only the path length
std::vector<int> runSequentialCycleLengths(
        int length, BitGenerator &bitGen,
        bool random, int minSteps) {

    std::vector<int> listOfLengths;
    std::vector<int> isFull(length, 0);

    if (!random) {
        int startNode = 0;
        for (int i = 0; i < length; i++) {
            listOfLengths.push_back(generateAndSaveCycleLengths(
                        length, isFull, bitGen, startNode,
                        minSteps));
        }
    } else {
        for (int i = 0; i < length; i++) {
            int startLoc = positiveModulo(bitGen.getNum(), length);
            listOfLengths.push_back(generateAndSaveCycleLengths(
                        length, isFull, bitGen, startLoc,
                        minSteps));
        }
    }

    return listOfLengths;
}

// Run one particle's walk
std::vector<int> generateAndSaveCycle(
        int length, std::vector<int> &isFull,
        BitGenerator &bitGen, int startNode,
        int minSteps) {

    // Record location and path
    int current = startNode;

    std::vector<int> pathTaken;
    pathTaken.push_back(0);

    while (minSteps > 0 || isFull[current]) {
        //printVector(isFull);
        // If we're on the left-most side, move right
        current = bitGen.getBit() ? current + 1 : current - 1;
        current = positiveModulo(current, length);
        assert(current > 0);

        //std::cout << current << std::endl << std::flush;
        pathTaken.push_back(current);
        minSteps--;
    }

    // Fill the hole
    isFull[current] = 1;
    return pathTaken;
}

// Run one particle's walk length
int generateAndSaveCycleLengths(
        int length, std::vector<int> &isFull,
        BitGenerator &bitGen, int startNode,
        int minSteps) {

    // Record location and path
    int current = startNode;
    int pathLength = 0;
    bool reached = false;

    while (pathLength < minSteps || isFull[current]) {
        //printVector(isFull);
        // If we're on the left-most side, move right

        current = bitGen.getBit() ? current + 1 : current - 1;
        current = positiveModulo(current, length);

        pathLength++;
    }

    // Fill the hole
    isFull[current] = 1;
    return pathLength;
}


// Run the parallel simulation
std::vector< std::vector<int> > runParallelCycle(
        int length, BitGenerator &bitGen,
        int startNode, bool random) {

    std::vector< std::vector<int> > listOfPaths(length);
    listOfPaths.reserve(length);

    std::vector<int> isFull(length, 0);

    // Store the location of each particle
    // This can be set randomly or not
    int particleLocation[length];
    if (!random) {
        memset(particleLocation, startNode, length*sizeof(int));
    } else {
        int startLoc = positiveModulo(bitGen.getNum(), length);
        memset(particleLocation, startNode, length*sizeof(int));
    }

    // Store a queue of which particle has to move next
    std::queue<int> partQ;
    for (int i = 0; i < length; i++) {
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
            int nextLoc;
            nextLoc = bitGen.getBit() ? loc + 1 : loc - 1;
            nextLoc = positiveModulo(nextLoc, length);
            particleLocation[part] = nextLoc;
            partQ.push(part);
        }
    }
    return listOfPaths;
}

// Run the parallel simulation, but record only the path length
std::vector<int> runParallelCycleLengths(
        int length, BitGenerator &bitGen,
        int startNode, bool random) {
    std::vector<int> listOfPathLengths(length, 0);
    std::vector<int> isFull(length, 0);

    // Store the location of each particle
    int particleLocation[length];
    if (!random) {
        memset(particleLocation, startNode, length*sizeof(int));
    } else {
        int startLoc = positiveModulo(bitGen.getNum(), length);
        memset(particleLocation, startNode, length*sizeof(int));
    }

    // Store a queue of which particle has to move next
    std::queue<int> partQ;
    for (int i = 0; i < length; i++) {
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
            int nextLoc;
            nextLoc = bitGen.getBit() ? loc + 1 : loc - 1;
            nextLoc = positiveModulo(nextLoc, length);
            listOfPathLengths[part]++;
            particleLocation[part] = nextLoc;
            partQ.push(part);
        }
    }
    return listOfPathLengths;
}
