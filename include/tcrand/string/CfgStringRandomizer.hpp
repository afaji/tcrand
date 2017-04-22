#pragma once
#define DEBUG false
#include "tcrand/string/StringRandomizer.hpp"
namespace tcrand {

class CfgStringRandomizer{
    map<char, vector<string> > all_cfg;

    map<char, vector<string> > nonterminal;
    map<char, vector<string> > terminal_str;
    map<char, vector< StringRandomizer* > > terminal_Randomizer;

    string params_start;
    int params_prob_length;
    int params_min_depth;
    int params_max_depth;

    int num_min_depth;
    int num_max_depth;
    vector<char> result;

     void load_params(){
         result.clear();
        prepare_cfg();
        num_max_depth = params_max_depth;
        num_min_depth = params_min_depth;
     }


    void prepare_cfg(){
        nonterminal.clear();
        //terminal_Randomizer.clear();
        terminal_str.clear();

        for (auto const &x : all_cfg){
            char key = x.first;
            //loop all rules
            for (auto str_rule: x.second){
                bool is_terminal = true;
                for (auto c: str_rule){
                    if (all_cfg.find(c) != all_cfg.end() ){
                        is_terminal = false;
                        break;
                    }
                }
            
                if (is_terminal)
                    terminal_str[key].push_back(str_rule);
                else
                    nonterminal[key].push_back(str_rule);
            }
        }
    }

    inline bool exists(char c, map<char, vector<string> >& m){
        return m.find(c) != m.end();
    }
    inline bool exists(char c, map<char, vector<StringRandomizer*> >& m){
        return m.find(c) != m.end();
    }


    int traverse_cfg(const string& rule, int depth){
        //we go from max depth from 0. if we reached 0, we stop using nonterminal
        //we will try not to use terminal before we reached target depth.
        //if we reached target_depth, it is optional to use terminal
        //IF user uses probably length, target depth counted as the total character generated left in that has to be generated before stopping.
        
        int len = rule.size();
        int target_depth = (num_max_depth - num_min_depth);
        if (params_prob_length > 0){
            target_depth = 0;
        }
        int generated = 0;
        int split = 0;
        int tmp_depth = depth;
        if (params_prob_length > 0){

            for (char c: rule){
                if (!exists(c, nonterminal) && !exists(c, terminal_str) && !exists(c, terminal_Randomizer))
                    generated++;
                else if(exists(c, nonterminal)){
                    split++;
                } else {
                    //TODO: terminal length approximation
                    generated++;
                }
            }
        }
        if (DEBUG && tmp_depth == 19 && rule.size() > 1)
            cerr<<" wooow"<<endl;
        for (int i=0;i<len;i++){
            
            int next_depth = depth - 1;
            if (params_prob_length > 0){
                depth = tmp_depth - generated;
                next_depth = depth;
                if (split > 1 && next_depth > 0){
                    next_depth = rand_int(next_depth);
                }
                if (next_depth <= 2)
                    depth = 0;
            }
            if (DEBUG && tmp_depth == 19 && rule.size() > 1)
                cerr<<"===== "<<depth<<" "<<generated<<" assign: "<<next_depth<<endl;

            int size_nonterminal = 0;
            int size_terminal = 0;
            if (exists(rule[i], nonterminal) )
                size_nonterminal += nonterminal[rule[i]].size();
            
            if (exists(rule[i], terminal_str) )
                size_terminal += terminal_str[rule[i]].size();
            
            if (exists(rule[i], terminal_Randomizer) )
                size_terminal += terminal_Randomizer[rule[i]].size();

            int modulo = size_nonterminal + size_terminal;
            
            if (modulo == 0){
                result.push_back(rule[i]);
                continue;
            }

            int chooser = rand_int(modulo);

            if (size_terminal == 0 ||  //if nonterminal is the only option
                (depth > target_depth && size_nonterminal > 0) ||  //if you still need to reach minimum depth, and is possible to expand nonterminal
                ( depth >  0 && depth <= target_depth && chooser < size_nonterminal )  ){ //or randomly selected to do so
                int idx = rand_int(size_nonterminal);
                generated += traverse_cfg( nonterminal[rule[i]][idx] , next_depth );
                split--;
                continue;
            }
            
            chooser -= size_nonterminal;

            if (size_nonterminal == 0 ||  //if terminal is the only option
                (depth <= 0 && size_terminal > 0 ) ||  //if max depth is exedeed.
                ( depth > 0 && depth <= target_depth && chooser < size_nonterminal ) ){ //selected randomly
                int idx = rand_int(size_terminal);
                split--;
                if (idx < terminal_str[rule[i]].size() ){
                    generated += traverse_cfg( terminal_str[rule[i]][idx] , next_depth );
                }
                else
                {
                    string generated_str = terminal_Randomizer[rule[i]][idx - terminal_str[rule[i]].size() ]->next();
                    generated += generated_str.size();
                    for (auto c: generated_str){
                        result.push_back(c);
                    }
                }
            }
        }
        return generated;
    }


public:

    CfgStringRandomizer(){
        params_min_depth = 1;
        params_max_depth = 5;    
        params_prob_length = -1;
    }

    CfgStringRandomizer& max_length(int n){
        params_prob_length = n;
        return *this;
    }


    CfgStringRandomizer& rule(char s, string to){
        all_cfg[s].push_back(to);
        return *this;
    }

    CfgStringRandomizer& rule(char s, StringRandomizer& to){
        terminal_Randomizer[s].push_back(&to);
        return *this;
    }


    CfgStringRandomizer& start_with(string start){
        params_start = start;
        return *this;
    }

    string next(){
        if (params_start.size() == 0){
            return "";
        }
        load_params();
        if (params_prob_length <= 0)
            traverse_cfg(params_start, params_max_depth);
        else
            do {
                result.clear();
                traverse_cfg(params_start, params_prob_length);
            } while(result.size() > params_prob_length);
        string s(result.begin(), result.end());
        return s;
    }
};

}