#pragma once

#include <algorithm>
#include <random>

using std::mt19937;
using std::random_shuffle;
using std::uniform_real_distribution;
using std::uniform_int_distribution;

namespace tcrand {
	 mt19937 __engine__;

    void setSeed(unsigned int seed) {
        __engine__.seed(seed);
    }

    int randInt(int minNum, int maxNum) {
        uniform_int_distribution<int> distr(minNum, maxNum);
        return distr(__engine__);
    }

    int randInt(int maxNumEx) {
        return randInt(0, maxNumEx - 1);
    }

    long long randLongLong(long long minNum, long long maxNum) {
        uniform_int_distribution<long long> distr(minNum, maxNum);
        return distr(__engine__);
    }

    long long randLongLong(long long maxNumEx) {
        return randLongLong(0, maxNumEx - 1);
    }

    double randDouble(double minNum, double maxNum) {
        uniform_real_distribution<double> distr(minNum, maxNum);
        return distr(__engine__);
    }

    double randDouble(double maxNum) {
        return randDouble(0, maxNum);
    }
}