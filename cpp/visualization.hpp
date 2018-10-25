// (c) Nikolaus Howe 2018

#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

// Given a square 2D grid of width "width", with given isFull vector,
// and a desired number of boxes "numSites", divide the 
// 2D grid into that number of squares (or the best possible approximation).
// Then count what proportion of those squares contains at least one
// empty spot
double gridClustering(std::vector<int> &isFull, int width, int numSites);

// Given a 2D grid of width "width", with given isFull vector,
// calculate the total manhattan distance between all pairs
// of empty points, and divide by the number of pairs
// (in effect, calculate the average manhattan distance
// between two empty nodes)
double naiveGridClustering(std::vector<int> &isFull, int width);

// Calculate clustering on the path. To do so, divide the
// path into a number of bins equal to the number
// of empty nodes in the isFull vector. Then return
// the proportion of such bins which have at least
// one particle in them.
double pathClustering(std::vector<int> &isFull);

// Calculate the clustering on the path for fill-from-the-left
// (as rotor-routor or standard IDLA would do), for various
// numbers of free nodes, using the pathClustering algorithm.
// Also calculate the clustering on the path for random-start
// IDLA, using the pathClustering algorithm.
// Print the results.
void observePathClustering(void);

// Calculate the clustering on the grid for fill-from-the-top-left
// or random IDLA (depending whether one sets random = true or false)
// for different stages of the fill.
void observeFill(bool random = false);

#endif // VISUALIZATION_HPP
