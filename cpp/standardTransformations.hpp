// (c) Nikolaus Howe 2018

#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "util.hpp"

// StP1, as described in the paper draft
// Given a sequential IDLA run, transforms it
// into a corresponding parallel IDLA run
std::vector< std::vector<int> > sequentialToParallel(
        std::vector< std::vector<int> > paths);

// StP1, but return the distribution of
// paste locations instead of the transformed paths
std::vector<int> sequentialToParallelDistribution(
        std::vector< std::vector<int> > paths);

// StP1, but return the number of times the title
// "longest path" changed particles during the transformation
int sequentialToParallelCountMaxChange(
        std::vector< std::vector<int> > paths);

// PtS1, as described in the paper draft
// Given a parallel IDLA run, transforms it
// into a corresponding sequential IDLA run
std::vector< std::vector<int> > parallelToSequential(
        std::vector< std::vector<int> > paths);

#endif // TRANSFORMATION_HPP
