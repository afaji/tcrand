#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include "../object.hpp"
#include "../tree.hpp"

using namespace std;

namespace tcrand {

	template <class T>
	class TreeRandomizerBase{


	protected:
		int params_node_count;
		int params_leaf_count;
		int params_child_count_min;
		int params_child_count_max;
		int params_depth;
		int params_index_base;
		int params_root;

		//create and return a Tree class given a vector of parents
		Tree load_tree(vector<int> &par){
			vector<int> from;
			vector<int> to;
			int N = par.size();

			int new_root = params_root - params_index_base;
			if (params_root == -1)
				new_root = 0;
			//swap root = 0 to params_root
			swap(par[0], par[new_root]);
			for (int i=0;i<N;i++){
				if (par[i] == 0)
					par[i] = new_root;
				else if (par[i] == new_root)
					par[i] = 0;
				if (par[i] == -1)
					continue;
				par[i] += params_index_base;
				to.push_back(i + params_index_base);
				from.push_back(par[i]);	
			}
			return Tree( new_root , par, params_index_base );
		}

	public:
		TreeRandomizerBase() {
			params_root = -1;
			params_index_base = 0;
			params_child_count_min = 1;
			params_child_count_max = 1000000;

			params_leaf_count = -1;
			params_node_count = 10;
			params_depth = -1;
			if (DEBUG)
				fprintf(stderr, "new TreeRandomizer is initiated\n");
		}

		Tree next(){
			parent.clear();
			load_params();
			validate();

			for (int i=0;i<params_node_count;i++)
				parent.push_back(-1);

			calculate_tree();

			Tree t = load_tree(parent);
			return t;
		}

		T& node_count(int n){
			params_node_count = n;
			return static_cast<T&>(*this);
		}


		T& child_count(int lo, int hi){
			if (lo > hi)
				swap(lo,hi);
			params_child_count_min = lo;
			params_child_count_max = hi;

			return static_cast<T&>(*this);
		}

		T& child_count(int n){
			child_count(n, n);
			return static_cast<T&>(*this);
		}

		T& leaf_count(int n){
			params_leaf_count = n;
			return static_cast<T&>(*this);
		}

		T& depth(int d){
			params_depth = d;
			return static_cast<T&>(*this);
		}

		T& root(int r){
			if (r < 0)
				throw runtime_error("root has to be non-negative");
			params_root = r;
			return static_cast<T&>(*this);
		}

		T& index_base(int o){
			if (o < 0)
				throw runtime_error("index base has to be non-negative");
			params_index_base = o;
			return static_cast<T&>(*this);
		}

	//INTERNAL
	//everything below is used to construct/generate the tree
	private:
		// temporary parameter. only used when next() is called.
		// these params are loaded before from params variable above
		int num_leaf;
		int leaf_min;
		int leaf_max;

		int num_child;
		int child_min;
		int child_max;

		int num_depth;

		//stores the solution
		vector<int> parent;

		void validate(){
			if (params_root >= 0 && params_root < params_index_base)
				throw runtime_error("Tree root is too small");
			if (params_root >= 0 && params_root > params_index_base + params_node_count)
				throw runtime_error("Tree root is too big");
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
			bool depth_reached = false;
			for (int i=0;i<_comp;i++){
				if (options.size() == start_id)
					return false;

				int idx = rand_int( start_id, options.size() - 1);
				if (greedy_mode)
					idx = start_id;
					
				if (num_depth < 0 && !depth_reached)
					idx = options.size() - 1;

				int tmp = node_id;
				for (int j=0;j<inner_child[i];j++){
					tree_connect(node_id++, options[idx]);
					
					if (num_depth < 0|| num_depth > opt_depth[idx] + 1){
						options.push_back(node_id - 1);
						opt_depth.push_back(opt_depth[idx] + 1);
					} else {
						depth_reached = true;
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
				int idx = rand_int(in_size);
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
			int _range =  min(params_node_count, child_max - child_min);
			int _node_left = params_node_count - 1;
			int _surplus = 0;
			int _comp = 0;
			int _curr_pos = 1; //nodes currently used
			int _curr_leaf = 1; //leaf currently available
			int _balance_depth = 1; 
			long long _next_depth_increase = 1;
			long long _next_depth_adder = 1;
			vector <pair<int, int> > _leaf_opts;

			while (_curr_pos <= params_node_count){
				if (DEBUG){
					fprintf(stderr, "now has %d nodes, %d leaves, and is possible to add up to %d leaf nodes\n", _curr_pos, _curr_leaf, _surplus);
					fprintf(stderr, " to reach %d node,  we will have final leaf of %d\n", params_node_count, _curr_leaf + _node_left);


				}
				if (_curr_pos + _surplus >= params_node_count){
					int _valid_leaf = _curr_leaf + _node_left;

					if (_valid_leaf >= leaf_min   
						&& _valid_leaf <= leaf_max 
						&& (num_depth < 0 || (_comp >= num_depth && _balance_depth <= num_depth ) ) ) //total subtree comp. has to be atleast depth, and the result of the balance tree's depth cannot exeed depth
						{
						_leaf_opts.push_back(make_pair(_valid_leaf, _comp) );
						if (DEBUG) 
							fprintf(stderr, "  add [%d] to candidate leaf. Has %d components and balance tree depth is %d\n", _valid_leaf, _comp, _balance_depth);
						}
				}
				_node_left -= child_min;
				_curr_pos += child_min;
				_curr_leaf += (child_min - 1);
				_surplus = min(_surplus + _range, params_node_count);
				if (_comp >= _next_depth_increase){
					_balance_depth++;
					_next_depth_adder *= child_max;
					_next_depth_increase += _next_depth_adder;
				}
				_comp++;
			}

			if (_leaf_opts.size() == 0){ //impossible
				fprintf(stderr, "impossible to generate tree with the given parameter(s)");
				exit(1);
			}
			int tries = 20;
			while (tries--){
				//choses num of leaf randomly
				int idx = (rand_int(_leaf_opts.size()) + rand_int(_leaf_opts.size()) )/ 2;

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
			child_min = params_child_count_min;
			child_max = params_child_count_max;

			if (params_leaf_count != -1) {
				leaf_min = params_leaf_count;
				leaf_max = params_leaf_count;
			}
			else
			{
				leaf_min = 1;
				leaf_max = 1000000;	
			}

			num_depth = params_depth;
		}
		
	};


	class TreeRandomizer: public TreeRandomizerBase<TreeRandomizer>{};
}