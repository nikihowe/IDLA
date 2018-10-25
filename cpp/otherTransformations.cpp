// (c) Nikolaus Howe

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <stack>
#include "util.hpp"
#include "otherTransformations.hpp"

// Sequential to Parallel, memoryfull
std::vector< std::vector<int> > sequentialToParallel2(
        std::vector< std::vector<int> > pathList) {
    
    int numNodes = pathList.size();

    // Colour graph
    std::vector< std::vector<int> > c = pathList;
    for (int i = 0; i < pathList.size(); i++) {
        for (int j = 0; j < pathList[i].size(); j++) {
            c[i][j] = 0;
        }
    }

    std::vector<int> firstSeenPlace(numNodes, INT_MAX);

    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    std::vector<ParticleAndStep> firstSeenAtSteps = 
        initializeFirstParallelOccurrenceVector(numNodes);

    ParticleAndStep loc;
    ParticleAndStep end;
    while ((loc = getFirstUncolouredS(c)) != end) {
        c[loc.particle][loc.step] = 1;
        int node = pathList[loc.particle][loc.step];
        if (parLT(loc, firstSeenAtSteps[node])) {
            
            // Sort out this node
            copyPaste(loc.step + 1, pathList[loc.particle],
                    pathList[restingPlace[node]]);
            copyPaste(loc.step + 1, c[loc.particle],
                    c[restingPlace[node]]);
            
            // Update the resting places
            int lower = pathList[loc.particle].back();
            int upper = pathList[restingPlace[node]].back();
            swap(restingPlace[upper], restingPlace[lower]);
            
            // Update the first seen vector
            firstSeenAtSteps[node] = loc;
        }
    }
    return pathList;
}

std::vector< std::vector<int> > sequentialToParallel2Stack(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size();
    // Record where each particle ended up
    // (where did this particle end up)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    // Record at which step we first saw particle
    std::vector<int> firstSeenPlace(numNodes, INT_MAX);
    
    // Put information onto the stack about what
    // to evaluate next
    // map will hold <particle #, step #>
    std::stack<int> nextPathToEvaluate;

    // Record whether or not this is the first time
    // we're seeing this particle
    std::vector<bool> seen(numNodes, false);

    // Set up a map recording where each particle is
    // (start by not having seen anything)
    std::map<int, int> finishedMap;
    for (int i = 0; i < numNodes; i++) {
        finishedMap[i] = 0;
    }

    // Put all the particles into the stack
    for (int i = pathList.size() - 1; i >= 0; i--) {
        nextPathToEvaluate.push(i);
    }

    // Now we go through, treating as though
    // sequential, but requiring parallel rules
    while (!nextPathToEvaluate.empty()) {
        int part = nextPathToEvaluate.top(); // particle index
        nextPathToEvaluate.pop();

        // Check off all legal steps
        for ( ; finishedMap[part] < pathList[part].size(); (finishedMap[part])++) {
            int stepNum = finishedMap[part];

            int curLoc = pathList[part][stepNum];

            if (seen[curLoc]) {
                if (stepNum < firstSeenPlace[curLoc] ||
                        (stepNum == firstSeenPlace[curLoc] && part < restingPlace[curLoc])) {
                    firstSeenPlace[curLoc] = stepNum;
                    int prevLoc = pathList[part][stepNum-1];
                    // Move everything after this to the relevant place
                    copyPaste(stepNum + 1, pathList[part], pathList[restingPlace[curLoc]]);
                    nextPathToEvaluate.push(restingPlace[curLoc]);

                    // Update the resting places
                    int lower = pathList[part].back();
                    int upper = pathList[restingPlace[curLoc]].back();
                    swap(restingPlace[upper], restingPlace[lower]);
                    
                    // Stop looking at this particle and jump back to the next
                    break;
                }
            } else {
                // Mark as seen if this was the first time we saw it
                seen[curLoc] = true;
                firstSeenPlace[curLoc] = stepNum;
            }
        }
    }
    return pathList;
}

