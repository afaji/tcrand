#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include "tree.hpp"
#define DEBUG false
using namespace std;

namespace tcrand {

	class Tree{
		vector<int> _path_from;
		vector<int> _path_to;
		
		template< class T >
		void reorder(vector<T> &v, vector<int> const &order )  {   
		    for ( int s = 1, d; s < order.size(); ++ s ) {
		        for ( d = order[s]; d < s; d = order[d] ) ;
		        if ( d == s ) while ( d = order[d], d != s ) swap( v[s], v[d] );
		    }
		}

	public:
		tuple<vector<int>,vector<int> > path(){
			return make_tuple(_path_from, _path_to);
		}

		vector<int> pathFrom(){
			return _path_from;
		}

		void pathFrom(vector<int> p){
			_path_from = p;
		}

		vector<int> pathTo(){
			return _path_to;
		}

		void pathTo(vector<int> p){
			_path_to = p;
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

		Tree& shuffleDirection(){
			int N = _path_to.size();
			for (int i=0;i<N;i++){
				if (randInt(100) % 2)
					swap(_path_to[i], _path_from[i]);
			}
			return *this;
		}

		Tree& shuffle(){
			shuffleOrder();
			shuffleDirection();
			return *this;
		}

		void print(FILE * pFile = stdout){
			int N = _path_from.size();
			fprintf(pFile, "%d\n",N + 1);
			for (int i=0;i<N;i++){
				fprintf(pFile, "%d %d\n", _path_from[i], _path_to[i] );
			}
		}

	};

}