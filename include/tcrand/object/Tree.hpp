#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include "Graph.hpp"
#define DEBUG false
using namespace std;

namespace tcrand {

	class Tree: public Graph{
		protected:
			int _root;
			vector<int> _parents;

			Tree& shuffleDirection(){
			
			int N = _path_to.size();
				for (int i=0;i<N;i++){
					if (rand_int(100) % 2)
						swap(_path_to[i], _path_from[i]);
				}
				return *this;
			}

		public:
			Tree(){}

			Tree(int root, vector<int> parents, int base = 0)
			{
				vector<int> path_from;
				vector<int> path_to;
				vector<int> nodes;
				int M = parents.size();
				for (int i=0;i<M;i++){
					nodes.push_back(i + base);
					if (parents[i] == -1)
						continue;
					path_from.push_back(parents[i]);
					path_to.push_back(i + base);
				}

				Graph::init_graph(nodes, path_from, path_to, base);
				_parents = parents;
				_root = root;

				shuffleDirection();
			}

			int root(){
				return _root;
			}

			vector<int> parents(){
				return _parents;
			}
	};

}