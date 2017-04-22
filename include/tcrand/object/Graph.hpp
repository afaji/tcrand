#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#define DEBUG false
using namespace std;

namespace tcrand {

    class Graph{
    protected:
        vector<int> _path_from;
        vector<int> _path_to;
        vector<int> _nodes;
        int num_edges;
        int num_nodes;
        int _base;
        
        template< class T >
        void reorder(vector<T> &v, vector<int> const &order )  {   
            for ( int s = 1, d; s < order.size(); ++ s ) {
                for ( d = order[s]; d < s; d = order[d] ) ;
                if ( d == s ) while ( d = order[d], d != s ) swap( v[s], v[d] );
            }
        }

        Graph& shuffleOrder(){
            int N = _path_to.size();
            vector<int> indexes;
            for (int i=0;i<N;i++)
                indexes.push_back(i);
            random_shuffle(indexes.begin(), indexes.end());
            reorder(_path_to , indexes );
            reorder(_path_from, indexes );
            return *this;
        }

        void init_graph(vector<int> nodes, vector<int> p_from, vector<int> p_to, int base = 0){
            _path_from = p_from;
            _path_to = p_to;
            num_edges = p_from.size();
            num_nodes = nodes.size();
            _nodes = nodes;
            _base = base;
            shuffleOrder();
        }

    public:
        Graph(){}
        Graph(vector<int> nodes, vector<int> p_from, vector<int> p_to, int base = 0){
            init_graph(nodes, p_from, p_to, base);
        }
        
        pair<vector<int>,vector<int> > edges(){
            return make_pair(_path_from, _path_to);
        }

        vector<int> nodes(){
            return _nodes;
        }
        
        int node_count(){
            return num_nodes;
        }

        int edge_count(){
            return num_edges;
        }

        int index_base(){
            return _base;
        }
    };

}