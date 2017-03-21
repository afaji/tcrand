#include "tcrand/graph.hpp"
#include <iostream>
#include <ctime>
#include <tuple>
using namespace std;
using namespace tcrand;




int main(){
	tcrand::GraphRandomizer gr;
	Graph g = gr.node_count(1)
				.edge_count(200000)
				.multigraph()
				.self_loop()
				.next();

	vector<int> p1, p2;
	p1 = g.edges().first;
	p2 = g.edges().second;
	
	for (int i=0;i<g.edge_count();i++){
		cout<<p1[i] <<" "<<p2[i]<<endl;
	}

}
