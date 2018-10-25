// (c) Nikolaus Howe 2018

#ifndef OPTIMIZED_IDLA_HPP
#define OPTIMIZED_IDLA_HPP

#include <vector>
#include <cstdlib>


// Using a bootstrapping technique, run the IDLA simulation
// on a path of (length - 1)*(length - 1)+1 nodes. Returns
// an approximation of the length of the walks.
std::vector<int> runFastSequentialPathLengths(int length, BitGenerator &randBits);

// Print the max, min, and average "max length"
// for a number of IDLA simulations on the path.
// Used primarily for information purposes.
void doRunsAndGetStats(void);

// Run one particle's walk on the path
// of length "length", where "isFull" says
// which nodes are filled already
std::vector<int> generateAndSavePath(
        int length, std::vector<int> &isFull,
        BitGenerator &randBits, int startNode,
        int minSteps, int firstReach);

// Like generateAndSavePath,
// but returns the walk length
int generateAndSavePathLength(
        int length, std::vector<int> &isFull,
        BitGenerator &randBits, int startNode,
        int minSteps, int firstReach);

// Run the sequential IDLA process
// on a path of length "length", with options:
// startNode: node where the walk starts
// random: whether or not to start
//     from a uniform random node
// minSteps: min number of steps the walk
//     must take before settling
// firstReach: node the walk must reach
//     before settling
std::vector< std::vector<int> > runSequentialPaths(int length,
        BitGenerator &randBits, int startNode = 0,
        bool random = false, int minSteps = 0,
        int firstReach = -1);

// Run the sequential IDLA process
// on a path of length "length",
// returning the lengths of the particles' walks.
std::vector<int> runSequentialPathLengths(int length,
        BitGenerator &randBits, int startNode = 0,
        bool random = false, int minSteps = 0,
        int firstReach = -1);

// Run the parallel IDLA process
// of a path of length "length",
// returning the walks performed by the particles
std::vector< std::vector<int> > runParallelPaths(int length,
        BitGenerator &randBits, int startNode,
        bool random = false);

// Run the parallel IDLA process
// on a path of length "length",
// returning the lengths of the walks
// performed by the particles
std::vector<int> runParallelPathLengths(int length,
        BitGenerator &randBits, int startNode,
        bool random = false);

// Print the IDLA processes of a number
// of IDLA simulations on the path.
// Mostly used for testing purposes.
void runPaths(void);

// Run the sequential IDLA process
// on a cycle of length "length", with options:
// startNode: node where the walk starts
// random: whether or not to start
//     from a uniform random node
// minSteps: min number of steps the walk
//     must take before settling
// firstReach: node the walk must reach
//     before settling
std::vector< std::vector<int> > runSequentialCycle(
        int length, BitGenerator &randBits,
        bool random = false, int minSteps = 0);

// Run the sequential IDLA process
// on a cycle of length "length",
// returning the lengths of the particles' walks.
std::vector<int> runSequentialCycleLengths(
        int length, BitGenerator &randBits,
        bool random = false, int minSteps = 0);

// Run one particle's walk on the cycle
// of length "length", where "isFull" says
// which nodes are filled already
std::vector<int> generateAndSaveCycle(
        int length, std::vector<int> &isFull,
        BitGenerator &randBits, int startNode,
        int minSteps);

// Like generateAndSaveCycle,
// but returns the walk length
int generateAndSaveCycleLengths(
        int length, std::vector<int> &isFull,
        BitGenerator &randBits, int startNode,
        int minSteps);

// Run the parallel IDLA process
// of a cycle of length "length",
// returning the walks performed by the particles
std::vector< std::vector<int> > runParallelCycle(
        int length, BitGenerator &randBits,
        int startNode, bool random = false);

// Run the parallel IDLA process
// on a cycle of length "length",
// returning the lengths of the walks
// performed by the particles
std::vector<int> runParallelCycleLengths(
        int length, BitGenerator &randBits,
        int startNode, bool random = false);

#endif // OPTIMIZED_IDLA_HPP
