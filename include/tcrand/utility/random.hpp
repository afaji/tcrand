#pragma once

#include <algorithm>
#include <random>

using std::mt19937;
using std::random_shuffle;
using std::uniform_real_distribution;
using std::uniform_int_distribution;

namespace tcrand {
	 mt19937 __engine__;

    void set_seed(unsigned int seed) {
        __engine__.seed(seed);
    }

    int rand_int(int minNum, int maxNum) {
        uniform_int_distribution<int> distr(minNum, maxNum);
        return distr(__engine__);
    }

    int rand_int(int maxNumEx) {
        return rand_int(0, maxNumEx - 1);
    }

    long long rand_long(long long minNum, long long maxNum) {
        uniform_int_distribution<long long> distr(minNum, maxNum);
        return distr(__engine__);
    }

    long long rand_long(long long maxNumEx) {
        return rand_long(0, maxNumEx - 1);
    }

    double rand_double(double minNum, double maxNum) {
        uniform_real_distribution<double> distr(minNum, maxNum);
        return distr(__engine__);
    }

    double rand_double(double maxNum) {
        return rand_double(0, maxNum);
    }




    //Deprecated

    int randInt(int minNum, int maxNum) {
        uniform_int_distribution<int> distr(minNum, maxNum);
        return distr(__engine__);
    }

    int randInt(int maxNumEx) {
        return rand_int(0, maxNumEx - 1);
    }

    long long randLongLong(long long minNum, long long maxNum) {
        uniform_int_distribution<long long> distr(minNum, maxNum);
        return distr(__engine__);
    }

    long long randLongLong(long long maxNumEx) {
        return rand_long(0, maxNumEx - 1);
    }

    double randDouble(double minNum, double maxNum) {
        uniform_real_distribution<double> distr(minNum, maxNum);
        return distr(__engine__);
    }

    double randDouble(double maxNum) {
        return rand_double(0, maxNum);
    }

}