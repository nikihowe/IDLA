// (c) Nikolaus Howe 2018

#ifndef RANDOM_HPP
#define RANDOM_HPP

// Given a vector of full nodes,
// free up n of those nodes randomly.
void freeNRandom(std::vector<int> &isFull, int n);

// Return how long it takes on average to hit one
// of sqrt(n) points placed randomly on a 2D grid or torus
// using random walk from the top left or from a random point.
int hitSqrtRandom(int width, bool torus = false, bool random = false);

// Test to check how long it takes to hit one of sqrt(n)
// (for n = 100, 200, ... , 600) points placed randomly
// on a 2D grid or torus, either starting from the 
// top-left or starting from random locations.
void runRandomHits(void);

#endif //RANDOM_HPP
