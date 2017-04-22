#include "tcrand/graph.hpp"
#include "tcrand/tree.hpp"

#include <iostream>
#include <ctime>
#include <tuple>

using namespace std;
using namespace tcrand;
int main(){
	tcrand::GraphRandomizer gr;
	Graph g = GraphRandomizer().index_base(1).node_count(90000).edge_count(200993).next();

	vector<int> ap = articulation_points(g);
	cout<<ap.size()<<endl;
	vector<int> U = g.edges().first;
	vector<int> V = g.edges().second;
	int M = g.edge_count();
	for (int i=0;i<M;i++)
		cout<<U[i]<<" "<<V[i]<<endl;

}
