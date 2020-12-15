#include <iostream>
#include <map>
#include <vector> 

using namespace std;

int main() {
  string next_line = "";
  map<long, long> memory = {};
  string current_mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
  map<long, long> memory_v2 = {};

  while(getline(cin, next_line)) {
    int key_val_del = next_line.find("=");
    string key = next_line.substr(0, key_val_del - 1);
    string value = next_line.substr(key_val_del + 2);

    if(key == "mask") {
      current_mask = value;
    } else {
      int memory_index_del = key.find("[");
      long memory_index = stol(key.substr(memory_index_del + 1)); 
      long actual_value = stol(value);

      // V1
      for(int i = current_mask.length() - 1; i >= 0; i--) {
        long shift = current_mask.length() - 1 - i;
        long bit = 1L;
        if(current_mask[i] == '1') {
          actual_value |= bit << shift;
        } else if(current_mask[i] == '0') {
          actual_value |= bit << shift;
          actual_value -= bit << shift;
        }
      }

      auto existing_memory = memory.find(memory_index);
      if(existing_memory != memory.end()) memory.erase(memory_index);
      memory.insert({memory_index, actual_value});

      // V2
      actual_value = stol(value);
      vector<string> masked_memory_locations = {};
      vector<string> masked_memory_locations_copy = {};
      for(int i = 0; i < current_mask.length(); i++) {
        short new_bits = 0;
        long shift = current_mask.length() - 1 - i;
        long bit = 1L;
        if(current_mask[i] == '1') {
          new_bits = 1;
        } else if(current_mask[i] == '0') {
          if(memory_index & (bit << shift)) 
            new_bits = 1;
        } else {
          new_bits = 2;
        }

        if(masked_memory_locations.size() == 0) masked_memory_locations.push_back("");

        for(int i = 0; i < masked_memory_locations.size(); i++) {
          if(new_bits == 0) {
            masked_memory_locations_copy.push_back(masked_memory_locations[i] + "0");
          } else if(new_bits == 1) {
            masked_memory_locations_copy.push_back(masked_memory_locations[i] + "1");
          } else if (new_bits == 2) {
            masked_memory_locations_copy.push_back(masked_memory_locations[i] + "0");
            masked_memory_locations_copy.push_back(masked_memory_locations[i] + "1");
          }
        }

        masked_memory_locations = masked_memory_locations_copy;
        masked_memory_locations_copy = {};
      }

      for(string masked_memory_location : masked_memory_locations) {
        long masked_memory = 0L;
        for(int i = 0; i < masked_memory_location.length(); i++) {
          long bit = stol(masked_memory_location.substr(i,1));
          long shift = current_mask.length() - 1 - i;
          masked_memory |= (bit << shift);
        }

        if(memory_v2.find(masked_memory) != memory_v2.end()) memory_v2.erase(masked_memory);
        memory_v2.insert({masked_memory, actual_value});
      }
    }
  }

  long result = 0;
  for(auto memory_location = memory.begin(); memory_location != memory.end(); memory_location++) {
    result += memory_location->second;
  }
  cout << result << endl;

  result = 0;
  for(auto memory_location = memory_v2.begin(); memory_location != memory_v2.end(); memory_location++) {
    result += memory_location->second;
  }
  cout << result << endl;
}