std::vector<int> sequentialToParallelDistribution2(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size();
    // Record where each particle ended up
    // (where did this particle end up)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    // Record at which step we first saw particle
    std::vector<int> firstSeenPlace(numNodes, INT_MAX);

    // Record the location of the paste
    std::vector<int> pasteDistribution(numNodes, 0);
    
    // Put information onto the stack about what
    // to evaluate next
    // map will hold <particle #, step #>
    std::stack<int> nextPathToEvaluate;

    // Record whether or not this is the first time
    // we're seeing this particle
    std::vector<bool> seen(numNodes, false);

    // Set up a map recording where each particle is
    // (start by not having seen anything)
    std::map<int, int> finishedMap;
    for (int i = 0; i < numNodes; i++) {
        finishedMap[i] = 0;
    }

    // Put all the particles into the stack
    for (int i = pathList.size() - 1; i >= 0; i--) {
        nextPathToEvaluate.push(i);
    }

    // Now we go through, treating as though
    // sequential, but requiring parallel rules
    while (!nextPathToEvaluate.empty()) {
        int part = nextPathToEvaluate.top(); // particle index
        nextPathToEvaluate.pop();

        // Check off all legal steps
        for ( ; finishedMap[part] < pathList[part].size(); (finishedMap[part])++) {
            int stepNum = finishedMap[part];

            int curLoc = pathList[part][stepNum];

            if (seen[curLoc]) {
                if (stepNum < firstSeenPlace[curLoc]) {
                    firstSeenPlace[curLoc] = stepNum;
                    int prevLoc = pathList[part][stepNum-1];
                    // Move everything after this to the relevant place
                    copyPaste(stepNum + 1, pathList[part], pathList[restingPlace[curLoc]]);
                    nextPathToEvaluate.push(restingPlace[curLoc]);

                    // Remember we pasted to this place
                    pasteDistribution[restingPlace[curLoc]]++;

                    // Update the resting places
                    int lower = pathList[part].back();
                    int upper = pathList[restingPlace[curLoc]].back();
                    swap(restingPlace[upper], restingPlace[lower]);
                    
                    // Stop looking at this particle and jump back to the next
                    break;
                }
            } else {
                // Mark as seen if this was the first time we saw it
                seen[curLoc] = true;
                firstSeenPlace[curLoc] = stepNum;
            }
        }
    }
    return pasteDistribution;
}

// Copy paste parallel->sequential, memoryfull
std::vector< std::vector<int> > parallelToSequential2(
        std::vector< std::vector<int> > pathList) {

    // Colour graph
    std::vector< std::vector<int> > c = pathList;
    for (int i = 0; i < pathList.size(); i++) {
        for (int j = 0; j < pathList[i].size(); j++) {
            c[i][j] = 0;
        }
    }

    int numNodes = pathList.size();
    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    bool done = false;
    std::vector<ParticleAndStep> firstSeenAtSteps = initializeFirstParallelOccurrenceVector(numNodes);
    ParticleAndStep loc;
    ParticleAndStep end;
    while ((loc = getFirstUncolouredP(c)) != end) {
        c[loc.particle][loc.step] = 1;
        int node = pathList[loc.particle][loc.step];
        if (seqLT(loc, firstSeenAtSteps[node])) {
            copyPaste(loc.step + 1, pathList[loc.particle],
                    pathList[restingPlace[node]]);
            copyPaste(loc.step + 1, c[loc.particle],
                    c[restingPlace[node]]);
            // Update the resting places
            int lower = pathList[loc.particle].back();
            int upper = pathList[restingPlace[node]].back();
            swap(restingPlace[upper], restingPlace[lower]);
            
            // Update the first seen vector
            firstSeenAtSteps[node].step = loc.step;
            firstSeenAtSteps[node].particle = loc.particle;
        }
    }
    return pathList;
}

