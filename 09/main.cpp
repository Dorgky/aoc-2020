#include <iostream>
#include <vector>
#include <algorithm>

using namespace std; 

const int PREAMBLE_SIZE = 5;
const long EXCEPTIONAL_NUMBER = 127;

void get_exceptional_number() {
  long preamble[PREAMBLE_SIZE] = {};
  int preamble_rotation = 0;
  bool preamble_done = false;

  long next_value = 0;
  while(cin >> next_value) {
    if(preamble_rotation < PREAMBLE_SIZE && !preamble_done) {
      preamble[preamble_rotation] = next_value;
      preamble_rotation++;
      if(preamble_rotation == PREAMBLE_SIZE) preamble_done = true;
      continue;
    }

    bool found_match = false;
    for(int i = 0; i < PREAMBLE_SIZE && !found_match; i++) {
      for(int j = i+1; j < PREAMBLE_SIZE && !found_match; j++) {
        if(preamble[i] + preamble[j] == next_value) {
          found_match = true;
        }
      }
    }

    if(!found_match) {
      cout << next_value << endl;
      break;
    } else {
      preamble[preamble_rotation % PREAMBLE_SIZE] = next_value;
      preamble_rotation++;  
    }
  }
}

void get_exceptional_seq() {
  vector<long> inputs = {};

  long next_value = 0;
  long test_value = 0;
  while(cin >> next_value) {
    inputs.push_back(next_value);
    
    test_value += next_value;
    while(test_value > EXCEPTIONAL_NUMBER) {
      test_value -= inputs[0];
      inputs.erase(inputs.begin());
    }

    if(test_value == EXCEPTIONAL_NUMBER) {
      sort(inputs.begin(), inputs.end());
      cout << inputs.front() + inputs.back() << endl;
      break;
    }
  }
}

int main() {
  //get_exceptional_number(); 
  get_exceptional_seq();
}
