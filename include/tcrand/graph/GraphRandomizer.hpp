#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include "../object.hpp"
#define DEBUG false
using namespace std;

namespace tcrand {

	namespace internal {
		vector<int> random_with_sum(int count, int sum){
			vector<int> res;
			for (int i=0;i < count-1;i++) res.push_back(rand_int(sum - count + 1));
			res.push_back(sum - count);
			sort(res.begin(), res.end());
			for (int i=count-1;i>=1;i--) res[i] = 1 + (res[i] - res[i-1]);
			res[0]++;
			//validate
			int tot = 0;
			for (int x:res)
				tot+=x;
			return res;
		}
		
		inline void join_vector(vector<int> &a, vector<int> b){
			for (int v:b)
				a.push_back(v);
		}

		Graph to_biconnected(vector<Graph> subgraphs, int base = 0){
			int N = subgraphs.size();
			vector<int> U;
			vector<int> V;
			vector<int> nodes;
			vector<bool> is_leaf;
			for (int i=0 ;i < N; i++){
				//join one:
				if (nodes.size() > 0){
					int id = rand_int(max(0,i - 30) , i-1);
					is_leaf[id] = false;
					int st = subgraphs[id].nodes()[ rand_int(subgraphs[id].node_count()) ];
					int ed = subgraphs[i].nodes()[ rand_int(subgraphs[i].node_count()) ];
					if (st > ed) swap(st, ed);
					U.push_back(st); V.push_back(ed);
				}
				is_leaf.push_back(true);
				join_vector(nodes, subgraphs[i].nodes());
				join_vector(U, subgraphs[i].edges().first);
				join_vector(V, subgraphs[i].edges().second);
			}

			int past = 0;
			for (int i=1;i<N;i++){
				if (!is_leaf[i]) continue;
				
				int st = subgraphs[past].nodes()[ rand_int(subgraphs[past].node_count()) ];
				int ed = subgraphs[i].nodes()[ rand_int(subgraphs[i].node_count()) ];
				if (st > ed) swap(st, ed);
					U.push_back(st); V.push_back(ed);
				past = i;
			}
			return Graph(nodes, U, V, base);
		}

		Graph to_single_tree(vector<Graph> subgraphs, int base = 0){
			int N = subgraphs.size();
			vector<int> U;
			vector<int> V;
			vector<int> nodes;
			for (int i=0 ;i < N; i++){
				
				//join one:
				if (nodes.size() > 0){
					int st = nodes[ rand_int(nodes.size()) ];
					int ed = subgraphs[i].nodes()[ rand_int(subgraphs[i].node_count()) ];
					if (st > ed) swap(st, ed);
					U.push_back(st); V.push_back(ed);
				}
				join_vector(nodes, subgraphs[i].nodes());
				join_vector(U, subgraphs[i].edges().first);
				join_vector(V, subgraphs[i].edges().second);
			}
			return Graph(nodes, U, V, base);
		}

	}

using namespace internal;

class GraphRandomizer{
	int params_node_count;
	int params_edge_count;
	int params_component_count;
	int params_index_base;
	int params_multi_path;
	int params_cut_vertices;
	bool params_self_loop;



	Graph load_graph(const map<pair<int, int> , int>& pathSet){
		
		vector<int> from;
		vector<int> to;
		vector<int> nodes;
		for (int i=0;i<params_node_count;i++)
			nodes.push_back(i);

		for (auto path: pathSet){
			for (int i=0;i<path.second;i++){
				from.push_back(path.first.first);
				to.push_back(path.first.second);
			}
		}
		Graph g = Graph(nodes, from, to, params_index_base);
		return g;
	}
	
	template <typename Func>
	vector<Graph> join_as_components(vector<Graph> subgraphs, Func merge_func, int tree_comp = 1){
		int tot_nodes = 0;
		for (auto g: subgraphs)
			tot_nodes += g.node_count();
		vector<int> tree_size = random_with_sum(tree_comp, tot_nodes);
		vector<Graph> res;
		int id = 0;
		for (int i=0;i<tree_comp;i++){
			vector<Graph> g;
			for (int j=0;j<tree_size[i];j++){
				g.emplace_back(subgraphs[id++]);
			}
			res.emplace_back(merge_func(g, params_index_base));
		}
		return res;
	}

	vector<Graph> init_subgraphs(int n){
		vector<Graph> res;
		int b = params_index_base;
		for (int i=0;i<n;i++){
			vector<int> nodes, U, V;
			nodes.push_back(b);
			res.emplace_back(Graph( nodes, U, V, b));
			b++;
		}
		return res;
	}

