#pragma once
#include <cstdio>
#include <string>
#include "random.hpp"
using namespace std;

namespace tcrand {
	string randomSubstring(string& data, int length){
		int start_pos = randInt(data.size() - length);
		return data.substr(start_pos, length);
	}

	string randomSubstring(string& data){
		return randomSubstring(data, randInt(1, data.size() - 1) );
	}

	string randomSubstring(char* data){
		string data_str(data);
		return randomSubstring(data_str);
	}

	template <typename Vector>
	Vector randomSubvector(Vector v, int length){
		int start_pos = randInt(v.size() - length);
	
		return Vector(v.begin() + start_pos , v.begin() + start_pos + length);
	}
	
	template <typename Vector>
	Vector randomSubvector(Vector v){
		return randomSubvector(v, randInt(1, v.size() - 1) );
	}


	template <typename Vector>
	Vector randomSubsequence(Vector v, int length){
		Vector res;
		int N = v.size();
		for (int i=0;i<N;i++){
			int c = 1 + randInt( N - i );
			
			if (c <= length ){
				res.push_back(v[i]);
				length--;
			}
		}
		return res;
	}

	template <typename Vector>
	Vector randomSubsequence(Vector v){
		return randomSubsequence(v, randInt(1, v.size() - 1) );
	}
}