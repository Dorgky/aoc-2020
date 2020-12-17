#include <iostream>
#include <vector>

using namespace std;

const int MAX_DIMENSION = 50; // Just fits on the stack with room for the grid...
const int MAX_CYCLES = 6;
const char ON = '#';
const char OFF = '.';

struct change_struct {
  int x, y, z, w;
  bool active;
};

void load_data(uint8_t array[MAX_DIMENSION][MAX_DIMENSION][MAX_DIMENSION][MAX_DIMENSION]) {
  string next_line = "";
  const int starting_z = MAX_DIMENSION/2;
  cout << "Loading intial state..." << endl;
  int starting_y = -1;
  int y = -1;
  while(getline(cin, next_line)) {
    if(y == -1) {
      y = MAX_DIMENSION/2 - (next_line.length()/2);
      starting_y = y;
    }
    int x = starting_y;
    for(int i = 0; i < next_line.length(); i++) {
      if(next_line[i] == ON) {
        array[x][y][starting_z][starting_z] = 1;
      }
      x++;
    }
    y++;
  }
  cout << "State loaded." << endl;
}

int count_active_neighbors(uint8_t array[MAX_DIMENSION][MAX_DIMENSION][MAX_DIMENSION][MAX_DIMENSION], int sx, int sy, int sz, int sw) {
  int active_neighbors = 0;
  for(int x = sx - 1; x <= sx + 1; x++) {
    for(int y = sy - 1; y <= sy + 1; y++) {
      for(int z = sz - 1; z <= sz + 1; z++) {
        for(int w = sw - 1; w <= sw + 1; w++) {
          if(x < 0 || y < 0 || z < 0 || w < 0) {
            continue;
          }
          else if(x >= MAX_DIMENSION || y >= MAX_DIMENSION || z >= MAX_DIMENSION || w >= MAX_DIMENSION) {
            continue;
          }
          else if(x == sx && y == sy && z == sz && w == sw) {
            continue;
          }
          else if(array[x][y][z][w] == 1) {
            active_neighbors++;
          }
        }
      }
    }
  } 
  return active_neighbors;
}

int main() {
  cout << "Initialiazing spacial array..." << endl;
  uint8_t spacial_array[MAX_DIMENSION][MAX_DIMENSION][MAX_DIMENSION][MAX_DIMENSION] = {};

  load_data(spacial_array);

  cout << "Running [" << MAX_CYCLES << "] cycles..." << endl;
  vector<change_struct> changes = {};
  for(int cycle = 0; cycle < MAX_CYCLES; cycle++) {
    for(int x = 0; x < MAX_DIMENSION; x++) {
      for(int y = 0; y < MAX_DIMENSION; y++) {
        for(int z = 0; z < MAX_DIMENSION; z++) {
          for(int w = 0; w < MAX_DIMENSION; w++) {
            int active_neighbors = count_active_neighbors(spacial_array, x, y, z, w);
            if(spacial_array[x][y][z][w] == 1 && (active_neighbors < 2 || active_neighbors > 3)) {
              changes.push_back({x,y,z,w,false});
            } else if(spacial_array[x][y][z][w] == 0 && active_neighbors == 3) {
              changes.push_back({x,y,z,w,true});
            } 
          }
        }
      }
    }

    for(auto change : changes) {
      spacial_array[change.x][change.y][change.z][change.w] = change.active;
    }
    changes.clear();
  }

  cout << "Counting active cubes... ";
  int active_cubes = 0;
  for(int x = 0; x < MAX_DIMENSION; x++) {
    for(int y = 0; y < MAX_DIMENSION; y++) {
      for(int z = 0; z < MAX_DIMENSION; z++) {
        for(int w = 0; w < MAX_DIMENSION; w++) {
          if(spacial_array[x][y][z][w] == 1) {
            active_cubes++;
          }
        }
      }
    }
  }
  cout << "[" << active_cubes << "]" << endl;
}
