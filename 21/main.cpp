#include <iostream>
#include <vector> 
#include <algorithm>
#include <unordered_map>

using namespace std;

struct allergen_struct {
  string name;
  vector<string> possible_ingredients = {};
};

int main() {
  unordered_map<string, int> ingredient_counts = {};
  unordered_map<string, allergen_struct> allergen_map = {};
  string next_line = "";
  while(getline(cin, next_line)) {
    int allergen_group_pos = next_line.find("(");
    vector<string> new_allergens = {};
    if(allergen_group_pos != string::npos) {
      string allergens = next_line.substr(allergen_group_pos + 1);
      allergens = allergens.substr(0, allergens.find(")"));
      int allergen_pos = 0;
      while(allergen_pos != string::npos) {
        allergen_pos = allergens.find(" ");
        string allergen_str = allergens.substr(0, allergen_pos);        
        allergens = allergens.substr(allergen_pos + 1);        
        int callergen_pos = allergen_str.find(",");
        if(callergen_pos != string::npos) {
          allergen_str = allergen_str.substr(0, callergen_pos);
        }
        if(allergen_str == "contains") {
          continue;
        }
        new_allergens.push_back(allergen_str);
      }
    } else {
      ;
    }
    next_line = next_line.substr(0, allergen_group_pos - 1);
    vector<string> new_ingredients = {};
    int ingredient_del_pos = 0;
    while(ingredient_del_pos != string::npos) {
      ingredient_del_pos = next_line.find(" ");
      string ingredient = next_line.substr(0, ingredient_del_pos);
      next_line = next_line.substr(ingredient_del_pos + 1);
      ingredient_counts[ingredient]++;
      new_ingredients.push_back(ingredient);
    }
  
    for(auto& allergen : new_allergens) {
      auto& allergen_entry = allergen_map[allergen];
      allergen_entry.name = allergen;
      if(allergen_entry.possible_ingredients.size() == 0) {
        for(auto& ingredient : new_ingredients) {
          allergen_entry.possible_ingredients.push_back(ingredient);
        }
      } else {
        vector<string> updated_possibles = {};
        for(auto& possible_ingredient : allergen_entry.possible_ingredients) {
          bool intersection = false;
          for(auto& ingredient : new_ingredients) {
            if(possible_ingredient == ingredient) intersection = true;
          }
          if(intersection) {
            updated_possibles.push_back(possible_ingredient);
          }
        }
        allergen_entry.possible_ingredients = updated_possibles;
      }
    }
  }

  bool list_updated = true;
  while(list_updated) {
    list_updated = false;
    for(auto& allergen_entry : allergen_map) {
      auto& allergen = allergen_entry.second;
      if(allergen.possible_ingredients.size() == 1) {
        string remove_str = allergen.possible_ingredients[0];
        for(auto& inner_allergen_entry : allergen_map) {
          auto& inner_allergen = inner_allergen_entry.second;
          if(inner_allergen.name == allergen.name) {
            continue;
          }
          auto matched_it = find(inner_allergen.possible_ingredients.begin(), inner_allergen.possible_ingredients.end(), remove_str);
          if(matched_it != inner_allergen.possible_ingredients.end()) {
            list_updated = true;
            inner_allergen.possible_ingredients.erase(matched_it);
          }      
        }
      }
    }
  }

  vector<string> allergen_ingredients = {};
  for(auto& allergen_entry : allergen_map) {
      auto& allergen = allergen_entry.second;
      cout << "Allergen: [" << allergen.name << "]" << endl;
      for(auto& possible_ingredient : allergen.possible_ingredients) {
        cout << "Possible Ingredient [" << possible_ingredient << "]" << endl;
        allergen_ingredients.push_back(allergen.name);
      }
  }
  sort(allergen_ingredients.begin(), allergen_ingredients.end());
  for(auto& allergen_ingredient : allergen_ingredients) {
    cout << allergen_map[allergen_ingredient].possible_ingredients[0] << ",";
  }
  cout << endl;

  int count = 0;
  for(auto& ingredient : ingredient_counts) {
    bool count_it = true;
    for(auto& allergen : allergen_map) {
      auto& allergen_data = allergen.second;
      for(auto& possible_allergen : allergen_data.possible_ingredients) {
        if(possible_allergen == ingredient.first) {
          count_it = false;
        }
      }
    }
    if(count_it) {
      count += ingredient.second;
    }
  }
  cout << count << endl;
}
