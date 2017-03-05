#pragma once

#include <algorithm>
#include <random>

using std::mt19937;
using std::random_shuffle;
using std::uniform_real_distribution;
using std::uniform_int_distribution;

namespace tcframe {
	 mt19937 __engine;

    void setSeed(unsigned int seed) {
        __engine.seed(seed);
    }

    int randInt(int minNum, int maxNum) {
        uniform_int_distribution<int> distr(minNum, maxNum);
        return distr(__engine);
    }

    int randInt(int maxNumEx) {
        return randInt(0, maxNumEx - 1);
    }

    long long randLongLong(long long minNum, long long maxNum) {
        uniform_int_distribution<long long> distr(minNum, maxNum);
        return distr(__engine);
    }

    long long randLongLong(long long maxNumEx) {
        return randLongLong(0, maxNumEx - 1);
    }

    
    template<typename RandomAccessIterator>
    void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        random_shuffle(first, last, [this](unsigned int idx) { return randInt(idx); });
    }


}