#include <iostream>
#include <random>
#include <chrono>

int main()
{
    std::mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(timeSeed);
    // initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> unif(0, 1);
    double currentRandomNumber = unif(rng);
    return 0;
}
