#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include "graph.hpp"
#define DEBUG false
using namespace std;

namespace tcrand {

	class Graph{
		vector<int> _path_from;
		vector<int> _path_to;
		int num_edges;
		int num_nodes;
		
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
		
		void setPath(vector<int> p_from, vector<int> p_to){
			_path_from = p_from;
			_path_to = p_to;
			num_edges = p_from.size();
		}

		void setNode(int n){
			num_nodes = n;
		}

		vector<int> pathFrom(){
			return _path_from;
		}

		vector<int> pathTo(){
			return _path_to;
		}

		int node(){
			return num_nodes;
		}

		int edge(){
			return num_edges;
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


		void print(FILE * pFile = stdout){
			fprintf(pFile, "%d %d\n",num_nodes, num_edges);
			for (int i=0;i<num_edges;i++){
				fprintf(pFile, "%d %d\n", _path_from[i], _path_to[i] );
			}
		}

	};

}