#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <functional>
#include <cstdio>
#include <iostream>
#include "../utility/utility.hpp"
#define DEBUG false
using namespace std;

namespace tcrand {

	template <class T>
	class VectorRandomizer{
		vector<T> result;
		function<T(void)> generator;
		map<T, int> occurences;
		int param_length;

		int _distinct_elements;
		bool is_unique;


		void load_params(){
			occurences.clear();
			if (_distinct_elements == -1 && is_unique){
				_distinct_elements = param_length;
			}
		}


	public:

		VectorRandomizer(){
			param_length = 10;
			is_unique = false;
			_distinct_elements = -1;
		}

		VectorRandomizer& length(int n){
			param_length = n;
			return *this;
		}

		VectorRandomizer& engine(function<T(void)> f){
			generator = f;
			return *this;
		}

		VectorRandomizer& unique(bool u){
			is_unique = u;
			_distinct_elements = -1;
			return *this;
		}

		VectorRandomizer& distinct_elements(int n){
			is_unique = true;
			_distinct_elements = n;
			return *this;
		}


		vector<T> next(){
			load_params();
			if (!generator){
				throw runtime_error("please define your engine");
			}
			result.clear();

			//create all valid sets
			int options = 0;
			if (is_unique && _distinct_elements > 0){
				while (result.size() < _distinct_elements){
					T res = (T) generator();
					if (occurences[res])
						continue;
					occurences[res]++;
					result.push_back(res);
					options++;
				}
			}
			
			while (result.size() < param_length ){
				T res;
				if (_distinct_elements > 0){
					res = result[ rand_int(options) ];
				} else {
					res = (T) generator();
				}

				result.push_back(res);
			}
			random_shuffle(result.begin(), result.end());
			return result;
		}
	};
}