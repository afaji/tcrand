#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include "../tree.hpp"
#define DEBUG false
using namespace std;

namespace tcrand {

	class Tree{
		vector<int> _path_from;
		vector<int> _path_to;
		int _root;
		vector<int> _parents;
		int _node_count;
		
		template< class T >
		void reorder(vector<T> &v, vector<int> const &order )  {   
		    for ( int s = 1, d; s < order.size(); ++ s ) {
		        for ( d = order[s]; d < s; d = order[d] ) ;
		        if ( d == s ) while ( d = order[d], d != s ) swap( v[s], v[d] );
		    }
		}
		
		Tree& shuffleDirection(){
			int N = _path_to.size();
			for (int i=0;i<N;i++){
				if (rand_int(100) % 2)
					swap(_path_to[i], _path_from[i]);
			}
			return *this;
		}

		Tree& shuffleOrder(){
			int N = _path_to.size();
			vector<int> indexes;
			for (int i=0;i<N;i++)
				indexes.push_back(i);
			random_shuffle(indexes.begin(), indexes.end());
			reorder(_path_to , indexes );
			reorder(_path_from, indexes );
			return *this;
		}

	public:
		Tree(int root, vector<int> p_from, vector<int> p_to, vector<int> parents){
			_path_from = p_from;
			_path_to = p_to;
			_parents = parents;
			_root = root;
			_node_count = p_from.size() + 1;
			shuffleOrder();
			shuffleDirection();
		}

		pair<vector<int>,vector<int> > edges(){
			return make_pair(_path_from, _path_to);
		}
		
		int root(){
			return _root;
		}

		int node_count(){
			return _node_count;
		}

		vector<int> parents(){
			return _parents;
		}

	};

}