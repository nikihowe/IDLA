// (c) Nikolaus Howe 2018

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <stack>
#include "util.hpp"
#include "standardTransformations.hpp"

// Copy paste sequential->parallel memory-less version
// Unlike python, the vector is passed by value by default
// (aka "deep copy" is performed automatically)
std::vector< std::vector<int> > sequentialToParallel(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size(); // also equals number of particles
    std::vector<int> restingPlace(numNodes);

    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }
    
    std::vector<int> haveSeen(numNodes, 0);

    int numLeft = numNodes;
    for (int j = 0; numLeft > 0; j++) {
        for (int i = 0; i < numNodes; i++) {
            if (j < pathList[i].size()) {
                int curLoc = pathList[i][j]; // current particle location
                if (!haveSeen[curLoc]) {
                    // Cut-and-paste if necessary
                    if (j + 1 < pathList[i].size()) {
                        copyPaste(j+1, pathList[i], pathList[restingPlace[curLoc]]);
                        
                        // Update the resting places
                        int lower = pathList[i].back();
                        int upper = pathList[restingPlace[curLoc]].back();
                        swap(restingPlace[upper], restingPlace[lower]);

                    }
                    haveSeen[curLoc] = 1;
                    numLeft--;
                }
            }
        }
    }
    return pathList;
}

// Copy paste sequential->parallel
// Unlike python, the vector is passed by value by default
// (aka "deep copy" is performed automatically)
// Returs the distribution of pastes
std::vector<int> sequentialToParallelDistribution(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size(); // also equals number of particles
    std::vector<int> restingPlace(numNodes);
    std::vector<int> pasteDistribution(numNodes, 0);

    // Initialize restingPlace to represent the
    // resting location of each particle
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }
    
    std::vector<int> haveSeen(numNodes, 0);

    int numLeft = numNodes;
    for (int j = 0; numLeft > 0; j++) {
        for (int i = 0; i < numNodes; i++) {
            if (j < pathList[i].size()) {
                int curLoc = pathList[i][j]; // current particle location
                if (!haveSeen[curLoc]) {
                    // Cut-and-paste if necessary
                    if (j + 1 < pathList[i].size()) {
                        copyPaste(j+1, pathList[i], pathList[restingPlace[curLoc]]);
                        
                        // Remember we pasted to this place
                        pasteDistribution[restingPlace[curLoc]]++;

                        // Update the resting places
                        int lower = pathList[i].back();
                        int upper = pathList[restingPlace[curLoc]].back();
                        swap(restingPlace[upper], restingPlace[lower]);
                        

                    }
                    haveSeen[curLoc] = 1;
                    numLeft--;
                }
            }
        }
    }
    return pasteDistribution;
}

// Copy paste sequential->parallel, but only remember
// how many times the max length was updated
// Unlike python, the vector is passed by value by default
int sequentialToParallelCountMaxChange(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size(); // also equals number of particles
    std::vector<int> restingPlace(numNodes);
    int lengthOfMax = std::max_element(
            pathList.begin(), pathList.end(), Longest)->size();
    int numMaxChanges = 0;

    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }
    
    std::vector<int> haveSeen(numNodes);

    int numLeft = numNodes;
    for (int j = 0; numLeft > 0; j++) {
        for (int i = 0; i < numNodes; i++) {
            if (j < pathList[i].size()) {
                int curLoc = pathList[i][j]; // current particle location
                if (!haveSeen[curLoc]) {
                    // Cut-and-paste if necessary
                    if (j + 1 < pathList[i].size()) {
                        copyPaste(j+1, pathList[i], pathList[restingPlace[curLoc]]);
                        if (pathList[restingPlace[curLoc]].size() > lengthOfMax) {
                            lengthOfMax = pathList[restingPlace[curLoc]].size();
                            numMaxChanges++;
                        }
                        
                        // Update the resting places
                        int lower = pathList[i].back();
                        int upper = pathList[restingPlace[curLoc]].back();
                        swap(restingPlace[upper], restingPlace[lower]);
                    }
                    haveSeen[curLoc] = 1;
                    numLeft--;
                }
            }
        }
    }
    return numMaxChanges;
}

// Copy paste parallel->sequential, memoryfree
std::vector< std::vector<int> > parallelToSequential(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size();
    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    std::vector<bool> haveSeen(numNodes, false);

    for (int part = 0; part < pathList.size(); part++) {
        for (int step = 0; step < pathList[part].size(); step++) {
            int loc = pathList[part][step];
            if (!haveSeen[loc]) {
                haveSeen[loc] = true;
                if (step + 1 < pathList[part].size()) {
                    copyPaste(step + 1,
                            pathList[part], pathList[restingPlace[loc]]);
                    // Update the resting places
                    int lower = pathList[part].back();
                    int upper = pathList[restingPlace[loc]].back();
                    swap(restingPlace[upper], restingPlace[lower]);

                    break;
                }
            }
        }
    }
    return pathList;
}
