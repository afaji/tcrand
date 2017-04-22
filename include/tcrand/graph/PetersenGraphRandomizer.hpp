#pragma once
#include "GraphRandomizer.hpp"

using namespace std;

namespace tcrand {

    template <class T>
    class PetersenGraphRandomizerBase: public GraphRandomizerBase<T>{
        public:
            T& component_count(int n) = delete;
            T& node_count(int n) = delete;
            T& edge_count(int n) = delete;
            T& self_loop() = delete;
            T& multi_path(int m = 1000000) = delete;
            T& bridge_count(int m) = delete;

            Graph next(){
                GraphRandomizerBase<T>::node_count(10);
                GraphRandomizerBase<T>::edge_count(15);


                map<pair<int,int>, int> path;
                path[make_pair(0,1)] = 1;
                path[make_pair(1,2)] = 1;
                path[make_pair(2,3)] = 1;
                path[make_pair(3,4)] = 1;
                path[make_pair(4,0)] = 1;

                path[make_pair(0,5)] = 1;
                path[make_pair(1,6)] = 1;
                path[make_pair(2,7)] = 1;
                path[make_pair(3,8)] = 1;
                path[make_pair(4,9)] = 1;
                
                path[make_pair(5,7)] = 1;
                path[make_pair(6,8)] = 1;
                path[make_pair(7,9)] = 1;
                path[make_pair(8,5)] = 1;
                path[make_pair(9,6)] = 1;
                
                return GraphRandomizerBase<T>::load_graph(path);
            };
    };

    class PetersenGraphRandomizer: public PetersenGraphRandomizerBase<PetersenGraphRandomizer>{};
}