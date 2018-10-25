
    /*
    std::vector<int> seqResults;
    std::vector<int> parResults;

    BitGenerator bitGen = BitGenerator();

    int totalCopy = 0;
    int totalRunP = 0;

    int totalNumMaxChanges = 0;
    //std::cout << "path of size " << NNODES << std::endl;

    for (int n = 2; n < 100; n += 8) {

        int totalMaxLength = 0;
        for (int i = 0; i < NRUNS; i++) {
        
        //vecvecint j = runSequentialPaths(NNODES, bitGen); 
        //totalNumMaxChanges += sequentialToParallelCountMaxChange(j);

            //std::vector<int> j = runFastSequentialPathLengths(n, bitGen);
            //int longest = *std::max_element(j.begin(), j.end());
            //totalMaxLength += longest;
        }

        //std::cout << "ave max length for " << (n-1)*(n-1) + 1 << " nodes is " << totalMaxLength * 1.0 / NRUNS << std::endl;
        
        //if (i % 10 == 0) std::cout << i << std::endl;
        //std::vector<int> j = runSequentialPathLengths(NNODES, bitGen);
        //int longest = *std::max_element(j.begin(), j.end());
        //printVector(j);
        //seqResults.push_back(longest);
       
        //high_resolution_clock::time_point t1 = high_resolution_clock::now();
        //vecvecint seq1 = runSequentialPaths(NNODES, bitGen);
        //vecvecint par1 = sequentialToParallel(seq1);
        //vecint seq1 = runSequentialPathLengths((NNODES - 1) * (NNODES - 1) + 1, bitGen);
        //high_resolution_clock::time_point t2 = high_resolution_clock::now();

        //auto duration = duration_cast<microseconds>( t2 - t1 ).count();
        //totalCopy += duration;
        //std::cout << "copying lists" << duration << std::endl;

        //t1 = high_resolution_clock::now();
        //vecvecint seq2 = runSequentialPaths(NNODES, bitGen);
        //vecint seq2 = runFastSequentialPathLengths(NNODES, bitGen);
        //vecvecint par2 = runParallelPaths(NNODES, bitGen);
        //vecint par2 = runParallelPathLengths(NNODES, bitGen);
        //t2 = high_resolution_clock::now();

        //duration = duration_cast<microseconds>( t2 - t1 ).count();
        //totalRunP += duration;
        //std::cout << "running parallel" << duration << std::endl;

        //vecint k = runParallelPathLengths(NNODES, bitGen);
        //print2dVector(j);
        //std::vector< std::vector<int> > k = sequentialToParallel(j);
        //print2dVector(k);
        //int longestSeq = std::max_element(j.begin(), j.end(), Longest)->size();
        //seqResults.push_back(longestSeq);

        //int longestPar = std::max_element(k.begin(), k.end(), Longest)->size();
        //longest = *std::max_element(k.begin(), k.end());
        //parResults.push_back(longest);

        //std::cout << "\n" << std::endl;

    }
    //std::cout << "for length " << NNODES
        //<< " the ave # of changes in max is "
        //<< totalNumMaxChanges * 1.0 / NRUNS << std::endl;
    //std::cout << "ave copy " << totalCopy / NRUNS << std::endl;
    //std::cout << "ave runp " << totalRunP / NRUNS << std::endl;
    //printVector(seqResults);
    //std::cout << std::endl;
    //printVector(parResults);

    //std::cout << std::endl;

    //printVector(parResults);

    std::vector<int> a;
    a.push_back(0);
    std::vector<int> b;
    b.push_back(0);
    b.push_back(1);
    std::vector<int> c;
    c.push_back(0);
    c.push_back(1);
    c.push_back(2);
    std::vector<int> d;
    d.push_back(0);
    d.push_back(2);
    d.push_back(1);
    d.push_back(3);

    std::vector< std::vector<int> > j;

    j.push_back(a);
    j.push_back(b);
    j.push_back(c);
    j.push_back(d);

    std::vector< std::vector<int> > k = sequentialToParallel(j);

    std::vector<int> a;
    a.push_back(10);
    a.push_back(20);
    a.push_back(30);

    std::vector<int> b;
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    printVector(a);
    printVector(b);

    copyPaste(0, a, b);

    printVector(a);
    printVector(b);

    std::cout << a.size() << std::endl;
    std::cout << b.size() << std::endl;
    */
