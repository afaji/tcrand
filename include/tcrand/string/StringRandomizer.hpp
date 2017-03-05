#pragma once

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include <map>
#include <regex>
#define DEBUG false
using namespace std;

namespace tcrand {

class StringRandomizer{
	string result;
	int params_minlength;
	int params_maxlength;

	int num_length;

	vector<char> charset_;
	enum StringType { type_none, type_palindrome };
	StringType string_type;

	void load_params(){
		int len = params_minlength + rand() % (params_maxlength - params_minlength + 1);
		num_length = len;
	}


	string generate(int n){
		if (charset_.size() == 0)
			charset("a-z");

		string tmp = "";
		for (int i=0;i<n;i++){
			int idx = rand() % charset_.size();
			tmp += charset_[idx];
		}

		return tmp;
	}


public:
	StringRandomizer() {
		params_minlength = 1;
		params_maxlength = 20;
		string_type = type_none;
	}
	StringRandomizer& charset(regex e){
		for (int i=0;i<256;i++){
			string test = "";
			test += (char)i;
			if(regex_match(test, e))
				charset_.push_back(i);
		}
		return *this;
	}

	StringRandomizer& charset(string s){
		regex tmp("[" + s + "]");
		return charset(tmp);
	}

	StringRandomizer& length(int lo, int hi){
		params_minlength = lo;
		params_maxlength = hi;
		return *this;
	}

	StringRandomizer& length(int n){
		return length(n,n);
	}

	StringRandomizer& palindrome(){
		string_type = type_palindrome;
		return *this;
	}

	string next(){
		load_params();
		result = "";
		if (string_type == type_palindrome){
			string tmp = generate(num_length/2);
			if (num_length % 2)
				result += generate(1);
			string copy(tmp);
			reverse(copy.begin(), copy.end());
			result = tmp + result + copy;
		} else {
			result = generate(num_length);
		}
		return result;
	}

};

}