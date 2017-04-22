#pragma once
#include "TreeRandomizer.hpp"

using namespace std;

namespace tcrand {

    template <class T>
    class StarTreeRandomizerBase: public TreeRandomizerBase<T>{
        public:
            T& depth(int d) = delete;
            T& leaf_count(int d) = delete;
            T& child_count(int d) = delete;
            T& child_count(int lo, int hi) = delete;

            StarTreeRandomizerBase(){
                TreeRandomizerBase<T>::depth(1);
            };
    };

    class StarTreeRandomizer: public StarTreeRandomizerBase<StarTreeRandomizer>{};
}