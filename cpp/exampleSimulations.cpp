// (c) Nikolaus Howe 2018

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <cmath>
#include "util.hpp"
#include "graphConstructors.hpp"
#include "standardTransformations.hpp"
#include "otherTransformations.hpp"
#include "standardIDLA.hpp"
#include "modifiedIDLA.hpp"
#include "optimizedIDLA.hpp"
#include "exampleSimulations.hpp"

#define NRUNS 100

void compareRotorRouterAndSequentialIDLA(void) {
    BitGenerator bitGen = BitGenerator();

    std::vector<int> rr;
    std::vector<int> ra;

    for (int i = 5; i < 12; i += 1) {
        std::cout << i << " " << std::endl;

        //std::vector< std::vector<int> > g = makeGrid(i, true);
        //std::vector< std::vector<int> > g = makeCycle(i);
        //std::vector< std::vector<int> > g = makeComplete(i);

        std::vector< std::vector<int> > g = makeBinTree(i);
        std::cout << g.size() << std::endl;
        //std::vector< std::vector<int> > g = makePath(i);
        //print2dVector(g);

        //int total = 0;
        //for (int k = 0; k < 100; k++) {
            //total += runRotorRouter(g, i/2, true);
        //}

        //rr.push_back(total / 100);

        std::vector<int> res = runSequentialRotorRouterLengths(g);
        int longRR = *std::max_element(res.begin(), res.end());
        std::cout << "did rr" << std::endl;
        rr.push_back(longRR);
        
        int total = 0;
        
        for (int k = 0; k < 100; k++) {
            std::vector<int> j = runSequentialLengths(g, bitGen, 0, false);
            int longest = *std::max_element(j.begin(), j.end());
            total += longest;
        }

        ra.push_back(total / 100);
    }

    printVector(rr);
    printVector(ra);
}

