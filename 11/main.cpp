#include <iostream>
#include <vector>

using namespace std;

void load_data(vector<vector<int>> *seat_map) {
  const char SEAT = 'L';
  const char FLOOR = '.';

  string seat_row = "";
  while(cin >> seat_row) {
    vector<int> row = {};
    for(char seat : seat_row) {
      if(seat == SEAT) {
        row.push_back(0);
      } else if(seat == FLOOR) {
        row.push_back(-1);
      } else {
        cout << "Unexpected input: " << seat << " from " << seat_row << "!!" << endl;
        exit(1);
      }
    }
    seat_map->push_back(row);
  }
}

void print_seat_map(vector<vector<int>> *seat_map) {
  char map[3] = {'.', 'L', '#'};

  cout << endl << "===========================" << endl;
  for(int i = 0; i < seat_map->size(); i++) {
    for(int j = 0; j < seat_map->at(i).size(); j++) {
      cout << map[seat_map->at(i)[j] + 1];
    }
    cout << endl;
  }
  cout << "===========================" << endl;
}

int count_surrounding_seats(vector<vector<int>> *seat_map, int row, int col) {
  int seat_count = 0;

  int i = row - 1;
  if(i < 0) i = 0;

  for (i; i < seat_map->size() && i <= row + 1; i++) {
    int j = col - 1;
    if(j < 0) j = 0;
    for(j; j < seat_map->at(i).size() && j <= col + 1; j++) {
      if(i == row && j == col) continue;
      if(seat_map->at(i)[j] == 1) {
        seat_count++;
      }
    }
  }

  return seat_count;
}

void trace_direction(vector<vector<int>> *seat_map, int row, int col, int *direction_map, int direction_index, int *count) {
  if(row >= 0 && row < seat_map->size()
      && col >= 0 && col < seat_map->at(row).size() 
      && (*direction_map & (1 << direction_index)) == 0
      && seat_map->at(row)[col] != -1) {
    *direction_map |= 1 << direction_index;
    (*count) += seat_map->at(row)[col];
  } 
}

int count_visible_surrounding_seats(vector<vector<int>> *seat_map, int row, int col) {
  int seat_count = 0;
  int seat_direction = 0;

  for(int i = 1 ; i < seat_map->size() && seat_direction != ((1 << 8) - 1); i++) {
    trace_direction(seat_map, row - i, col, &seat_direction, 0, &seat_count);
    trace_direction(seat_map, row + i, col, &seat_direction, 1, &seat_count);
    trace_direction(seat_map, row - i, col - i, &seat_direction, 2, &seat_count);
    trace_direction(seat_map, row - i, col + i, &seat_direction, 3, &seat_count);
    trace_direction(seat_map, row + i, col + i, &seat_direction, 4, &seat_count);
    trace_direction(seat_map, row + i, col - i, &seat_direction, 5, &seat_count);
    trace_direction(seat_map, row, col + i, &seat_direction, 6, &seat_count);
    trace_direction(seat_map, row, col - i, &seat_direction, 7, &seat_count);
  }

  return seat_count;
}

bool process_generation(vector<vector<int>> *seat_map) {
  bool seat_changed = false;
  vector<vector<int>> next_generation = {};

  for(int i = 0; i < seat_map->size(); i++) {
    auto row = seat_map->at(i);
    for(int j = 0; j < seat_map->at(i).size(); j++) {
      if(seat_map->at(i)[j] == 0 
          && count_visible_surrounding_seats(seat_map, i, j) == 0) {
        row[j] = 1; 
        seat_changed = true;
      } else if(seat_map->at(i)[j] == 1
          && count_visible_surrounding_seats(seat_map, i, j) >= 5) {
        row[j] = 0;
        seat_changed = true;
      } 
    }
    next_generation.push_back(row);
  }

  for(int i = 0; i < seat_map->size(); i++) {
    seat_map->at(i) = next_generation[i];
  }

  return seat_changed;
}

int count_all_occupied_seats(vector<vector<int>> *seat_map) {
  int seat_count = 0;

  for(int i = 0; i < seat_map->size(); i++) {
    for(int j = 0; j < seat_map->at(i).size(); j++) {
      if(seat_map->at(i)[j] == 1) {
        seat_count++;
      }
    }
  }

  return seat_count;
}

int main() {
  vector<vector<int>> seat_map = {};

  cout << "Loading data..." << endl;
  load_data(&seat_map); 

  //print_seat_map(&seat_map);

  cout << "Running simulation";
  while(process_generation(&seat_map)) {
    cout << ".";
    //print_seat_map(&seat_map);
  } 
  cout << endl;

  cout << "Occupied seats: " << count_all_occupied_seats(&seat_map) << endl; 

}
