#pragma once
#include "TreeRandomizer.hpp"

using namespace std;

namespace tcrand {

	template <class T>
	class ChainTreeRandomizerBase: public TreeRandomizerBase<T>{
		public:
			T& depth(int d) = delete;
			T& leaf_count(int d) = delete;
			T& child_count(int d) = delete;
			T& child_count(int lo, int hi) = delete;
			
			ChainTreeRandomizerBase(){
				TreeRandomizerBase<T>::child_count(1);
			};
	};

	class ChainTreeRandomizer: public ChainTreeRandomizerBase<ChainTreeRandomizer>{};
}