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

	class TreeRandomizer{
		int num_node;

		int params_leaf_min;
		int params_leaf_max;
		int params_child_min;
		int params_child_max;
		int params_depth;

		// temporary parameter. only used when next() is called.
		// these params are loaded before from params variable above
		int num_leaf;
		int leaf_min;
		int leaf_max;

		int num_child;
		int child_min;
		int child_max;

		int num_depth;
		bool is_depth_set;

		enum TreeType { type_basic, type_star, type_chain , type_caterpillar , type_binary , type_bst, type_heap };
		TreeType tree_type;

		//stores the solution
		vector<int> parent;
		vector<int> nodeWeight;
		vector<int> pathWeight;

		const int MAGIC = 30;

		void validate(){
			switch(tree_type){
				case type_star: 
					
					break;
				case type_chain:

					break;
				case type_caterpillar:

					break;
			}
		}

		/* helper method. connect child_id to par_id
		 */
		void tree_connect(int child_id , int par_id){
			parent[child_id] = par_id;
		}


		bool merge_components(vector<int> &inner_child, int _comp, bool greedy_mode){
			// connect all nodes into a single tree. Initially we only have root 0
			vector<int> options;
			vector<int> opt_depth;
			options.push_back(0);
			opt_depth.push_back(0);
			int node_id = 1;
			int start_id = 0;

			for (int i=0;i<_comp;i++){
				if (options.size() == start_id)
					return false;

				int idx = randInt( start_id, options.size() - 1);
				if (greedy_mode)
					idx = start_id;
					
				if (tree_type == type_caterpillar)
					idx = options.size() - 1;

				int tmp = node_id;
				for (int j=0;j<inner_child[i];j++){
					tree_connect(node_id++, options[idx]);
					
					if (!is_depth_set || num_depth > opt_depth[idx] + 1){
						options.push_back(node_id - 1);
						opt_depth.push_back(opt_depth[idx] + 1);
					}
				}

				swap( options[idx] , options[start_id] );
				swap( opt_depth[idx] , opt_depth[start_id] );
				start_id++;
			}

			return true;
		}

		/* build 'comp' amount of subtrees, where each of them has one internal node and [child_min, child_min + _range] leaves.
		 * need: the number of available single nodes that have to be connected into one of the subtree.
		 * merge all subtrees into a single tree.
		 */
		bool random_tree_connect(int _comp, int child_min, int _range, int need, bool greedy_mode = 0){
			vector<int> inner_child;

			inner_child.clear();
			for (int i=0; i<_comp;i++){
				inner_child.push_back(child_min);
			}
			int in_size = inner_child.size();
			while(need){
				need--;
				int idx = randInt(in_size);
				if (greedy_mode)
					idx = 0;
				inner_child[idx]++;
				if (inner_child[idx] == child_min + _range){
					swap(inner_child[idx] , inner_child[in_size - 1]);
					in_size--;
				}
			}

			for (int i=0;need && i<_comp;i++){
				if (inner_child[i] < child_max){
					int addition = min(need , child_max - inner_child[i]);
					inner_child[i] += addition;
					need -= addition;
				}
			}

			if (!greedy_mode)
				random_shuffle(inner_child.begin(), inner_child.end());
			if (need){
				fprintf(stderr, "XXimpossible to generate tree with the given parameter(s)\n");
				exit(1);	
			}

			if (DEBUG)
				fprintf(stderr, "Building Tree: Final leaf %d\n", num_leaf);

			return merge_components(inner_child, _comp, greedy_mode);
		}

		/*
		 * construct all possible subtree components based on the given parameters
		 * then randomly select one configuration and build the tree.
		 */
		void calculate_tree(){
			int _range =  min(num_node, child_max - child_min);
			int _node_left = num_node - 1;
			int _surplus = 0;
			
			int _comp = 0;
			int _curr_pos = 1; //nodes currently used
			int _curr_leaf = 1; //leaf currently available
			int _balance_depth = 1; 
			long long _next_depth_increase = 1;
			long long _next_depth_adder = 1;
			vector <pair<int, int> > _leaf_opts;

			while (_curr_pos <= num_node){
				if (DEBUG){
					fprintf(stderr, "now has %d nodes, %d leaves, and is possible to add up to %d leaf nodes\n", _curr_pos, _curr_leaf, _surplus);
					fprintf(stderr, " to reach %d node,  we will have final leaf of %d\n", num_node, _curr_leaf + _node_left);


				}
				if (_curr_pos + _surplus >= num_node){
					int _valid_leaf = _curr_leaf + _node_left;

					if (_valid_leaf >= leaf_min   
						&& _valid_leaf <= leaf_max 
						&& (!is_depth_set || (_comp >= num_depth && _balance_depth <= num_depth ) ) ) //total subtree comp. has to be atleast depth, and the result of the balance tree's depth cannot exeed depth
						{
						_leaf_opts.push_back(make_pair(_valid_leaf, _comp) );
						if (DEBUG) 
							fprintf(stderr, "  add [%d] to candidate leaf. Has %d components and balance tree depth is %d\n", _valid_leaf, _comp, _balance_depth);
						}
				}
				_node_left -= child_min;
				_curr_pos += child_min;
				_curr_leaf += (child_min - 1);
				_surplus = min(_surplus + _range, num_node);
				if (_comp >= _next_depth_increase){
					_balance_depth++;
					_next_depth_adder *= child_max;
					_next_depth_increase += _next_depth_adder;
				}
				_comp++;
				if (tree_type == type_caterpillar) //if caterpillar, expected depth == expected component;
					_balance_depth = _comp;
			}

			if (_leaf_opts.size() == 0){ //impossible
				fprintf(stderr, "impossible to generate tree with the given parameter(s)");
				exit(1);
			}
			int tries = 20;
			while (tries--){
				//choses num of leaf randomly
				int idx = (randInt(_leaf_opts.size()) + randInt(_leaf_opts.size()) )/ 2;

				num_leaf = _leaf_opts[idx].first;
				_comp = _leaf_opts[idx].second;
				//require:
				int need = (num_leaf - 1) - (_comp * (child_min - 1));
				//construct inner nodes
				if (random_tree_connect(_comp, child_min, _range, need, tries == 0))
					return;
			}
		}

		void load_params(){
			child_min = params_child_min;
			child_max = params_child_max;

			leaf_min = params_leaf_min;
			leaf_max = params_leaf_max;

			num_depth = params_depth;

			//override some params for spesific tree type
			switch(tree_type){
				case type_chain:
					child_min = 1;
					child_max = 1;
					leaf_min = 1;
					leaf_max = 1;
					num_depth = num_node - 1;
					break;

				case type_star:
					child_min = num_node - 1;
					child_max = num_node - 1;
					leaf_min = num_node - 1;
					leaf_max = num_node - 1;
					num_depth = 1;
					break;

				case type_binary:
					child_min = 1;
					child_max = 2;
					break;
				case type_caterpillar:
					//merge_components will be overrided 
					break;

			}
		}

		void load_tree(Tree &t, const vector<int> &par){
			vector<int> from;
			vector<int> to;
			int N = par.size();
			for (int i=0;i<N;i++){
				if (par[i] == -1)
					continue;
				to.push_back(i);
				from.push_back(par[i]);	
			}
			t.pathTo(to);
			t.pathFrom(from);
		}

	public:
		TreeRandomizer() {
			params_child_min = 1;
			params_child_max = 1000000;

			params_leaf_min = 1;
			params_leaf_max = 1000000;
			num_node = 10;
			is_depth_set = false;
			tree_type = type_basic;
			if (DEBUG)
				fprintf(stderr, "new TreeRandomizer is initiated\n");
		}

		Tree next(){
			parent.clear();
			load_params();

			for (int i=0;i<num_node;i++)
				parent.push_back(-1);

			calculate_tree();

			Tree t;
			load_tree(t, parent);
			return t;
		}

		TreeRandomizer& node(int n){
			num_node = n;
			return *this;
		}


		TreeRandomizer& child(int lo, int hi){
			if (lo > hi)
				swap(lo,hi);
			params_child_min = lo;
			params_child_max = hi;

			return *this;
		}

		TreeRandomizer& child(int n){
			child(n, n);
			return *this;
		}

		TreeRandomizer& leaf(int lo, int hi){
			if (lo > hi)
				swap(lo,hi);
			params_leaf_min = lo;
			params_leaf_max = hi;
			return *this;
		}

		TreeRandomizer& leaf(int n){
			leaf(n, n);
			return *this;
		}

		TreeRandomizer& depth(int d){
			params_depth = d;
			is_depth_set = true;
			return *this;
		}
		
		TreeRandomizer& star(){
			tree_type = type_star;
			return *this;
		}

		TreeRandomizer& binary(){
			tree_type = type_binary;
			return *this;
		}

		TreeRandomizer& chain(){
			tree_type = type_chain;
			return *this;
		}

		TreeRandomizer& caterpillar(){
			tree_type = type_caterpillar;
			return *this;
		}


	};

}