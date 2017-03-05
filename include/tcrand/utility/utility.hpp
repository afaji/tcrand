#pragma once
#include <cstdio>
#include <string>
using namespace std;

namespace tcrand {
	string random_substring(string& data, int length){
		int start_pos = rand() % (data.size() - length);
		return data.substr(start_pos, length);
	}

	string random_substring(string& data){
		return random_substring(data, 1 + rand() % (data.size() - 1));
	}

	string random_substring(char* data){
		string data_str(data);
		return random_substring(data_str);
	}

	template <typename Vector>
	Vector random_subvetor(Vector v, int length){
		int start_pos = rand() % (v.size() - length);
		return Vector(v.begin() + start_pos , v.end() + start_pos + length);
	}
	
	template <typename Vector>
	Vector random_subvetor(Vector v){
		return random_subvetor(v, 1 + rand() % (v.size() - 1));
	}
}