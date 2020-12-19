#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

const char NOVALUE = -1;

struct rule_struct {
  int number;
  char value; 
  vector<vector<int>> sub_rules; 

  rule_struct(int number_, char value_, vector<vector<int>> sub_rules_) {
    number = number_;
    value = value_;
    sub_rules = sub_rules_;
  }

  rule_struct() {
    number = -1;
    value = -1;
    sub_rules = {};
  };
};

rule_struct parse_rule(string line) {
  const string delim = " ";
  int delim_pos = line.find(delim);
  rule_struct rule = {stoi(line.substr(0, delim_pos)), NOVALUE, {}};

  line = line.substr(delim_pos + 1);
  delim_pos = 0;
  vector<int> rule_group = {};
  while(delim_pos != string::npos) {
    delim_pos = line.find(delim);  
    if(line[0] == '"') {
      rule.value = line[1];
      return rule;
    } else if (line[0] == '|'){
      rule.sub_rules.push_back(rule_group);
      rule_group.clear();
    } else {
      rule_group.push_back(stoi(line.substr(0, delim_pos)));
    }
    line = line.substr(delim_pos + 1);
  }
  if(rule_group.size() > 0) {
    rule.sub_rules.push_back(rule_group);
  }

  return rule;
}

vector<string> process_rule(vector<string> matches, unordered_map<int, rule_struct> &rules, int rule_num) {
  vector<string> possible_matches = {};

  for(auto& line : matches) {
    if(rules[rule_num].value != NOVALUE) {
      if(line.length() > 0 && line[0] == rules[rule_num].value) {
        if(line.length() == 1) {
          possible_matches.push_back("");
        } else {
          possible_matches.push_back(line.substr(1)); 
        }
      } 
    } else {
      for(auto& group : rules[rule_num].sub_rules) {
        vector<string> sub_rule_matches = {line};
        for(auto& rule : group) {
          sub_rule_matches = process_rule(sub_rule_matches, rules, rule);
        }
        for(auto& match : sub_rule_matches) {
          possible_matches.push_back(match);
        }
      }
    }
  }

  return possible_matches;
}

int main() {
  string next_line = "";
  unordered_map<int, rule_struct> rules = {};

  getline(cin, next_line);
  while(next_line != "") {
    rule_struct rule = parse_rule(next_line);
    rules[rule.number] = rule;
    getline(cin, next_line);
  }

  int match_counter = 0;
  while(getline(cin, next_line)) {
    bool matches = false;
    for(string& possible_result : process_rule({next_line}, rules, 0)) {
      if(possible_result == "") {
        matches = true;
        match_counter++;
      }
    }
    cout << "[" << next_line << "] : [" << matches << "]" << endl;
  }
  cout << "[Sum]: [" << match_counter << "]" << endl;
}
