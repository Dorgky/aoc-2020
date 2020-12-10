#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long sum_col(vector<vector<long long>> *jolt_map, int row) {
  long long result = 0;
  for(int i = 0; i <= row; i++) {
    result += (*jolt_map)[i][row];
  }
  return result;
}

int main() {
  vector<int> adapters = {};

  int next_adapter = 0;
  while(cin >> next_adapter) {
    adapters.push_back(next_adapter);
  }

  sort(adapters.begin(), adapters.end());
  
  int jolts[4] = {};
  jolts[adapters[0]]++;
  for(int i = 1; i < adapters.size(); i++) {
    jolts[adapters[i]-adapters[i-1]]++;
  }
  jolts[3]++;

  cout << jolts[1] << " * " << jolts[3] << " = " << jolts[1] * jolts[3] << endl;

  vector<vector<long long>> jolt_map(adapters.size());
  for(int i = 0; i < jolt_map.size(); i++) {
    vector<long long> row(adapters.size());
    jolt_map[i] = row;
  }

  for(int i = 0; i < adapters.size(); i++) {
    for(int j = 0; j < adapters.size(); j++) {
      if(i == j && adapters[i] <= 3) {
        auto row = jolt_map[i];
        row[j] = 1;        
        jolt_map[i] = row;
      } else 
        if(adapters[j] - adapters[i] >= 1 
            && adapters[j] - adapters[i] <= 3) {
          auto sum = sum_col(&jolt_map, i);
          auto row = jolt_map[i];
          row[j] = sum;
          jolt_map[i] = row;
      }    
    }
  }

//  cout << "Table..." << endl;
//  for(int i = 0; i < adapters.size(); i++) {
//    cout << "\t" << adapters[i];
//  }
//  cout << endl;
//  for(int i = 0; i < jolt_map.size(); i++) {
//    for(int j = 0; j < jolt_map[i].size(); j++) {
//      cout << "\t" << jolt_map[i][j];
//    }
//    cout << endl;
//  }
//  cout << endl;
  
  long long result = 0;
  for(int i = 0; i < jolt_map.size(); i++) {
    result += jolt_map[i].back();
  }

  cout << "Permutations: " << result << endl;

}
