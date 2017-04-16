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
			Tree(int root, vector<int> p_from, vector<int> p_to, vector<int> parents):
				Graph(p_from.size() + 1, p_from, p_to) {
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