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

	vector<int> x = {1, 2, 3, 4, 5, 6, 7, 8};
	vector<int> y = randomSubsequence(x);
	vector<int> z = randomSubvector(x);


	for (int v:z)
		cout<<v<<endl;
	cout<<endl;
	for (int v:y)
		cout<<v<<endl;
	cout<<endl;

	string s = "lorem ipsum sir dolor amet";
	cout<<randomSubsequence(s,5)<<endl;
	cout<<randomSubstring(s,5)<<endl;

}