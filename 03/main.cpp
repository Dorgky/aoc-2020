#include <iostream>
#include <vector>

using namespace std;

struct slope {
  int right;
  int down;
};

int main () {
  vector<vector<bool>> map = {};
  string row = "";

  while(cin >> row) {
    vector<bool> rowData = {};
    for(int i = 0; i < row.size(); i++) {
      if(row[i] == '.') {
        rowData.push_back(false);
      } else {
        rowData.push_back(true);
      }
    }
    map.push_back(rowData);
  }

  vector<slope> slopes = {{1,1},{3,1},{5,1},{7,1},{1,2}}; 
  long answer = 1;

  for(int i = 0; i < slopes.size(); i++) {
    int right = slopes[i].right;
    int down = slopes[i].down;

    int walkRow = 0;
    int walkCol = 0;
    long trees = 0;

    while(walkRow + down < map.size()) {
      walkRow += down;
      walkCol += right;
      if(map[walkRow][walkCol % map[walkRow].size()]) {
        trees++;  
      }
    }

    answer *= trees;
  }

  cout << "Answer: " << answer << endl;
}
