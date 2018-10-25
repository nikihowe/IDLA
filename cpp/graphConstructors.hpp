// (c) Nikolaus Howe 2018

#ifndef GRAPH_CONSTRUCTORS_HPP
#define GRAPH_CONSTRUCTORS_HPP

// Returns an adjacency list representing
// a path graph of length nNodes
std::vector< std::vector<int> > makePath(int nNodes);

// Returns an adjacency list representing
// a cycle graph with nNodes nodes
std::vector< std::vector<int> > makeCycle(int nNodes);

// Returns an adjacency list representing
// a complete graph with nNodes nodes
std::vector< std::vector<int> > makeComplete(int nNodes);

// Returns an adjacency list representing
// a binary tree of depth "depth"
// (hence with (2^depth) - 1 nodes)
std::vector< std::vector<int> > makeBinTree(int depth);

// Returns an adjacency list representing
// a square grid (or torus) of width "width"
// (hence with width*width nodes)
std::vector< std::vector<int> > makeGrid(int width, bool torus = false);

// Returns an adjacency list representing
// a square torus of width "width"
// (hence with width*width nodes)
std::vector< std::vector<int> > makeTorus(int width);

#endif // GRAPH_CONSTRUCTORS_HPP
