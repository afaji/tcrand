#pragma once
#include <cstdio>
#include <string>
#include "random.hpp"
using namespace std;

namespace tcrand {
	template <typename Vector>
	Vector rand_subarray(Vector v, int length){
		int start_pos = rand_int(v.size() - length);
	
		return Vector(v.begin() + start_pos , v.begin() + start_pos + length);
	}
	
	template <typename Vector>
	Vector rand_subarray(Vector v){
		return rand_subarray(v, rand_int(1, v.size() - 1) );
	}


	template <typename Vector>
	Vector rand_subsequence(Vector v, int length){
		Vector res;
		int N = v.size();
		for (int i=0;i<N;i++){
			int c = 1 + rand_int( N - i );
			
			if (c <= length ){
				res.push_back(v[i]);
				length--;
			}
		}
		return res;
	}

	template <typename Vector>
	Vector rand_subsequence(Vector v){
		return rand_subsequence(v, rand_int(1, v.size() - 1) );
	}
}