// (c) Nikolaus Howe

// Test how balanced the RNG is
void testRng(void) {
    BitGenerator bitGen = BitGenerator();

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;

    for (int i = 0; i < 1000000; i++) {
        int bit = bitGen.getBit();
        a += bit;
        if (i % 2 == 0) {
            b += bit;
        } else {
            c += bit;
        }
        if (i % 3 == 0) {
            d += bit;
        } else if (i % 3 == 1) {
            e += bit;
        } else {
            f += bit;
        }
    }

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "e = " << e << std::endl;
    std::cout << "f = " << f << std::endl;
}

// Check that a walk starting from the left
// takes around the same time as one from the right
void leftRightSanityCheck(void) {
    std::vector< std::vector<int> > fromLeft;
    std::vector< std::vector<int> > fromRight;
    std::vector<int> maxLeft;
    std::vector<int> maxRight;

    BitGenerator bitGen = BitGenerator();

    std::cout << "from left now " << std::endl;
    int total = 0;
    for (int i = 0; i < NRUNS; i++) {
        std::vector<int> j = runSequentialPathLengths(
                NNODES, bitGen, 0, false, 0, -1);
        fromLeft.push_back(j);
        int longest = *std::max_element(j.begin(), j.end());
        maxLeft.push_back(longest);
        total += longest;
    }

    printVector(maxLeft);
    int maxx = *std::max_element(maxLeft.begin(), maxLeft.end());
    int minn = *std::min_element(maxLeft.begin(), maxLeft.end());
    std::cout << "max: " << maxx << std::endl;
    std::cout << "min: " << minn << std::endl;
    std::cout << "ave: " << total / NRUNS << std::endl;
    std::cout << std::endl;

    std::cout << "from right now " << std::endl;
    total = 0;
    for (int i = 0; i < NRUNS; i++) {
        std::vector<int> j = runSequentialPathLengths(
                NNODES, bitGen, NNODES-1, false, 0, -1);
        fromRight.push_back(j);
        int longest = *std::max_element(j.begin(), j.end());
        maxRight.push_back(longest);
        total += longest;
    }

    printVector(maxRight);
    maxx = *std::max_element(maxRight.begin(), maxRight.end());
    minn = *std::min_element(maxRight.begin(), maxRight.end());
    std::cout << "max: " << maxx << std::endl;
    std::cout << "min: " << minn << std::endl;
    std::cout << "ave: " << total / NRUNS << std::endl;
    std::cout << std::endl;
}
