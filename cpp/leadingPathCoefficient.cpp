// (c) Nikolaus Howe 2018

#include <iostream>
#include <cstdlib>
#include <cmath>

// Estimate the leading constant for the path,
// using the expected value formula
// Estimate the leading constant for the path,
// by running simulations and averaging

double prtb(int b, int t) {
    double sum = 0;

    for (int k = 0; k < b; k++) {

        double sine = sin((2.0*k+1)*M_PI / (2.0*b));
        double topcos = (1 - pow(cos((2.0*k+1)*M_PI / (2.0*b)), t-1));
        double botcos = (1 - cos((2.0*k+1)*M_PI / (2.0*b)));
        //std::cout << "sine   " << sine << std::endl;
        //std::cout << "topcos " << topcos << std::endl;
        //std::cout << "botcos " << botcos << std::endl;

        double addend = sine * topcos / botcos;

        if (k & 1) {
            addend *= -1;
        }
        //std::cout << "addend " << addend << std::endl;
        sum += addend;
    }

    //std::cout << "sum: " << sum / b << std::endl;
    return sum / b;
}

double prtseq(int t, int n) {
    double product = 1;

    for (int b = floor(n / log(n)); b <= n; b++) {
        //std::cout << "multiplying by " << prtb(b, t) << std::endl;
        product *= prtb(b, t);
        //std::cout << "product " << product << std::endl;
        if (product < 10e-20) {
            break;
        }
    }

    //std::cout << "Pr[tseq >= " << t << "] = " << 1.0 - product << std::endl;
    return 1.0 - product;
}

double etseq(int n) {
    double sum = 0;
    for (size_t t = n; t < n * n * sqrt(n); t++) { // n^3 seems a reasonable upper bound to how far we need to look
        //if (t % 1000 == 0) std::cout << t << std::endl;
        double toAdd = prtseq(t, n);
        //if (toAdd < 10e-9) { // break if the prob. contributions are super small
            //std::cout << "breaking" << std::endl;
            //break;
        //}
        sum += toAdd;
    }
    return sum;
}

int main(void) {

    // Estimate the leading constant for the path,
    // using the expected value formula
    for (int n = 100; n < 1000; n += 200) {
        std::cout << "n = " << n << std::endl;
        std::cout << etseq(n) / (n * n * log(n)) << std::endl;
    } 

    // Estimate the leading constant for the path,
    // by running simulations and averaging
    BitGenerator bitGen = BitGenerator();

    for (int i = 100; i < 201; i += 10) {
        int nn = (i-1)*(i-1) + 1;
        unsigned long total = 0;
        for (int j = 0; j < 100; j++) {
            std::vector<int> lengths = runFastSequentialPathLengths(i, bitGen);
            unsigned long longest = *std::max_element(lengths.begin(), lengths.end());
            total += longest;
        }
        double ave = total * 1.0 / 100;

        double constant = ave / (nn * nn * log(nn));

        std::cout << "n = " << nn << " : " << constant << std::endl;
    
    }
}