std::vector< std::vector<int> > parallelToSequential2Fast(
        std::vector< std::vector<int> > pathList) {

    // Colour graph
    std::vector< std::vector<int> > c = pathList;
    for (int i = 0; i < pathList.size(); i++) {
        for (int j = 0; j < pathList[i].size(); j++) {
            c[i][j] = 0;
        }
    }

    int numNodes = pathList.size();
    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    std::vector<ParticleAndStep> firstSeenAtSteps = initializeFirstParallelOccurrenceVector(numNodes);
    ParticleAndStep loc;
    ParticleAndStep end;
    while ((loc = getFirstUncolouredP(c)) != end) {
        c[loc.particle][loc.step] = 1;
        int node = pathList[loc.particle][loc.step];
        ParticleAndStep firstOccurred = getFirstOccurrenceS(pathList, node);
        copyPaste(firstOccurred.step + 1, pathList[firstOccurred.particle],
                pathList[restingPlace[node]]);
        copyPaste(firstOccurred.step + 1, c[firstOccurred.particle],
                c[restingPlace[node]]);
        // Update the resting places
        int lower = pathList[firstOccurred.particle].back();
        int upper = pathList[restingPlace[node]].back();
        swap(restingPlace[upper], restingPlace[lower]);
        
        // Update the first seen vector
        firstSeenAtSteps[node].step = firstOccurred.step;
        firstSeenAtSteps[node].particle = firstOccurred.particle;
    }
    return pathList;
}

std::vector< std::vector<int> > sequentialToParallel3(
        std::vector< std::vector<int> > pathList) {
    int numNodes = pathList.size();
    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    bool changed = true;
    while (changed) {
        changed = false;
        std::vector<ParticleAndStep> pSVector = calculateFirstParallelOccurrenceVector(pathList);
        // Go through all the nodes
        for (int node = 0; node < pathList.size(); node++) { // we go in increasing order, but any order would work fine
            int particle = pSVector[node].particle;
            int step = pSVector[node].step;

            if (step + 1 < pathList[particle].size()) {
                // Move everything after this to the relevant place
                copyPaste(step + 1, pathList[particle], pathList[restingPlace[node]]);
                changed = true; // we modified the paths this round
                
                // Update the pSVector
                pSVector = calculateFirstParallelOccurrenceVector(pathList);

                // Update the resting places
                int lower = pathList[particle].back();
                int upper = pathList[restingPlace[node]].back();
                swap(restingPlace[upper], restingPlace[lower]);
            }
        }
    }
    return pathList;
}

std::vector< std::vector<int> > parallelToSequential3(
        std::vector< std::vector<int> > pathList) {

    int numNodes = pathList.size();
    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    bool changed = true;

    int count = 0;
    while (changed && count++ < 3) {
        std::vector<ParticleAndStep> firstSeenAtSteps = initializeFirstParallelOccurrenceVector(numNodes);
        changed = false;
        // we're focusing on node "targetNode"
        for (int targetNode = numNodes - 1; targetNode >= 0; targetNode--) {
            bool done = false;
            for (int step = 0; !done; step++) {
                done = true;
                for (int part = 0; part < numNodes; part++) { // particle
                    if (step >= pathList[part].size()) {
                        continue;
                    }
                    done = false;
                    int node = pathList[part][step];
                    if (node == targetNode) {
                        int firstSeenAtStep = firstSeenAtSteps[node].step;
                        int firstSeenAtPart = firstSeenAtSteps[node].particle;
                        if (part < firstSeenAtPart ||
                                (part == firstSeenAtPart && step < firstSeenAtStep)) {
                            if (step + 1 < pathList[part].size()) {
                                changed = true;
                                copyPaste(step + 1, pathList[part],
                                        pathList[restingPlace[node]]);
                                // Update the resting places
                                int lower = pathList[part].back();
                                int upper = pathList[restingPlace[node]].back();
                                swap(restingPlace[upper], restingPlace[lower]);
                            }

                            // Update the first seen vector
                            firstSeenAtSteps[node].step = step;
                            firstSeenAtSteps[node].particle = part;
                        }
                    }
                }
            }
        }
    }
    return pathList;
}

