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
#include "../string.hpp"
using namespace std;

namespace tcrand {

class StringRandomizer{
	string result;
	int params_minlength;
	int params_maxlength;

	int num_length;

	vector<char> charset_;
	vector<char> lead_charset_;
	vector<char> trail_charset_;

	enum StringType { type_none, type_palindrome };
	StringType string_type;

	void load_params(){
		if (charset_.size() == 0)
			charset("[a-z]");
		if (lead_charset_.size() == 0)
			lead_charset_ = charset_;
		if (trail_charset_.size() == 0)
			trail_charset_ = charset_;

		int len = rand_int(params_minlength, params_maxlength);
		num_length = len;
	}


	string generate(int n){
		string tmp = "";
		if (n == 1){
			tmp += lead_charset_[ rand_int(lead_charset_.size()) ];
			return tmp;
		}
		
		tmp += lead_charset_[ rand_int(lead_charset_.size()) ];

		for (int i=1;i<n-1;i++){
			int idx = rand_int(charset_.size());
			tmp += charset_[idx];
		}

		tmp += trail_charset_[ rand_int(lead_charset_.size()) ];

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
	
	StringRandomizer& first_charset(regex e){
		for (int i=0;i<256;i++){
			string test = "";
			test += (char)i;
			if(regex_match(test, e))
				lead_charset_.push_back(i);
		}
		return *this;
	}
	
	StringRandomizer& last_charset(regex e){
		for (int i=0;i<256;i++){
			string test = "";
			test += (char)i;
			if(regex_match(test, e))
				trail_charset_.push_back(i);
		}
		return *this;
	}

	StringRandomizer& charset(string s){
		regex tmp(s);
		return charset(tmp);
	}

	StringRandomizer& first_charset(string s){
		regex tmp(s);
		return first_charset(tmp);
	}

	StringRandomizer& last_charset(string s){
		regex tmp(s);
		return last_charset(tmp);
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