	Graph merge_graph(vector<Graph> subgraphs, int leftover, int cut_vertices = 0){
		
		map< pair<int,int>, int> path;
		
		vector<int> nodes;
		vector<int> component_id;
		vector<int> component_size;
		vector<int> component_offset;
		int offset = 0;
		int c_id = 0;
		int N = subgraphs.size();
		for (int i=0;i< N;i++){
			Graph &g = subgraphs[i];
			auto V = g.edges().first;
			auto U = g.edges().second;
			//join_vector(nodes, g.nodes());
			for (auto node : g.nodes()){
				nodes.push_back(node);
				component_id.push_back(c_id);
			}
			c_id++;
			component_size.push_back(g.node_count());
			component_offset.push_back(offset);
			offset += g.node_count();

			int x = V.size();
			for (int i=0;i<x;i++){
				path[make_pair(U[i], V[i])]++;
			}

			//add articulation point
			if (cut_vertices > 0 && i > 0 && g.node_count() > 1 && subgraphs[i-1].node_count() > 1){
				cut_vertices--;
				leftover-=2;
				int u = g.nodes()[rand_int(g.node_count())];
				int v = subgraphs[i-1].nodes()[rand_int(subgraphs[i-1].node_count())];
				int u2 = u;
				int v2 = v;
				while (v2 == v) v2 = subgraphs[i-1].nodes()[rand_int(subgraphs[i-1].node_count())];

				if (u > v) swap(u,v);
				if (u2 > v2) swap(u2,v2);
				path[make_pair(u,v)]++;
				path[make_pair(u2,v2)]++;
			}
		}

		int node_size = nodes.size();
		//can it be dense graph?
		vector<pair<int,int>> options;
		if (node_size * node_size * params_multi_path <= 1000000){
			for (int j=0;j<node_size;j++)
				for (int k=0;k<node_size;k++)
					for (int i=0;i<params_multi_path;i++)
						options.push_back(make_pair(nodes[j],nodes[k]));

			random_shuffle(options.begin(), options.end());
		}
		int opt_id = 0;
		while (leftover > 0){
			int st = nodes[rand_int(node_size)];
			
			int comp_id = component_id[st - params_index_base];
			int ed = nodes[ component_offset[comp_id] + rand_int( component_size[comp_id] ) ];

			if (opt_id < options.size()){
				st = options[opt_id].first;
				ed = options[opt_id++].second;
			}
			while (opt_id >= options.size());

			if (st > ed) swap(st, ed);
			if (component_id[st - params_index_base] != component_id[ed - params_index_base]) continue;
			if (st == ed && !params_self_loop) continue;
			//check:
			if (path[make_pair(st,ed)] + path[make_pair(ed,st)] >= params_multi_path ) continue;
			path[make_pair(st, ed)]++;
			leftover--;
		}

		return load_graph(path);
	}


public:
	GraphRandomizer(){
		params_index_base = 0;
		params_component_count = 1;
		params_node_count = 8;
		params_edge_count = 10;
		params_self_loop = false;
		params_multi_path = 1;
		params_cut_vertices = -1;
	}

	GraphRandomizer& index_base(int b){
		params_index_base = b;
		return *this;
	}

	// parameters
	GraphRandomizer& component_count(int n){
		params_component_count = n;
		return *this;
	}
		
	GraphRandomizer& node_count(int n){
		params_node_count = n;
		return *this;
	}
	
	GraphRandomizer& edge_count(int n){
		params_edge_count = n;
		return *this;
	}

	GraphRandomizer& self_loop(){
		params_self_loop = true;
		return *this;
	}

	GraphRandomizer& multi_path(int m = 1000000){
		params_multi_path = m;
		return *this;
	}
	
	GraphRandomizer& cut_vertices(int m){
		params_cut_vertices = m;
		return *this;
	}


	Graph next(){
		while (true){
			vector<Graph> g = init_subgraphs(params_node_count);
			if (params_cut_vertices == -1)
				g = join_as_components(g, to_single_tree, params_component_count);
			else{
				g = join_as_components(g, to_biconnected, params_component_count + params_cut_vertices);
			}
			
			//add leftover edges, as long as they are from different components.
			int M = params_edge_count;
			for (auto _g:g)
				M -= _g.edge_count();

			Graph final = merge_graph(g, M, params_cut_vertices);
			
			if (final.edge_count() == params_edge_count)
				return final;
		}
	}
};

}

