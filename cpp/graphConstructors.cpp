// (c) Nikolaus Howe

#include <cstdlib>
#include <vector>
#include "util.hpp"

std::vector< std::vector<int> > makePath(int nNodes) {
    std::vector< std::vector<int> > g;

    // Left end
    std::vector<int> s;
    s.push_back(1);
    g.push_back(s);

    for (int i = 1; i < nNodes - 1; i++) {
        std::vector<int> neighbours;
        neighbours.push_back(positiveModulo(i+1, nNodes));
        neighbours.push_back(positiveModulo(i-1, nNodes));
        g.push_back(neighbours);
    }

    // Right end
    std::vector<int> e;
    e.push_back(nNodes - 2);
    g.push_back(e);

    return g;
}

std::vector< std::vector<int> > makeCycle(int nNodes) {
    std::vector< std::vector<int> > g;
    for (int i = 0; i < nNodes; i++) {
        std::vector<int> neighbours;
        neighbours.push_back(positiveModulo(i+1, nNodes));
        neighbours.push_back(positiveModulo(i-1, nNodes));
        g.push_back(neighbours);
    }
    return g;
}

std::vector< std::vector<int> > makeComplete(int nNodes) {
    
    std::vector< std::vector<int> > g;
    for (int i = 0; i < nNodes; i++) {
        std::vector<int> neighbours;
        for (int j = 0; j < nNodes; j++) {
            if (i != j) {
                neighbours.push_back(j);
            }
        }
        g.push_back(neighbours);
    }
    return g;
}

std::vector< std::vector<int> > makeBinTree(int depth) {
    int nNodes = (1 << depth) - 1; // i < pow(2, depth)

    std::vector< std::vector<int> > g;
    for (int i = 0; i < nNodes; i++) { 
        std::vector<int> neighbours;
        // Add parent
        if (i != 0) {
            neighbours.push_back((i-1)/2);
        }
        // Add children
        if (i < nNodes / 2) {
            neighbours.push_back(2*i + 1);
            neighbours.push_back(2*i + 2);
        }
        g.push_back(neighbours);
    }
    return g;
}

std::vector< std::vector<int> > makeGrid(int width, bool torus = false) {
    std::vector< std::vector<int> > g;
    for (int i = 0; i < width * width; i++) {
        std::vector<int> neighbours;
        // Top
        if (i / width == 0) {
            if (torus) {
                neighbours.push_back(i + width * width - width);
            }
        } else {
            neighbours.push_back(i - width);
        }
        // Left
        if (i % width == 0) {
            if (torus) {
                neighbours.push_back(i + width - 1);
            }
        } else {
            neighbours.push_back(i - 1);
        }
        // Right
        if (i % width == width - 1) {
            if (torus) {
                neighbours.push_back(i - width + 1);
            }
        } else {
            neighbours.push_back(i + 1);
        }
        // Bottom
        if (i / width == width - 1) {
            if (torus) {
                neighbours.push_back(i - width * width + width);
            }
        } else {
            neighbours.push_back(i + width);
        }
        g.push_back(neighbours);
    }
    return g;
}

std::vector< std::vector<int> > makeTorus(int width) {
    return makeGrid(width, true);
}
