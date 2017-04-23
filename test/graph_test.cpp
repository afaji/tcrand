#include "tcrand/graph.hpp"
#include "tcrand/tree.hpp"

#include <iostream>
#include <ctime>
#include <tuple>

using namespace std;
using namespace tcrand;
int main(){
	tcrand::GraphRandomizer gr;
	//Graph g = PetersenGraphRandomizer().index_base(100).next();
	Graph g = GraphRandomizer().node_count(15).edge_count(20).index_base(5000).next();
	vector<int> ap = articulation_points(g);
	cout<<ap.size()<<endl;
	vector<int> U = g.edges().first;
	vector<int> V = g.edges().second;
	int M = g.edge_count();
	for (int i=0;i<M;i++)
		cout<<U[i]<<" "<<V[i]<<endl;

}
