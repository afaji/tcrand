#include <algorithm>
#include <stdexcept>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>
#include <map>
#include <regex>
#define DEBUG false
using namespace std;

class StringRandomizer{
	string result;
	int params_minlength;
	int params_maxlength;

	int num_length;

	vector<char> charset_;
	enum StringType { type_none, type_palindrome };
	StringType string_type;

	void load_params(){
		int len = params_minlength + rand() % (params_maxlength - params_minlength + 1);
		num_length = len;
	}


	string generate(int n){
		if (charset_.size() == 0)
			charset("a-z");

		string tmp = "";
		for (int i=0;i<n;i++){
			int idx = rand() % charset_.size();
			tmp += charset_[idx];
		}

		return tmp;
	}


public:
	StringRandomizer() {
		params_minlength = 1;
		params_maxlength = 20;
		string_type = type_none;
	}
	StringRandomizer& charset(regex e){
		for (int i=0;i<256;i++){
			string test = "";
			test += (char)i;
			if(regex_match(test, e))
				charset_.push_back(i);
		}
		return *this;
	}

	StringRandomizer& charset(string s){
		regex tmp("[" + s + "]");
		return charset(tmp);
	}

	StringRandomizer& length(int lo, int hi){
		params_minlength = lo;
		params_maxlength = hi;
		return *this;
	}

	StringRandomizer& length(int n){
		return length(n,n);
	}

	StringRandomizer& palindrome(){
		string_type = type_palindrome;
		return *this;
	}

	string next(){
		load_params();
		result = "";
		if (string_type == type_palindrome){
			string tmp = generate(num_length/2);
			if (num_length % 2)
				result += generate(1);
			string copy(tmp);
			reverse(copy.begin(), copy.end());
			result = tmp + result + copy;
		} else {
			result = generate(num_length);
		}
		return result;
	}

};


class GrammarRandomizer{
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
					next_depth = rand() % next_depth;
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

			int chooser = rand() % modulo;

			if (size_terminal == 0 ||  //if nonterminal is the only option
				(depth > target_depth && size_nonterminal > 0) ||  //if you still need to reach minimum depth, and is possible to expand nonterminal
				( depth >  0 && depth <= target_depth && chooser < size_nonterminal )  ){ //or randomly selected to do so
				int idx = rand() % size_nonterminal;
				generated += traverse_cfg( nonterminal[rule[i]][idx] , next_depth );
				split--;
				continue;
			}
			
			chooser -= size_nonterminal;

			if (size_nonterminal == 0 ||  //if terminal is the only option
				(depth <= 0 && size_terminal > 0 ) ||  //if max depth is exedeed.
				( depth > 0 && depth <= target_depth && chooser < size_nonterminal ) ){ //selected randomly
				int idx = rand() % size_terminal;
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

	GrammarRandomizer(){
		params_min_depth = 1;
		params_max_depth = 5;	
		params_prob_length = -1;
	}

	GrammarRandomizer& length_around(int n){
		params_prob_length = n;
		return *this;
	}


	GrammarRandomizer& grammar(char s, string to){
		all_cfg[s].push_back(to);
		return *this;
	}

	GrammarRandomizer& grammar(char s, StringRandomizer& to){
		terminal_Randomizer[s].push_back(&to);
		return *this;
	}


	GrammarRandomizer& start(string start){
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
			traverse_cfg(params_start, params_prob_length);
		string s(result.begin(), result.end());
		return s;
	}
};



int main(){
	StringRandomizer num, operand;
	num.length(1,3).charset("0-9");
	operand.length(1).charset("*+/-");

	GrammarRandomizer cfg;
	cfg.grammar('F',"(F O F)");
	cfg.grammar('O', operand );
	cfg.grammar('F', num);
	cfg.length_around(42);
	int x =0;
	for (int i=0;i<100;i++)
		x+=cfg.start("F").next().size();
	cout<<x/100<<endl;

	GrammarRandomizer cfg2;
	cfg2.grammar('X',"");
	cfg2.grammar('X', "XX");
	cfg2.grammar('X', "(X)");
	for (int i=0;i<10;i++){
		cfg2.length_around(i*2 + 10);
		cout<<cfg2.start("X").next()<<endl;
	}


}