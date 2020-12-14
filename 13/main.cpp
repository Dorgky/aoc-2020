#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

long lowest_multiple_greater_than(long min_multiple, long interval) {
  float multiple = round(min_multiple / interval) + 1;
  return multiple * interval;
}

struct congruence_struct {
  long prime;
  long offset;
};

long solve_congruences(vector<congruence_struct> *congruences) {
  long modulus = 1;
  long solution = 0;
  for(auto congruence : *congruences) {
    modulus *= congruence.prime;
  }

  for(auto congruence : *congruences) {
    long multiplier = modulus / congruence.prime;
    long x = 1;
    while(multiplier * x % congruence.prime != 1) {
      x++;
    }
    solution += (multiplier * x * congruence.offset);
  }

  solution %= modulus;
  if(solution < 0) solution += modulus;
  return solution;
}

int main() {
  long lowest_timestamp = -1;
  long next_value = -1;

  cin >> lowest_timestamp;
  cout << "Min mulitple: " << lowest_timestamp << endl;

  string str_schedule = "";
  cin >> str_schedule;
  int next_del = 0;
  cout << "Schedule: " << str_schedule << endl;
  int lowest_interval = -1;
  int bus_id = -1;
  int counter = 0;
  vector<congruence_struct> congruences = {};
  while(next_del != string::npos) {
    next_del = str_schedule.find(',');
    string str_next_value = str_schedule.substr(0, next_del);
    str_schedule = str_schedule.substr(next_del + 1);
    if(str_next_value != "x") { 
      next_value = stoi(str_next_value);
      congruences.push_back({next_value, -1*counter});
      if(lowest_interval == -1 || lowest_interval > lowest_multiple_greater_than(lowest_timestamp, next_value)) {
        lowest_interval = lowest_multiple_greater_than(lowest_timestamp, next_value);
        bus_id = next_value;
      }
    } 
    counter++;
  }

  cout << bus_id << ":" << lowest_interval << ":" << lowest_timestamp << endl;
  cout << bus_id * (lowest_interval - lowest_timestamp) << endl;
  cout << "Result: " << solve_congruences(&congruences) << endl;
}
