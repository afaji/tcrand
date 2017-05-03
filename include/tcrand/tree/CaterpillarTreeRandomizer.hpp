#pragma once
#include "TreeRandomizer.hpp"

using namespace std;

namespace tcrand {

    template <class T>
    class CaterpillarTreeRandomizerBase: public TreeRandomizerBase<T>{
            typedef TreeRandomizerBase<T> Base;        
            vector<int> caterpillar_body;
            vector<int> leg_counts;
            vector<int> par;
            int body_options;
        protected:
            void add_body(int node_id){
                caterpillar_body.push_back(node_id);
                leg_counts.push_back(0);
                body_options++;
            }

            void add_leg(int c_body_id, int node_id){
                int body_id = caterpillar_body[c_body_id];
                par[node_id] = body_id;
                
                leg_counts[c_body_id]++;
                if (leg_counts[c_body_id] == Base::params_child_count_max){
                    body_options--;
                    swap( caterpillar_body[body_options] , caterpillar_body[c_body_id] );
                    swap( leg_counts[body_options] , leg_counts[c_body_id] );
                }
            }

        public:

            Tree next(){

                int n = Base::params_node_count;
                par.clear();
                caterpillar_body.clear();
                leg_counts.clear();
                body_options = 0;

                //zero is root, and also body
                par = vector<int>(n);

                par[0] = -1;

                add_body(0);
                int i=1;
                if (Base::params_depth > 0){
                    for (;i<Base::params_depth;i++){
                        add_leg(i-1, i);
                        add_body(i);
                    }
                } else if (Base::params_leaf_count > 0){
                    //calculate the depth given the leaf:
                    int d = Base::params_node_count - Base::params_leaf_count;
                    for (;i<=d;i++){
                        add_leg(i-1, i);
                        add_body(i);
                    }

                } else {
                    vector<int> options;
                    //get all possible depth, then select randomly:
                    for (int j=1;j<Base::params_node_count;j++){
                        if (j * (Base::params_child_count_min) < Base::params_node_count 
                            &&  j * (Base::params_child_count_max) >= Base::params_node_count )
                            options.push_back(j);
                    }

                    if (options.size() == 0)
                        throw runtime_error("Opt: impossible to generate caterpillar tree with the given parameter(s)");
                    
                    int d = options[rand_int(options.size()) ];

                    for (;i<d;i++){
                        add_leg(i-1, i);
                        add_body(i);
                    }

                }
                int j = 0;
                for (;i<n;i++){
                    while (j < body_options && leg_counts[j] >= Base::params_child_count_min) j++;
                    if (j >= body_options)
                        break;
                    if (leg_counts[j] < Base::params_child_count_min)
                        add_leg(j,i);
                
                }
                while (j < body_options && leg_counts[j] >= Base::params_child_count_min) j++;
                if (j < body_options)
                    throw runtime_error("J: impossible to generate caterpillar tree with the given parameter(s)");

                for (;i<n;i++){
                    if (body_options == 0)
                        throw runtime_error("B: impossible to generate caterpillar tree with the given parameter(s)");
                    int p = rand_int(body_options);
                    add_leg(p,i);
                }
                return Base::load_tree(par);
            }
    };

    class CaterpillarTreeRandomizer: public CaterpillarTreeRandomizerBase<CaterpillarTreeRandomizer>{};
}