std::vector< std::vector<int> > parallelToSequential4(
        std::vector< std::vector<int> > pathList) {

    //std::cout << "starting with " << std::endl;
    //print2dVector(pathList);
    int numNodes = pathList.size();
    // Record where each particle ended up
    // (which particle ended up at this node)
    std::vector<int> restingPlace(numNodes);
    for (int i = 0; i < numNodes; i++) {
        int ithListLength = pathList[i].size();
        restingPlace[pathList[i][ithListLength-1]] = i;
    }

    bool changed = true;
    while (changed) {
        changed = false;
        // we're focusing on node "targetNode"
        for (int targetNode = 0; targetNode < numNodes; targetNode++) {
            bool found = false;
            for (int i = 0; !found && i < numNodes; i++) {
                for (int j = 0; !found && j < pathList[i].size(); j++) {
                    int node = pathList[i][j];
                    if (node == targetNode) {
                        found = true;
                        if (j + 1 < pathList[i].size()) {
                            copyPaste(j+1, pathList[i],
                                    pathList[restingPlace[targetNode]]);
                            changed = true;
                            // Update the resting places
                            int lower = pathList[i].back();
                            int upper = pathList[restingPlace[targetNode]].back();
                            swap(restingPlace[upper], restingPlace[lower]);
                        }
                    }
                }
            }
            assert(found);
        }
    }
    return pathList;
}

std::vector<ParticleAndStep> initializeFirstParallelOccurrenceVector(int numNodes) {
    std::vector<ParticleAndStep> pSVector;
    for (int i = 0; i < numNodes; i++) {
        ParticleAndStep init;
        pSVector.push_back(init);
    }
    return pSVector;
}

std::vector<ParticleAndStep> calculateFirstParallelOccurrenceVector(
        std::vector< std::vector<int> > &pathList) {

    std::vector<ParticleAndStep> leftMostOccurrence =
        initializeFirstParallelOccurrenceVector(pathList.size());

    for (int i = 0; i < pathList.size(); i++) {
        for (int j = 0; j < pathList[i].size(); j++) {
            int loc = pathList[i][j];
            if (j < leftMostOccurrence[loc].step) {
                leftMostOccurrence[loc].particle = i;
                leftMostOccurrence[loc].step = j;
            }
        }
    }
    return leftMostOccurrence;
}

// Get first (sequential) uncoloured occurrence of any node
ParticleAndStep getFirstUncolouredS(
        std::vector< std::vector<int> > &c) {
    ParticleAndStep pas;
    for (int i = 0; i < c.size(); i++) {
        for (int j = 0; j < c[i].size(); j++) {
            if (c[i][j] == 0) {
                ParticleAndStep cur = ParticleAndStep(i, j);
                if (seqLT(cur, pas)) {
                    pas = cur;
                }
            }
        }
    }
    return pas;
}

// Get first (parallel) uncoloured occurrence of any node
ParticleAndStep getFirstUncolouredP(
        std::vector< std::vector<int> > &c) {
    ParticleAndStep pas;
    for (int i = 0; i < c.size(); i++) {
        for (int j = 0; j < c[i].size(); j++) {
            if (c[i][j] == 0) {
                ParticleAndStep cur = ParticleAndStep(i, j);
                if (parLT(cur, pas)) {
                    pas = cur;
                }
            }
        }
    }
    return pas;
}

// Get first (sequential) uncoloured occurrence of given node
ParticleAndStep getFirstOccurrenceS(
        std::vector< std::vector<int> > &pathList, int node) {
    ParticleAndStep pas;
    for (int i = 0; i < pathList.size(); i++) {
        for (int j = 0; j < pathList[i].size(); j++) {
            if (pathList[i][j] == node) {
                ParticleAndStep cur = ParticleAndStep(i, j);
                if (seqLT(cur, pas)) {
                    pas = cur;
                }
            }
        }
    }
    return pas;
}

// Get first (parallel) uncoloured occurrence of given node
ParticleAndStep getFirstOccurrenceP(
        std::vector< std::vector<int> > &pathList, int node) {
    ParticleAndStep pas;
    for (int i = 0; i < pathList.size(); i++) {
        for (int j = 0; j < pathList[i].size(); j++) {
            if (pathList[i][j] == node) {
                ParticleAndStep cur = ParticleAndStep(i, j);
                if (parLT(cur, pas)) {
                    pas = cur;
                }
            }
        }
    }
    return pas;
}