void printPasteLocations(void) {
    // Note that it is more likely to paste higher up, as we would expect.
    int start = 20;
    int end = 51;

    for (int n = start; n < end; n += 5) {
        std::cout << n*n << ' ';
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    for (int n = start; n < end; n += 5) { // want to see how it changes for graphs of different sizes

        std::vector< std::vector<int> > g = makeGrid(n);
        std::vector<double> aveDist(g.size(), 0.0);

        for (int i = 0; i < 100; i++) { // we'll take the average over 100 runs

            std::vector< std::vector<int> > runs = runSimpleGraph(g);
            
            // Choose whichiver transformation you want here
            std::vector<int> dist = sequentialToParallelDistribution2(runs);
            
            //std::sort(dist.begin(), dist.end(), greaterThan);

            // Record the distribution
            for (int j = 0; j < g.size(); j++) {
                aveDist[j] += dist[j];
            }
        }

        // Normalize the numbers
        for (int j = 0; j < g.size(); j++) {
            aveDist[j] = aveDist[j] / 100.;
        }

        printDoubleVector(aveDist);
    }
}

void checkTransformation(void) {
    std::vector< std::vector<int > > g;
   
    std::vector<int> a;
    a.push_back(0);
    g.push_back(a);

    std::vector<int> b;
    b.push_back(0);
    b.push_back(4);
    g.push_back(b);

    std::vector<int> c;
    c.push_back(0);
    c.push_back(4);
    c.push_back(3);
    g.push_back(c);

    std::vector<int> d;
    d.push_back(0);
    d.push_back(4);
    d.push_back(1);
    g.push_back(d);

    std::vector<int> e;
    e.push_back(0);
    e.push_back(1);
    e.push_back(3);
    e.push_back(0);
    e.push_back(3);
    e.push_back(2);
    g.push_back(e);

    std::vector<int> f;
    f.push_back(0);
    f.push_back(3);
    f.push_back(0);
    f.push_back(1);
    f.push_back(5);
    g.push_back(f);

    // Below, choose whichever transformation you want
    std::vector< std::vector<int> > one = sequentialToParallel3(g);
    print2dVector(g);
    std::cout << std::endl;
    print2dVector(one);
}

void checkIfInverses(void) {
    for (int i = 0; ; i++) {
        std::vector< std::vector<int> > g = makeComplete(8);
        std::vector< std::vector<int> > pl = runSimpleGraph(g);
        std::vector< std::vector<int> > one = sequentialToParallel2(pl);
        std::vector< std::vector<int> > original = parallelToSequential2(one);
        if (pl != original) {
            std::cout << "error" << std::endl;
            print2dVector(pl);
            print2dVector(one);
            print2dVector(original);
            break;
        }
    }
}

void examinePostPasteWalkLengths(void) {
    // Output graph size
    for (int width = 10; width < 51; width += 10) {
        std::cout << width * width << " " << std::flush;
    }
    std::cout << "$"; // separates sizes from the lengths themselves
                      // (use for parsing with numpy)

    // Output path lengths
    for (int width = 10; width < 51; width += 10) {
        int numNodes = width * width;
        std::vector< std::vector<int> > g = makeGrid(width);

        std::vector<double> par1Lengths(numNodes, 0);
        std::vector<double> par2Lengths(numNodes, 0);

        for (int i = 0; i < 100; i++) {
            std::vector< std::vector<int> > paths = runSimpleGraph(g);
            std::vector< std::vector<int> > one = sequentialToParallel(paths);
            std::vector< std::vector<int> > two = sequentialToParallel2(paths);

            for (int j = 0; j < numNodes; j++) {
                par1Lengths[j] += one[j].size();
                par2Lengths[j] += two[j].size();
            }
        }

        for (int j = 0; j < numNodes; j++) {
            par1Lengths[j] /= 100.0;
            par2Lengths[j] /= 100.0;
        }

        printDoubleVector(par1Lengths);
        printDoubleVector(par2Lengths);
        std::cout << std::endl;
    }
}

void exploreOneStepLookahead(void) {
    for (int width = 10; width < 61; width += 10) {
        std::cout << width * width << " ";
        int totalNormal = 0;
        int totalSneaky = 0;
        for (int i = 0; i < 100; i++) {
        std::vector< std::vector<int> > g = makeGrid(width);
        
        int run = runSimpleGraphGetMax(g);
        std::vector<int> sneaky = runOneStepLookaheadSequentialLength(g);

        totalNormal += run;
        totalSneaky += *std::max_element(sneaky.begin(), sneaky.end());
        }
        std::cout << totalNormal / 100.0 << " ";
        std::cout << totalSneaky / 100.0 << std::endl;
    }
}

void exploreMomentumLandscape(void) {
    int nRuns = 500;
    for (int width = 10; width < 31; width += 2) {
        std::vector< std::vector<int> > g = makeGrid(width);
        double best = 0;
        double bestVal = INT_MAX;
        for (double momentum = 0.4; momentum < 1.0; momentum += 0.05) {
            int ave = 0;
            for (int i = 0; i < nRuns; i++) {
                std::vector< std::vector<int> > paths = runMomentumSequential(g, momentum);
                int maxLen = 0;
                for (int j = 0; j < g.size(); j++) {
                    int pathLen = paths[j].size();
                    if (pathLen > maxLen) {
                        maxLen = pathLen;
                    }
                }
                ave += maxLen;
            }
            if (ave < bestVal) {
                bestVal = ave;
                best = momentum;
            }
            //std::cout << momentum << ": " << ave * 1.0 / 100 << std::endl;
        }
        std::cout << g.size();
        std::cout << ": " << best << std::endl;
    }
}

void compareMomentumAndNormal(void) {
    // Compare momentum-based IDLA (m=0.9) to normal IDLA
    std::vector<double> momentum;
    std::vector<double> normal;
    double m = 0.9;
    for (int width = 10; width < 51; width += 5) {

        std::vector< std::vector<int> > g = makeGrid(width);

        // Run momentum
        int ave = 0;
        for (int i = 0; i < 100; i++) {
            std::vector< std::vector<int> > paths = runMomentumSequential(g, m);
            int maxLen = 0;
            for (int j = 0; j < g.size(); j++) {
                int pathLen = paths[j].size();
                if (pathLen > maxLen) {
                    maxLen = pathLen;
                }
            }
            ave += maxLen;
        }
        momentum.push_back(ave * 1.0 / 100);

        // Run normal
        ave = 0;
        for (int i = 0; i < 100; i++) {
            ave += runSimpleGraphGetMax(g);
        }
        normal.push_back(ave * 1.0 / 100);
    }

    std::cout << "normal" << std::endl;
    printDoubleVector(normal);

    std::cout << "momentum 0.9" << std::endl;
    printDoubleVector(momentum);
}

void runVariousPaths(void) {

    // Compare starting from 0, starting from middle, and starting from random location
    std::vector<int> maxTimesZero;
    std::vector<int> maxTimesNOver2;
    std::vector<int> maxTimesRandom;
    std::vector<int> maxTimesMinStep;
    std::vector<int> maxTimesFirstReach;
    std::vector<int> maxTimesMinStep2;
    int countZero = 0;
    int countNOver2 = 0;
    int countRandom = 0;
    int countMinStep = 0;
    int countFirstReach = 0;
    int countMinStep2 = 0;

    BitGenerator bitGen = BitGenerator();
    int minSize = 300;
    int maxSize = 801;

    for (int nNodes = minSize; nNodes < maxSize; nNodes += 100) {
        std::cout << "nNodes: " << nNodes << std::endl;

        for (int i = 0; i < NRUNS; i++) {
            if (i % 10 == 0) std::cout << i << std::endl;
            std::vector<int> z = runSequentialPathLengths(nNodes, bitGen, 0, false);
            std::vector<int> n = runSequentialPathLengths(nNodes, bitGen, nNodes/2, false);
            std::vector<int> r = runSequentialPathLengths(nNodes, bitGen, 0, true);
            std::vector<int> m = runSequentialPathLengths(nNodes, bitGen, 0, false, 10 * nNodes);
            std::vector<int> f = runSequentialPathLengths(nNodes, bitGen, 0, false, 0, nNodes/2);
            std::vector<int> m2 = runSequentialPathLengths(nNodes, bitGen, 0, false, 0.25 * nNodes * nNodes);
            int longestz = *std::max_element(z.begin(), z.end());
            int longestn = *std::max_element(n.begin(), n.end());
            int longestr = *std::max_element(r.begin(), r.end());
            int longestm = *std::max_element(m.begin(), m.end()); // forget the waiting time
            int longestf = *std::max_element(f.begin(), f.end());
            int longestm2 = *std::max_element(m2.begin(), m2.end());
            countZero += longestz;
            countNOver2 += longestn;
            countRandom += longestr;
            countMinStep += longestm;
            countFirstReach += longestf;
            countMinStep2 += longestm2;
        }
        //std::cout << "Number of Nodes " << nNodes << std::endl;
        //std::cout << "Average max walk length for 0 start: " << maxTimesZero / NRUNS << std::endl;
        //std::cout << "Average max walk length for n/2 start: " << maxTimesNOver2 / NRUNS << std::endl;
        //std::cout << "Average max walk length for random start: " << maxTimesRandom / NRUNS << std::endl;
        maxTimesZero.push_back(countZero / NRUNS);
        maxTimesNOver2.push_back(countNOver2 / NRUNS);
        maxTimesRandom.push_back(countRandom / NRUNS);
        maxTimesMinStep.push_back(countMinStep / NRUNS);
        maxTimesFirstReach.push_back(countFirstReach / NRUNS);
        maxTimesMinStep2.push_back(countMinStep2 / NRUNS);
        std::cout << "for nodes = " << minSize << ", ... ," << maxSize << std::endl;
        std::cout << "starting at 0" << std::endl;
        printVector(maxTimesZero);
        std::cout << "starting at n/2" << std::endl;
        printVector(maxTimesNOver2);
        std::cout << "starting at random" << std::endl;
        printVector(maxTimesRandom);
        std::cout << "starting at 0, first step 10*n" << std::endl;
        printVector(maxTimesMinStep);
        std::cout << "starting at 0, first reach n/2" << std::endl;
        printVector(maxTimesFirstReach);
        std::cout << "starting at 0, first step 0.25*n*n" << std::endl;
        printVector(maxTimesMinStep2);
    }
    std::cout << "for nodes = " << minSize << ", ... ," << maxSize << std::endl;
    std::cout << "starting at 0" << std::endl;
    printVector(maxTimesZero);
    std::cout << "starting at n/2" << std::endl;
    printVector(maxTimesNOver2);
    std::cout << "starting at random" << std::endl;
    printVector(maxTimesRandom);
    std::cout << "starting at 0, first step 10*n" << std::endl;
    printVector(maxTimesMinStep);
    std::cout << "starting at 0, first reach n/2" << std::endl;
    printVector(maxTimesFirstReach);
    std::cout << "starting at 0, first step 0.25*n*n" << std::endl;
    printVector(maxTimesMinStep2);
}

void comparePathAndCycle(void) {
    std::vector<int> seqResults;
    BitGenerator bitGen = BitGenerator();
    int total = 0;
    int length = 100;
    int nRuns = 100;
    
    for (int i = 0; i < nRuns; i++) {
        std::vector<int> j = runSequentialPathLengths(length, bitGen);
        int longest = *std::max_element(j.begin(), j.end());
        seqResults.push_back(longest);
        total += longest;
        //std::cout << bitGen.getBit() << std::endl;
    }
    //printVector(seqResults);
    int maxx = *std::max_element(seqResults.begin(), seqResults.end());
    int minn = *std::min_element(seqResults.begin(), seqResults.end());

    std::cout << "path times" << std::endl;
    std::cout << "max: " << maxx << std::endl;
    std::cout << "min: " << minn << std::endl;
    std::cout << "ave: " << total / nRuns << std::endl;
    std::cout << std::endl;

    std::vector<int> cycResults;
    BitGenerator cycBitGen = BitGenerator();
    total = 0;
    
    for (int i = 0; i < nRuns; i++) {
        std::vector<int> j = runSequentialCycleLengths(length, cycBitGen);
        int longest = *std::max_element(j.begin(), j.end());
        cycResults.push_back(longest);
        total += longest;
        //std::cout << bitGen.getBit() << std::endl;
    }
    //printVector(seqResults);
    maxx = *std::max_element(cycResults.begin(), cycResults.end());
    minn = *std::min_element(cycResults.begin(), cycResults.end());

    std::cout << "cycle times" << std::endl;
    std::cout << "max: " << maxx << std::endl;
    std::cout << "min: " << minn << std::endl;
    std::cout << "ave: " << total / nRuns << std::endl;
}

void runComplete(int numNodes) {
    std::vector< std::vector<int> > g;
    for (int i = 0; i < numNodes; i++) {
        std::vector<int> neighbours;
        for (int j = 0; j < numNodes; j++) {
            if (i != j) {
                neighbours.push_back(j);
            }
        }
        g.push_back(neighbours);
    }
    std::cout << "complete graph" << std::endl;

    BitGenerator bitGen = BitGenerator();
    std::vector<int> seqLen;
    
    for (int i = 0; i < NRUNS; i++) {
        std::vector<int> j = runSequentialLengths(g, bitGen);
        int longest = *std::max_element(j.begin(), j.end());
        seqLen.push_back(longest);
    }
    
    printVector(seqLen); 
}

void runGrids(void) {
    std::vector<int> averagesCorner;
    std::vector<int> averagesMiddle;
    std::vector<int> averagesCornerWaitN;
    std::vector<int> averagesMiddleWaitN;
    std::vector<int> averagesRandom;
    bool torus = true;
    BitGenerator bg = BitGenerator();
    for (int i = 31; i < 70; i += 4) {
        std::cout << i*i << std::endl;

        std::vector< std::vector<int> > g = makeGrid(i, torus);

        int totalCorner = 0;
        int totalMiddle = 0;
        int totalCornerWaitN = 0;
        int totalMiddleWaitN = 0;
        int totalRandom = 0;
        //std::vector<int> lengths;
        int nRuns = 100;
        for (int j = 0; j < nRuns; j++) {
            if (j % 10 == 0) std::cout << j << std::endl;
            totalCorner += runSequentialGetMax(g, bg);
            totalMiddle += runSequentialGetMax(g, bg, g.size() / 2);
            totalCornerWaitN += runSequentialGetMax(g, bg, 0, false, g.size());
            totalMiddleWaitN += runSequentialGetMax(g, bg, g.size() / 2, false, g.size());
            totalRandom += runSequentialGetMax(g, bg, 0, true);
            //lengths.push_back(runGrid(i));
        }

        int averageCorner = totalCorner / nRuns;
        int averageMiddle = totalMiddle / nRuns;
        int averageCornerWaitN = totalCornerWaitN / nRuns;
        int averageMiddleWaitN = totalMiddleWaitN / nRuns;
        int averageRandom = totalRandom / nRuns;
        averagesCorner.push_back(averageCorner);
        averagesMiddle.push_back(averageMiddle);
        averagesCornerWaitN.push_back(averageCornerWaitN);
        averagesMiddleWaitN.push_back(averageMiddleWaitN);
        averagesRandom.push_back(averageRandom);
        std::cout << "torus: " << torus << std::endl;
        std::cout << "grid averages are" << std::endl;
        std::cout << "top left" << std::endl;
        printVector(averagesCorner); 
        std::cout << "middle" << std::endl;
        printVector(averagesMiddle); 
        std::cout << "top left, wait n" << std::endl;
        printVector(averagesCornerWaitN); 
        std::cout << "middle, wait n" << std::endl;
        printVector(averagesMiddleWaitN); 
        std::cout << "random" << std::endl;
        printVector(averagesRandom); 
    }
    std::cout << "for ";
    for (int i = 31; i < 70; i +=4) {
        std::cout << i*i << ", ";
    }
    std::cout << std::endl;
    std::cout << "torus: " << torus << std::endl;
    std::cout << "grid averages are" << std::endl;
    std::cout << "top left" << std::endl;
    printVector(averagesCorner); 
    std::cout << "middle" << std::endl;
    printVector(averagesMiddle); 
    std::cout << "top left, wait n" << std::endl;
    printVector(averagesCornerWaitN); 
    std::cout << "middle, wait n" << std::endl;
    printVector(averagesMiddleWaitN); 
    std::cout << "random" << std::endl;
    printVector(averagesRandom); 
}

void runBinTrees(void) {
    std::vector<int> averagesRoot;
    std::vector<int> averagesLeaf;
    std::vector<int> averagesRootWaitN;
    std::vector<int> averagesLeafWaitN;
    std::vector<int> averagesRandom;
    BitGenerator bg = BitGenerator();
    for (int i = 7; i < 8; i++) {
        int nNodes = (1 << i) - 1; // i < pow(2, depth)
        std::cout << nNodes << std::endl;

        std::vector< std::vector<int> > g = makeBinTree(i);

        int totalRoot = 0;
        int totalLeaf = 0;
        int totalRootWaitN = 0;
        int totalLeafWaitN = 0;
        int totalRandom = 0;
        //std::vector<int> lengths;
        int nRuns = 100;
        for (int j = 0; j < nRuns; j++) {
            if (j % 10 == 0) std::cout << j << std::endl;
            totalRoot += runSequentialGetMax(g, bg);
            totalLeaf += runSequentialGetMax(g, bg, g.size() / 2);
            totalRootWaitN += runSequentialGetMax(g, bg, 0, false, g.size());
            totalLeafWaitN += runSequentialGetMax(g, bg, g.size() / 2, false, g.size());
            totalRandom += runSequentialGetMax(g, bg, 0, true);
            //lengths.push_back(runGrid(i));
        }

        int averageRoot = totalRoot / nRuns;
        int averageLeaf = totalLeaf / nRuns;
        int averageRootWaitN = totalRootWaitN / nRuns;
        int averageLeafWaitN = totalLeafWaitN / nRuns;
        int averageRandom = totalRandom / nRuns;
        averagesRoot.push_back(averageRoot);
        averagesLeaf.push_back(averageLeaf);
        averagesRootWaitN.push_back(averageRootWaitN);
        averagesLeafWaitN.push_back(averageLeafWaitN);
        averagesRandom.push_back(averageRandom);
    }
    std::cout << "for ";
    for (int i = 7; i < 12; i++) {
        std::cout << (1 << i) - 1 << ", ";
    }
    std::cout << std::endl;
    std::cout << "binTree averages are" << std::endl;
    std::cout << "root" << std::endl;
    printVector(averagesRoot); 
    std::cout << "leaf" << std::endl;
    printVector(averagesLeaf); 
    std::cout << "root, wait n" << std::endl;
    printVector(averagesRootWaitN); 
    std::cout << "leaf, wait n" << std::endl;
    printVector(averagesLeafWaitN); 
    std::cout << "random" << std::endl;
    printVector(averagesRandom); 
}

void fillFromStratified(void) {
    std::vector< std::vector<int> > g = makeGrid(60);
    std::cout << "made grid" << std::endl;
    std::vector<int> locs = generateFastLocations(60);
    std::cout << "made it here" << std::endl;
    int total = 0;
    int totalr = 0;

    BitGenerator bitGen = BitGenerator();

    for (int i = 0; i < 100; i++) {
        std::vector<int> lengths = fillFromPoints(g, locs, bitGen);
        int longest = *std::max_element(lengths.begin(), lengths.end());
        total += longest;
    }

    for (int i = 0; i < 100; i++) {
        std::vector<int> lengths = runSequentialLengths(g, bitGen, 0, true);
        int longest = *std::max_element(lengths.begin(), lengths.end());
        totalr += longest;
    }

    std::cout << "average from stratified points " << total / 100 << '\n';
    std::cout << "average from random " << totalr / 100 << '\n';
}
