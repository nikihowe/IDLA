// (c) Nikolaus Howe 2018

#ifndef MODIFIED_IDLA_HPP
#define MODIFIED_IDLA_HPP

// Starting with (numNodes^power) nodes already filled,
// estimate how long it takes, on average,
// to hit one of the remaining empty nodes in the graph,
// starting a random walk from random location.
// Do this by simulating a number of walks
// starting from each start vertex, and then taking
// the average length over all of those.
double studyRandomPathFillViaSimulation(int numNodes, double power);

// Do the same as above, but by using the expected
// value for walks lengths based on Gambler's Ruin
double studyRandomPathFillViaExpectationPower(int numNodes, double power);

// Study the expected number of steps for the next particle
// to settle on a path graph of size numNodes, for every
// step of a sequential IDLA fill.
std::vector<double> studyRandomPathFillGamblersRuin(int numNodes);

// Calculate the expected number of steps for the next
// particle to settle from any point.
double getGamblersRuin(std::vector<int> &isFull);

// Run the Sequential IDLA process with all the standard
// settings, but include a momentum, 0 <= m <= 1, which
// indicates the likelihood of continuing straight instead
// of choosing randomly from neighbours.
std::vector< std::vector<int> > runMomentumSequential(
        std::vector< std::vector<int> > &g, double momentum);

// Run one particle's random walk in IDLA on graph,
// with the momentum described above.
// Returns the vector of nodes in the walk.
std::vector<int> singleParticleMomentum(
        std::vector< std::vector<int> > &g,
        std::vector<int> &isFull,
        BitGenerator &bg,
        int lastDirection,
        double momentum);

// Run the Sequential IDLA process with all the standard
// settings, but allow a particle to see whether its 
// immediate neighbours are full or not. If at least one
// is not full, choose randomly among those for the next
// step. Otherwise, choose randomly as with standard IDLA.
std::vector< std::vector<int> > runOneStepLookaheadSequential(
        std::vector< std::vector<int> > &g);

// Run the Sequential IDLA process with one-step
// lookahead as above, but return the lengths
// of the walks instead of the walks themselves.
std::vector<int> runOneStepLookaheadSequentialLength(
        std::vector< std::vector<int> > &g);

// Run one particle's random walk in IDLA on graph,
// with one-step lookahead described above.
std::vector<int> singleParticleLookahead(
        std::vector< std::vector<int> > &graph,
        std::vector<int> &isFull,
        BitGenerator &bitGen);

// Run the rotor-router process on graph, optionally
// starting from a given node or a random location each time,
// with random initialization of rotor directions,
// but where rotor always follows up->left->right->down
// sequence of directions
std::vector<int> runSequentialRotorRouterLengths(
        std::vector< std::vector<int> > &g,
        int startNode = 0, bool randStart = false);

// Given a graph, an isFull vector, and a nextMove vector
// describing where to go from any node (as in the
// rotor-router process), run one particle through
// the graph, following and updating where
// the rotors point
int followPath(std::vector< std::vector<int> > &graph,
        std::vector<int> &isFull, std::vector<int> &nextMove,
        int startNode);

// Try to create a dispersion process that settles in faster
// than random time. Strategy: have sqrt(n) "origin" points,
// and randomly generate from them
// Note: it seems this approach is actually not faster than random.
std::vector<int> generateFastLocations(int width);

// Given a graph g and a list of start points, fill the graph
// with standard sequential IDLA process, but starting from those start
// points (choosing randomly between them each time).
// Returns the lengths of the fill times starting from points startPoints
std::vector<int> fillFromPoints(std::vector< std::vector<int> > &g,
        std::vector<int> &startPoints, BitGenerator &bitGen);

#endif // MODIFIED_IDLA_HPP
