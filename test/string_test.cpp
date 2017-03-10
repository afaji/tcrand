#include "tcrand/string.hpp"
#include <iostream>
#include <ctime>
using namespace std;
using namespace tcrand;


void speed_test(){
	cerr<<"\nSpeed Test : generating 1000 random strings with length = 100000\n"<<endl;
	clock_t begin = clock();
	StringRandomizer str_rand;
	str_rand.length(100000);
	for (int i=0;i<1000;i++){
		str_rand.next();
	}
	clock_t end = clock();
  	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  	fprintf(stderr, "  time required: %.5lf sec.\n", elapsed_secs);
	cerr<<"\nSample Test: DONE\n"<<endl;
}


void sample_test(){
	cerr<<"\nSample Test: generate and print string with some predefined configurations\n"<<endl;
	StringRandomizer str_rand;
	cerr<<"  random string of length 5 : "<<str_rand.length(5).next()<<endl;
	StringRandomizer str_rand2;
	cerr<<"  random palindrome capital string of length 7 : "<<str_rand2.length(7).charset("[A-Z]").palindrome().next()<<endl;
	StringRandomizer str_rand3;
	cerr<<"  random binary string of length 10 : "<<str_rand3.length(10).charset("[01]").next()<<endl;
	cerr<<"  random substring of \"Hello world!\" : "<<random_substring("Hello world!")<<endl;


	cerr<<"\nSpeed Test: DONE\n"<<endl;
}

int main() {
	//sample_test();
	//speed_test();




	StringRandomizer str_rand;
	str_rand.charset("[GTCA]").length(20).palindrome();
	cout<< str_rand.next() <<endl;


	StringRandomizer number_str, operator_str;
	number_str.charset("[0-9]").leadingCharset("[1-9]").length(1, 6);
	operator_str.charset("[+*-]").length(1);	

	GrammarRandomizer math_grammar;
	math_grammar.grammar('N' , "(N O N)");
	math_grammar.grammar('N' , number_str);
	math_grammar.grammar('O' , operator_str);
	math_grammar.start("N");
	math_grammar.length_around(100);
	
	cout<<math_grammar.next()<<endl;
	cout<<math_grammar.next()<<endl;
	cout<<math_grammar.next()<<endl;
	cout<<math_grammar.next()<<endl;



	GrammarRandomizer brackets;
	brackets.grammar('S' , "SS");
	brackets.grammar('S' , "(S)");
	brackets.grammar('S' , "");
	brackets.start("S");
	cout<<brackets.length_around(10).next()<<endl;
	cout<<brackets.length_around(14).next()<<endl;
	cout<<brackets.length_around(18).next()<<endl;
	cout<<brackets.length_around(30).next()<<endl;











}