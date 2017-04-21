#include "tcrand/graph.hpp"
#include "tcrand/tree.hpp"

#include <iostream>
#include <ctime>
#include <tuple>

using namespace std;
using namespace tcrand;
int main(){
	tcrand::GraphRandomizer gr;
	Graph g = GraphRandomizer().node_count(10000).edge_count(20000).next();

	vector<int> ap = articulation_points(g);
	vector<int> U = g.edges().first;
	vector<int> V = g.edges().second;

	for (int i=0;i<ap.size();i++){
		cout<<ap[i]<<endl;
	}

}
