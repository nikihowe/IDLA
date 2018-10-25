// (c) Nikolaus Howe 2018

#ifndef GENERAL_SIMULATION_HPP
#define GENERAL_SIMULATION_HPP

// Run the sequential IDLA process on g,
// returning the length of the longest random walk
std::vector<int> runSimpleGraphGetLengths(
        std::vector< std::vector<int> > &g);

// Run the sequential IDLA process on g,
// returning the paths of the random walks,
// with all the default settings
std::vector< std::vector<int> > runSimpleGraph(
        std::vector< std::vector<int> > &g);

// Run the sequential IDLA process on g,
// returning the length of the longest walk
int runSimpleGraphGetMax(
        std::vector< std::vector<int> > &g);

// Run the sequential IDLA process on graph,
// returning the full walk taken by each particle,
// with options:
// startNode: which node to start the walk from
// random: whether or not to start
//     from a uniform random node
// minSteps: minimum number of steps the
//     walk must take before settling
// firstReach: the location the walk must
//     reach before it is allowed to settle
std::vector< std::vector<int> > runSequential(
        std::vector< std::vector<int> > &graph,
        BitGenerator &randBits, int startNode = 0,
        bool random = false, int minSteps = 0,
        int firstReach = -1);

// Run the sequential IDLA process on graph,
// returning the lengths of the individual particles' walks,
// with same options.
int runSequentialGetMax(
        std::vector< std::vector<int> > &graph,
        BitGenerator &randBits, int startNode = 0,
        bool random = false, int minSteps = 0,
        int firstReach = -1);

// Run the sequential IDLA process on graph,
// returning the length of the longest walk,
// with some options.
std::vector<int> runSequentialLengths(
        std::vector< std::vector<int> > &graph,
        BitGenerator &randBits, int startNode = 0,
        bool random = false, int minSteps = 0,
        int firstReach = -1);

// Run one particle's random walk in IDLA on graph
// Returns the vector of nodes in the walk
std::vector<int> generateAndSave(
        std::vector< std::vector<int> > &graph,
        std::vector<int> &isFull,
        BitGenerator &randBits, int startNode = 0,
        int minSteps = 0, int firstReach = -1);

// Run one particle's walk in IDLA on graph,
// but return only the length of the walk
int generateAndSaveLength(
        std::vector< std::vector<int> > &graph,
        std::vector<int> &isFull,
        BitGenerator &randBits, int startNode = 0,
        int minSteps = 0, int firstReach = -1);

// Run the parallel IDLA process on graph g,
// returning the walks performed by each particle
std::vector< std::vector<int> > runParallel(
        std::vector< std::vector<int> > &g,
        BitGenerator &randBits,
        int startNode = 0, bool random = false);

// Run the parallel IDLA process on graph g,
// returning the lengths of the walks
// performed by each particle
std::vector<int> runParallelLengths(
        std::vector< std::vector<int> > &g,
        BitGenerator &randBits,
        int startNode, bool random = false);

#endif // GENERAL_SIMULATION_HPP
