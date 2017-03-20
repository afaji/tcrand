#include "tcrand/string.hpp"
#include <iostream>
#include <ctime>
using namespace std;
using namespace tcrand;


int main() {
	//sample_test();
	//speed_test();




	StringRandomizer str_rand;
	str_rand.charset("[GTCA]").length(20).palindrome();
	cout<< str_rand.next() <<endl;


	StringRandomizer number_str, operator_str;
	number_str.charset("[0-9]").first_charset("[1-9]").length(1, 6);
	operator_str.charset("[+*-]").length(1);	

	CfgStringRandomizer math_grammar;
	math_grammar.rule('N' , "(N O N)");
	math_grammar.rule('N' , number_str);
	math_grammar.rule('O' , operator_str);
	math_grammar.start_with("N");
	math_grammar.max_length(100);
	
	cout<<math_grammar.next()<<endl;
	cout<<math_grammar.next()<<endl;
	cout<<math_grammar.next()<<endl;
	cout<<math_grammar.next()<<endl;



	CfgStringRandomizer brackets;
	brackets.rule('S' , "SS");
	brackets.rule('S' , "(S)");
	brackets.rule('S' , "");
	brackets.start_with("S");
	cout<<brackets.max_length(10).next()<<endl;
	cout<<brackets.max_length(14).next()<<endl;
	cout<<brackets.max_length(18).next()<<endl;
	cout<<brackets.max_length(30).next()<<endl;











}