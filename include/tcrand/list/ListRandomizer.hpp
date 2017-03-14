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
	class ListRandomizer{
		vector<T> result;
		function<T(void)> generator;
		map<T, int> occurences;
		int length;

		int distinct_elements;
		bool is_unique;


		void load_params(){
			occurences.clear();
			if (distinct_elements == -1 && is_unique){
				distinct_elements = length;
			}
		}


	public:

		ListRandomizer(){
			length = 10;
			generator = []{return randDouble(0, 20); };
			is_unique = false;
			distinct_elements = -1;
		}

		ListRandomizer& engine(function<T(void)> f){
			generator = f;
			return *this;
		}

		ListRandomizer& unique(bool u){
			is_unique = u;
			distinct_elements = -1;
			return *this;
		}

		ListRandomizer& distinctElements(int n){
			is_unique = true;
			distinct_elements = n;
			return *this;
		}


		vector<T> next(){
			load_params();

			result.clear();

			//create all valid sets
			int options = 0;
			if (is_unique && distinct_elements > 0){
				while (result.size() < distinct_elements){
					T res = (T) generator();
					if (occurences[res])
						continue;
					occurences[res]++;
					result.push_back(res);
					options++;
				}
			}
			
			while (result.size() < length ){
				T res;
				if (distinct_elements > 0){
					res = result[ randInt(options) ];
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