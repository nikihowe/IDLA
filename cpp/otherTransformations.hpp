// (c) Nikolaus Howe 2018

#ifndef BONUS_TRANSFORMATIONS_HPP
#define BONUS_TRANSFORMATIONS_HPP

/**
 * These transformations were created as an extension to the ones found
 * in transformations.hpp. They transform sequential to parallel
 * and parallel to sequential in various ways, though I was
 * unable to find any with an explicit inverse.
 */

/**
 * StP2, as defined in the paper draft,
 * implemented as in the paper, using
 * vertex colouring.
 */
std::vector< std::vector<int> > sequentialToParallel2(
        std::vector< std::vector<int> > paths);

/**
 * StP2, as defined in the paper draft,
 * implemented with a stack.
 */
std::vector< std::vector<int> > sequentialToParallel2Stack(
        std::vector< std::vector<int> > paths);

/**
 * PtS2, as defined in the paper draft,
 * implemented as in the paper, using
 * vertex colouring.
 */
std::vector< std::vector<int> > parallelToSequential2(
        std::vector< std::vector<int> > pathList);

/**
 * StP2, stack-based, but returns the distribution of
 * pastes instead of the list of walks
 */
std::vector<int> sequentialToParallelDistribution2(
        std::vector< std::vector<int> > paths);

/**
 * Step through the nodes in parallel order, and as 
 * soon as a node is reached, make the copy
 * for the earliest parallel occurrence of that node.
 * Repeat until all nodes are coloured.
 */
std::vector< std::vector<int> > parallelToSequential2Fast(
        std::vector< std::vector<int> > pathList);

/**
 * Step through the nodes in order, each time making the best possible
 * copy-paste for that node (copy from the walk containing the
 * earliest parallel step we see that node). Keep stepping through all nodes
 * until there is a complete round with no changes.
 */
std::vector< std::vector<int> > sequentialToParallel3(
        std::vector< std::vector<int> > paths);

/**
 * Step through the nodes in order, each time stepping through
 * the particle paths in parallel sense, making a copy-paste when
 * the current instance of the particle is the earliest (in parallel
 * sense) that we have seen it).
 */
std::vector< std::vector<int> > parallelToSequential3(
        std::vector< std::vector<int> > pathList);

/** PtS1, but with a twist: now we focus on one node at
 * a time (starting with the first one, then the second,
 * etc.). So we read through in sequential order until
 * we see the node we're looking for, and we fix it
 * so that it would be in sequential order. We run through
 * all the nodes repeating this until there are no more
 * changes for a whole run through examining all the nodes.
 */
std::vector< std::vector<int> > parallelToSequential4(
        std::vector< std::vector<int> > pathList);

// Get first ParticleAndStep of a given node (sequential)
ParticleAndStep getFirstOccurrenceS(
        std::vector< std::vector<int> > &pathList, int node);

// Get first ParticleAndStep of a given node (parallel)
ParticleAndStep getFirstOccurrenceP(
        std::vector< std::vector<int> > &pathList, int node);

// Get first uncoloured ParticleAndStep (sequential)
ParticleAndStep getFirstUncolouredS(std::vector< std::vector<int> > &c);

// Get first uncoloured ParticleAndStep (parallel)
ParticleAndStep getFirstUncolouredP(std::vector< std::vector<int> > &c);

// Returns a vector, indexed by node, which says the
// first occurrence (in parallel terms) of a node
// in a given IDLA run.
std::vector<ParticleAndStep> calculateFirstParallelOccurrenceVector(
        std::vector< std::vector<int> > &pathList);

// Initialize the PS vector to all INT_MAX values.
std::vector<ParticleAndStep> initializeFirstParallelOccurrenceVector(int numNodes);

#endif // BONUS_TRANSFORMATIONS_HPP
