#include "tcrand/tree.hpp"
#include <iostream>
#include <ctime>
#include <tuple>
using namespace std;
using namespace tcrand;


int main() {       n   
	Tree t = tree_rand.node(6).next();
	cout<<"I am printing the tree (result may vary)"<<endl;
	t.print();

	cout<<"Obtaining the path simultaneously via .path()"<<endl;
	vector<int> p1, p2;
	tie(p1, p2) = t.path();
	cout<<"vector p1 : "; for (int v: p1) cout<< v <<" "; cout<<endl;
	cout<<"vector p2 : "; for (int v: p2) cout<< v <<" "; cout<<endl;

	cout<<"Obtaining the path individually via .pathFrom() and .pathTo()"<<endl;
	vector<int> p3, p4;
	p3 = t.pathFrom();
	p4 = t.pathTo();
	cout<<"vector p3 : "; for (int v: p3) cout<< v <<" "; cout<<endl;
	cout<<"vector p4 : "; for (int v: p4) cout<< v <<" "; cout<<endl;

	cout<<"Shuffling the path order"<<endl;
	t.shuffleOrder().print();
	cout<<"Shuffling the direction"<<endl;
	t.shuffleDirection().print();

}