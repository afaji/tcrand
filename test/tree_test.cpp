#include "tcrand/tree.hpp"
#include <iostream>
#include <ctime>
#include <tuple>
using namespace std;
using namespace tcrand;



void print(Tree t){

	cout<<"printing tree: "<<endl;
	vector<int> p1, p2;
	p1 = t.edges().first;
	p2 = t.edges().second;
	
	for (int i=0;i<p1.size() - 1;i++){
		cout<<p1[i] <<" "<<p2[i]<<endl;
	}
}

int main() {
	int N = 10;
	
	print( StarTreeRandomizer().node_count(N).next() );
	print( ChainTreeRandomizer().node_count(N).next() );
	print( TreeRandomizer().node_count(N).next() );

}