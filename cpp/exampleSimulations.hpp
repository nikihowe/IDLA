// (c) Nikolaus Howe 2018

#ifndef EXAMPLE_SIMULATIONS_HPP
#define EXAMPLE_SIMULATIONS_HPP

/**
 * These are some examples of ways you might want to use
 * the software in this directory. To run, just modify
 * main.cpp to run the desired function, or write your own.
 */

// Test the performance of rotor-router,
// at each node pointing in random directions to start,
// and all moving up->left->right->down, on the 2D Grid,
// and compare it with standard IDLA (both starting from top-left
// corner). Note that rotor-router is somewhat faster, seemingly
// no matter the starting configuration.
void compareRotorRouterAndSequentialIDLA(void);

// Run the seq->par transformation, and return the
// distribution of paste locations, averaged over 100 runs
// (can use seq->par1 or seq->par2 for two different transformations)
void printPasteLocations(void);

// Feed a specific run into the transformation,
// to make sure it performs as expected.
void checkTransformation(void);

// Check whether two transformations seem to be
// inverses of one another.
void checkIfInverses(void);

// Look at the distribution of lengths
// after the two different paste operations
void examinePostPasteWalkLengths(void);

// Check performance of one-step-lookahead IDLA
void exploreOneStepLookahead(void);

// Explore the momentum speedup landscape
// (to discover the optimal momentum value
// for fastest IDLA time in grids of different sizes)
void exploreMomentumLandscape(void);

// Compare the dispersion time for momentum-based IDLA
// and standard IDLA on 2D grids of various sizes.
void compareMomentumAndNormal(void);

// Compare the dispersion time for different versions
// of the IDLA process, printing out results.
void runVariousPaths(void);

// Compare the average dispersion times of the path
// and the cycle, printing out the results.
void comparePathAndCycle(void);

// Run one simulation of Sequential IDLA on
// a complete graph of size numNodes,
// printing the length of each walk.
void runComplete(int numNodes);

// Compare the dispersion time for different versions
// of the IDLA process on a number of sizes
// of 2D grid, printing out the results.
void runGrids(void);

// Compare the dispersion time for different versions
// of the IDLA process on a number of sizes
// of binary tree, printing out the results.
void runBinTrees(void);

// Fill the 2D grid with IDLA from stratified points
// (points interspersed evently throughout the graph),
// as opposed to IDLA from one corner (slower) or 
// IDLA from random starting locations (faster)
void fillFromStratified(void);

#endif // EXAMPLE_SIMULATIONS_HPP
