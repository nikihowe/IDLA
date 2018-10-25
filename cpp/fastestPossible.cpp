// (c) Nikolaus Howe 2018

#include <iostream>
#include <cstdlib>
#include <time.h>

// An optimized version (but not using any approximations)
// of the standard random walk on the path,
// tarting from the leftmost node.

#define NNODES 100
#define NRUNS  100

int doWork();
int getMax(int *a);
int getMin(int *a);

int main() {
    clock_t start = clock();
    int lengths[NRUNS];
    int sum = 0;
    for (int i = 0; i < NRUNS; i++) {
        if (i % 10 == 0) std::cout << i << std::endl;
        int res = doWork();
        lengths[i] = res;
        sum += res;
    }

    clock_t end = clock();

    std::cout << "max " << getMax(lengths) << std::endl;
    std::cout << "min " << getMin(lengths) << std::endl;
    std::cout << "ave " << sum / NRUNS << std::endl;
    std::cout << "runtime " << (double) (end-start) / CLOCKS_PER_SEC << std::endl;
}

int doWork() {

    int max_len = 0;
    int max_filled = -1;
    
    // Run NNODES walks on the path
    for (int i = 0; i < NNODES; i++) {
        int loc = 0;
        int length = 0;
        while (loc <= max_filled) {
            if (loc == 0) {
                loc++;
            } else {
                //std::srand(std::time(nullptr));
                double rand = std::rand();
                //std::cout << rand << std::endl;
                if (rand > RAND_MAX / 2) {
                    loc++;
                } else {
                    loc--;
                }
            }
            length++;
        }
        max_filled = loc;
        if (length > max_len) {
            max_len = length;
        }
    }
    return max_len;
}

int getMax(int *a) {
    int max = 0;
    for (int i = 0; i < NRUNS; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int getMin(int *a) {
    int min = 10000000;
    for (int i = 0; i < NRUNS; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}
