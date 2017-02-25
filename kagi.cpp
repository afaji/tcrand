#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
using namespace std;

class RuleBase {
protected:
	string name;
	vector<RuleBase> conflict_list;
};

class NumericalRule : public RuleBase {
public:
	virtual bool is_valid(long long x){	
		throw invalid_argument(name + " Rule does not support integer value");
	}
	
	virtual bool is_valid(double x){	
		throw invalid_argument(name + " Rule does not support floating point value");
	}

	virtual bool remap(long long x, long long &res){
		return false;
	}

	virtual bool remap(double x, double &res){
		return false;
	}

	virtual bool pool(long long lo, long long hi, set<long long> &res){
		return false;
	}

	virtual bool pool(double lo, double hi, set<double> &res){
		return false;
	}
	
};


class EvenRule : public NumericalRule {
	public:
	EvenRule(){
		name = "even";
	}

	bool is_valid(long long x){ 
		return (x % 2) == 0;
	}


	bool remap(long long x, long long &res){
		res = (x/2)*2;
		return true;
	}

	 bool pool(long long lo, long long hi, set<long long> &res){
	 	if (hi - lo > 50000)
	 		return false;

	 	if (lo % 2)
	 		lo++;
	 	for (int i=lo;i<=hi;i+=2)
	 		res.insert(i);
	 	return true;
	 }
};

template <class T>
class NumberGenerator{
	bool exist_pool;
	set<T> pool;
	vector<T> v_pool;
	T pool_min;
	T pool_max;

	vector< NumericalRule* > rules;
	vector< bool > is_verified;


	void verify_pool(int idx){
		if (is_verified[idx])
			return;

		NumericalRule* r = rules[idx];

		for (T val: pool){
			if (!r->is_valid(val))
				pool.erase(val);
		}
		is_verified[idx] = true;
	}

	void repool(T lo, T hi){
		int N = rules.size();
		bool pool_added = 0;
		for (int i=0; !pool_added && i<N ;i++){
			NumericalRule* r = rules[i];
			if (r->pool(lo, hi, pool)){
				for (int j=0; j < N;j++)
					is_verified[j] = false;
				pool_added = true;
				is_verified[i] = true;
			
			}
		}
		if (pool_added){
			for (int i=0;i<N;i++)
				verify_pool(i);
			if (!exist_pool){
				exist_pool = true;
				pool_min = lo;
				pool_max = hi;
			} else {
				pool_min = min(pool_min, lo);
				pool_max = max(pool_max, hi);
			}
			v_pool.clear();
			for (T x:pool)
				v_pool.push_back(x);
		}
		//cout<<"pooling "<<endl;
	}

	T bogo_gen(T lo, T hi){

		int N = rules.size();
		while (true){
			double ran = rand() / (double) RAND_MAX;
			T x = (T) (lo + ran*(hi - lo + 1));
			x = (T) lo + rand() % (hi - lo + 1);			

			bool all_valid = true;
			for (int i=0;i<N;i++)
				if ( !rules[i]->is_valid(x) ){
					all_valid = false;
					break;
				}
			if (all_valid)
				return x;
		}
	}

	T generate_from_pool(T lo, T hi){
		int lo_idx = lower_bound (v_pool.begin(), v_pool.end(), lo) - v_pool.begin(); 
  		int hi_idx = upper_bound (v_pool.begin(), v_pool.end(), hi) - v_pool.begin();
  		int idx = lo_idx + rand() % (hi_idx - lo_idx);

  		return v_pool[idx];
	}

public:
	//ADD NEW RULE HERE
	void even(){
		addRule(new EvenRule());
	}

	void addRule(NumericalRule* r){
		//verify_rule(r);
		rules.push_back(r);
		is_verified.push_back(false);

		if (exist_pool)
			verify_pool(rules.size() - 1);
	}

	T generate(T lo, T hi){
		if (rules.size() == 0)
			return bogo_gen(lo, hi);
		if (!exist_pool){
			repool(lo, hi);
		} else {
			if (lo >= pool_min && pool_max >= hi){} // new range is inside the pool range
			if (lo < pool_min && pool_max < hi){ // new range is covering the pool range
				repool(lo, hi);
			}
			if (lo < pool_min && hi <= pool_max){ // to the left
				repool(lo, pool_min);
			}
			if (lo >= pool_min && pool_max < hi){
				repool(pool_max, hi);
			}
		}

		//if in range
		if (lo >= pool_min && pool_max >= hi){
			return generate_from_pool(lo, hi);
		} else 
			return bogo_gen(lo, hi);
	}
};
typedef NumberGenerator<long long> IntegerGen;
typedef NumberGenerator<double> FloatGen;


int main() {
	IntegerGen num;
	num.even();

	int hist[11];
	memset(hist , 0 ,sizeof hist);
	for (int i=0;i<1000000;i++)
		hist[num.generate(0,10)]++;

	for (int i=0;i<=10;i++)
		printf("histogram %d : %d\n", i ,hist[i]);
}