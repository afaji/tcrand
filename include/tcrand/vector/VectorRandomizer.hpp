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
        int param_length;
        int param_distinct_elements;
        int param_min_count;
        int param_max_count;
        bool param_is_unique;

        function<T(void)> generator;


        T next_random_element(){
            return generator();
        }

        vector<T> __next(){
            int fail_attempt = 0;
            vector<T> result;
            map<T, int> counts;
            vector<T> result_set;

            while (fail_attempt < 1000 && result.size() < param_length){
                //attempt to add a possibly new element:
                if (result_set.size() < param_distinct_elements || param_distinct_elements == -1){
                    T element = next_random_element();
                    cout<<"AA "<<element<<endl;

                    //if exists before, try again
                    if ((param_is_unique && counts[element]) || (counts[element] + 1 > param_max_count)){
                        fail_attempt++;
                        continue;
                    }

                    //if adding it makes the array longer, try again
                    //if exists before, try again
                    if (result.size() + param_min_count > param_length){
                        fail_attempt++;
                        continue;
                    }

                    //if this element is not in our current result, add this by "param_min_count"
                    //otherwise, just add one
                    int add_amount = 1;
                    if (counts[element] == 0) {
                        result_set.push_back(element);
                        add_amount = param_min_count;
                    }

                    for (int i = 0;i < add_amount; i++) {
                        result.push_back(element);
                        counts[element]++;
                    }
                }

                //attempt to add an old element:
                else if (result_set.size() > 0 && param_is_unique == false){
                    int pick_id = rand_int(result_set.size());
                    T element = result_set[pick_id];

                    counts[element]++;
                    result.push_back(element);

                    if (counts[element] == param_max_count) {
                        swap(result_set[pick_id], result_set[result_set.size() - 1] );
                        result_set.pop_back();
                    }
                } else {
                    fail_attempt++;
                }
            }
            
            if (fail_attempt == 1000)
                throw runtime_error("failed to generate vector");

            random_shuffle(result.begin(), result.end());
            return result;
        }

    public:
        VectorRandomizer(){
            param_length = 10;
            param_is_unique = false;
            param_distinct_elements = -1;
            param_max_count = 9999999;
            param_min_count = 1;
        }

        VectorRandomizer& length(int n){
            param_length = n;
            return *this;
        }

        VectorRandomizer& unique(bool u){
            param_is_unique = u;
            param_distinct_elements = -1;
            return *this;
        }

        VectorRandomizer& distinct_elements(int n){
            param_is_unique = false;
            param_distinct_elements = n;
            return *this;
        }

        VectorRandomizer& element_counts(int min_count, int max_count){
            param_min_count = min_count;
            param_max_count = max_count;
        }

        VectorRandomizer& engine(){

        }

        VectorRandomizer& engine(function<T(void)> f){
            generator = f;
            return *this;
        }

        vector<T> next(){
            return __next();
        }
    };
}