// (c) Nikolaus Howe 2018

#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>

void printVector(std::vector<int> path);
void printDoubleVector(std::vector<double> path);
void printArray(int *path, int length);
void print2dVector(std::vector< std::vector<int> > path);
void printVectorToDim(std::vector<int> path, int d);

// Return the longer of the two vectors
inline bool Longest(const std::vector<int> &A, const std::vector<int> &B)
{
   return A.size() < B.size();
}

// Return the positive version of i % n
inline int positiveModulo(int i, int n) {
    return (i % n + n) % n;
}

// The ">" in function form
inline bool greaterThan(int i, int j) {
    return i > j;
}

// Swap two integers in-place
inline void swap(int &i, int &j) {
    int temp = i;
    i = j;
    j = temp;
}

// This object is used to generate unsigned integers,
// doubles, and individual bits, from a uniform
// random distribution, as quickly as possiblee
class BitGenerator {
    private:
        // The underlying RNG
        std::mt19937 e;
        // Get a random int
        int randNum;
        // Records the number of times we've read
        // a bit from the same random number
        int numReads;

    public:
        BitGenerator(); 
        // Get a random unsigned int between 0 and max()
        uint32_t getNum();
        // Get a random bit
        unsigned int getBit();
        // The max value possibly returned by getNum()
        uint32_t max();
        // Get a random double between 0 and 1
        double getDouble();
};

// Choose randomly among the free neighbours. If there are
// no free neighbours, choose randomly among all neighbours.
int chooseBestNeighbour(std::vector< std::vector<int> > &g,
        std::vector<int> &isFull, BitGenerator &bg,
        int node);

// If the conditions allow, with probability='momentum' choose
// to continue in the same direction as before. Otherwise,
// choose randomly between neighbours.
int momentumChoice(
        std::vector< std::vector<int> > &g,
        std::vector<int> &isFull,
        BitGenerator &bitGen,
        int current,
        int lastDirection,
        double momentum);

// An object which holds "location" information
// in an IDLA process. Basically it stores the
// particle (which "row" of the process) and the
// step (which "column" of the process) we're
// talking about. It can be initialized to 
// a specific place, or (by default) 
// to INT_MAX values
struct ParticleAndStep {
    int particle;
    int step;
    ParticleAndStep(int p, int s): particle(p), step(s) {}
    ParticleAndStep(): particle(INT_MAX), step(INT_MAX) {}
};

// Check if two particleAndStep objects are equal
inline bool operator==(const ParticleAndStep &one, const ParticleAndStep &two) {
    return one.step == two.step && one.particle == two.particle;
}

// Check if two particleAndStep objects are not equal
inline bool operator!=(const ParticleAndStep &one, const ParticleAndStep &two) {
    return !(one == two);
}

// "sequentially less than"
inline bool seqLT(ParticleAndStep one, ParticleAndStep two) {
    return one.particle < two.particle || 
            (one.particle == two.particle && one.step < two.step);
}

// "parallel-y less than"
inline bool parLT(ParticleAndStep one, ParticleAndStep two) {
    return one.step < two.step ||
        (one.step == two.step && one.particle < two.particle);
}

// Cut and paste the end of one vector onto the end of another,
// starting from index j to the end.
void copyPaste(int j, std::vector<int> &appendFrom,
        std::vector<int> &appendTo);
#endif // UTIL_HPP
