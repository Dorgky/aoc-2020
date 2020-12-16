#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct field_def_range_struct {
  int lower;
  int upper;
};

struct field_def_struct {
  string name;
  vector<field_def_range_struct> valid_ranges;
  vector<int> possible_indexes = {};
};

void load_field_definitions(vector<field_def_struct> *field_defs) {
  string next_line = "";
  getline(cin, next_line);

  while(next_line != "") {
    int key_value_del = next_line.find(":");
    string field_name = next_line.substr(0, key_value_del);
    next_line = next_line.substr(key_value_del + 2);

    field_def_struct field_def = {field_name, {}};
    do {
      key_value_del = next_line.find(" or ");
      string field_range = next_line.substr(0, key_value_del);
      if(key_value_del != string::npos) {
        next_line = next_line.substr(key_value_del + 4);
      }

      int range_del = field_range.find("-");
      int lower_value = stoi(field_range.substr(0, range_del));
      int upper_value = stoi(field_range.substr(range_del + 1));
      field_def.valid_ranges.push_back({lower_value, upper_value});
    } while(key_value_del != string::npos);

    field_defs->push_back(field_def);
    getline(cin, next_line);
  }
}

void load_my_ticket(vector<int> *my_ticket) {
  string next_line = "";
  getline(cin, next_line);

  while(next_line != "") {
    if(next_line != "your ticket:") {
      int delimiter = next_line.find(",");
      do {
        delimiter = next_line.find(",");
        my_ticket->push_back(stoi(next_line.substr(0, delimiter)));
        if(delimiter != string::npos) {
          next_line = next_line.substr(delimiter + 1);
        }
      } while(delimiter != string::npos);
    } 
    getline(cin, next_line);
  }
}

void load_other_tickets(vector<vector<int>> *tickets) {
  string next_line = "";
  getline(cin, next_line);

  while(next_line != "") {
    if(next_line != "nearby tickets:") {
      int delimiter = next_line.find(",");
      vector<int> current_ticket = {};
      do {
        delimiter = next_line.find(",");
        current_ticket.push_back(stoi(next_line.substr(0, delimiter)));
        if(delimiter != string::npos) {
          next_line = next_line.substr(delimiter + 1);
        }
      } while(delimiter != string::npos);
      tickets->push_back(current_ticket);
    } 
    getline(cin, next_line);
  }
}

int find_scanning_error(vector<field_def_struct> *field_defs, vector<vector<int>> *tickets) {
  int error = 0;
  vector<vector<int>> valid_tickets = {};

  for(auto& ticket : *tickets) {
    bool valid_ticket = true;
    for(auto& field_value : ticket) {
      bool valid_field = false;
      for(auto& field_def : *field_defs) {
        for(auto& field_range : field_def.valid_ranges) {
          if(field_value >= field_range.lower && field_value <= field_range.upper) {
            valid_field = true;
            break;
          }
        } 
        if(valid_field) break;
      }
      if(!valid_field) {
        error += field_value;
        valid_ticket = false;
      }
    }
    if(valid_ticket) {
      valid_tickets.push_back(ticket);
    }
  }

  *tickets = valid_tickets;

  return error;
}

bool fields_have_multiple_indexes(vector<field_def_struct> *field_defs) {
  for(auto& field_def : *field_defs) {
    if(field_def.possible_indexes.size() > 1){
      return true;
    }
  }
  return false;
}

void remove_index_from_other_fields(vector<field_def_struct> *field_defs, field_def_struct *skip_field) {
  for(auto& field_def : *field_defs) {
    if(field_def.name != skip_field->name) {
      auto index_ref = find(field_def.possible_indexes.begin(), field_def.possible_indexes.end(), skip_field->possible_indexes[0]);
      if(index_ref != field_def.possible_indexes.end()) {
        field_def.possible_indexes.erase(index_ref);
      }
    }
  }
}

void deduce_field_indexes(vector<field_def_struct> *field_defs, vector<vector<int>> *tickets) {
  for(auto& field_def : *field_defs) {
    int index = 0;
    field_def.possible_indexes.resize(tickets->at(0).size());
    generate(field_def.possible_indexes.begin(), field_def.possible_indexes.end(), [&index](){return index++;});
  }

  for(auto& ticket : *tickets) {
    for(int i = 0; i < ticket.size(); i++) {
      for(auto& field_def : *field_defs) {
        bool field_match = false;
        for(auto& field_range : field_def.valid_ranges) {
          if(ticket[i] >= field_range.lower && ticket[i] <= field_range.upper) {
            field_match = true;
          }
        }
        if(!field_match) {
          auto index_ref = find(field_def.possible_indexes.begin(), field_def.possible_indexes.end(), i);
          if(index_ref != field_def.possible_indexes.end()) {
            field_def.possible_indexes.erase(index_ref);
          }
        }
      }
    }
  }

  while(fields_have_multiple_indexes(field_defs)) {
    for(auto& field_def : *field_defs) {
      if(field_def.possible_indexes.size() == 1) {
        remove_index_from_other_fields(field_defs, &field_def);
      }
    }
  }
}

int main() {
  cout << "Loading field defs..." << endl;
  vector<field_def_struct> field_defs;
  load_field_definitions(&field_defs);

  cout << "Loading my ticket..." << endl;
  vector<int> my_ticket = {};
  load_my_ticket(&my_ticket);

  cout << "Loading other tickets..." << endl;
  vector<vector<int>> other_tickets = {};
  load_other_tickets(&other_tickets);
  cout << "Processed [" << other_tickets.size() << "] tickets" << endl;

  cout << "[Part 1] Calculating scanning error... ";
  cout << "[" << find_scanning_error(&field_defs, &other_tickets) << "]" << endl;
  cout << "[" << other_tickets.size() << "] valid tickets remaining" << endl;

  cout << "Deducing field indexes..." << endl;
  deduce_field_indexes(&field_defs, &other_tickets);

  cout << "[Part 2] Calculating departure fields... ";
  long result = 1;
  for(auto& field_def : field_defs) {
    if(field_def.name.find("departure") != string::npos) {
      result *= my_ticket[field_def.possible_indexes[0]];
    }
  }
  cout << "[" << result << "]" << endl;
}
