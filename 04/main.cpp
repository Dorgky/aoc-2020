#include <iostream>
#include <vector>

using namespace std;

bool is_blank(string str) {
  for(char c : str) {
    if(!isspace(c)) {
      return false;
    }
  }
  return true;
}

int find_key(vector<string> required_fields, string key) {
  for (int i = 0; i < required_fields.size(); i++) {
    if(required_fields[i] == key) return i;
  }
  return -1;
}

bool validate_pair(string key, string value) {
  if(key == "byr") {
    // 4 digits, 1920-2002
    if(stoi(value) >= 1920 && stoi(value) <= 2002) return true;
    return false;
  }
  if(key == "iyr") {
    // 4 digits 2010-2020
    if(stoi(value) >= 2010 && stoi(value) <= 2020) return true;
    return false;
  }
  if(key == "eyr") {
    //4 digits 2020-2030
    if(stoi(value) >= 2020 && stoi(value) <= 2030) return true;
    return false;
  }
  if(key == "hgt") {
    // number followed by in/cm
    // if in, 59-76
    // if cm, 150-193
    int height = stoi(value);
    string units = value.substr(to_string(height).size());
    if(units == "cm" && height >= 150 && height <= 193) return true;
    if(units == "in" && height >= 59 && height <= 76) return true;
    return false;
  }
  if(key == "hcl") {
    // hex color code #xxxxxx
    if(value.size() == 7 
        && value[0] == '#' 
        && value.find_first_not_of("0123456789abcdefABCDEF", 1) == string::npos) 
      return true;
    return false;

  }
  if(key == "ecl") {
    // exactly one of: amb blu brn gry grn hzl oth
    vector<string> valid_eyes = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    if(find_key(valid_eyes, value) != -1) return true;
    return false;
  }
  if(key == "pid") {
    // 9 digits including leading 0's
    if(value.size() == 9
        && value.find_first_not_of("0123456789") == string::npos) 
      return true;
    return false;
  }
  if(key == "cid") {
    // Not needed but just tossed in for completeness.
    return true;
  }
  return true;
}

int main() {
   vector<string> required_fields = {"byr","iyr","eyr","hgt","hcl","ecl","pid"};
   const int all_fields = (1 << required_fields.size()) -1;

   string next_line = "";
   int current_fields = 0;
   int valid_passports = 0;

   while(getline(cin, next_line)) {
      if(is_blank(next_line)) {
        if(current_fields == all_fields) valid_passports++;
        current_fields = 0;
      } else {
        int orig_pos = 0;
        int next_pos = 0;
        while(next_pos = next_line.find(" ", orig_pos)) {
          string pair = next_line.substr(orig_pos, next_pos-orig_pos);
          int pair_del = pair.find(":");
          string key = pair.substr(0, pair_del);
          string value = pair.substr(pair_del+1);
          bool value_valid = validate_pair(key, value);
          int key_match = find_key(required_fields, key);
          if (key_match != -1 && value_valid) {
            current_fields |= 1 << key_match;
          }
          orig_pos = next_pos + 1;
          if (next_pos == -1) break;
        }
      }
   }
   cout << valid_passports << endl;
}
