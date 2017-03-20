#include "tcrand/tree.hpp"
#include <iostream>
#include <ctime>
#include <tuple>
using namespace std;
using namespace tcrand;


int main() {
	TreeRandomizer tree_rand;
	int N = 25;
	Tree t = tree_rand.node_count(N).depth(5).index_base(1).root(10).next();


	
	vector<int> p1, p2;
	p1 = t.edges().first;
	p2 = t.edges().second;
	
	for (int i=0;i<N-1;i++){
		cout<<p1[i] <<" "<<p2[i]<<endl;
	}
}