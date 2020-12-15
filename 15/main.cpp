#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
  unordered_map<long, long> memory = {};
  long turn_counter = 1;

  string starting_numbers = "";
  getline(cin, starting_numbers);

  int next_del = 0;
  long next_number = 0;
  long current_number = 0;
  while(next_del != string::npos) {
    next_del = starting_numbers.find(",");
    current_number = stol(starting_numbers.substr(0, next_del));
    starting_numbers = starting_numbers.substr(next_del + 1);

    auto& previous_instance = memory[current_number];
    if(previous_instance != 0L) {
      next_number = turn_counter - previous_instance;
    } else { 
      next_number = 0;
    }
    previous_instance = turn_counter++;
    current_number = next_number;
  }

  long turn_limit = 30000000;

  while(turn_counter < turn_limit) {
    auto& previous_instance = memory[current_number];
    if(previous_instance != 0L) {
      next_number = turn_counter - previous_instance;
    } else { 
      next_number = 0;
    }
    previous_instance = turn_counter++;
    current_number = next_number;
  }

  cout << current_number << endl;
}
