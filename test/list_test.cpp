#include "tcrand/list.hpp"
#include "tcrand/string.hpp"
#include <iostream>
#include <ctime>
#include <tuple>
using namespace std;
using namespace tcrand;


int main() {
	
	//we want to get a list of string, where for each string consists only of lowercase with length from 3 to 5
	//furthermore, we want the list to have 5 distinct elements
	//define the object
	StringRandomizer str_rnd;
	ListRandomizer<string> list_rnd;

	//define the string
	str_rnd.charset("[a-z]").length(3, 5);
	
	//define the list
	list_rnd.length(20).distinctElements(5);
	
	//embed the string randomizer into the listrandomizer
	list_rnd.engine( [&] {return str_rnd.next();} );

	//obtain and print the result
	vector<string> res = list_rnd.next();
	for (string v:res)
		cout<<v<<endl;
}