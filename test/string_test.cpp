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
	cerr<<"  random palindrome capital string of length 7 : "<<str_rand2.length(7).charset("A-Z").palindrome().next()<<endl;
	StringRandomizer str_rand3;
	cerr<<"  random binary string of length 10 : "<<str_rand3.length(10).charset("01").next()<<endl;
	cerr<<"  random substring of \"Hello world!\" : "<<random_substring("Hello world!")<<endl;


	cerr<<"\nSpeed Test: DONE\n"<<endl;
}

int main() {
	sample_test();
	speed_test();
}