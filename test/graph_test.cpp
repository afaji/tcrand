#include "tcrand/graph.hpp"
#include <iostream>
#include <ctime>
#include <tuple>
using namespace std;
using namespace tcrand;




int main(){
	tcrand::GraphRandomizer gr;
	Graph g = gr.node(5).edge(5).component(5).loop(true).next();
	g.print();

}
