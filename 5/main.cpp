#include <iostream>
#include <vector>

using namespace std;

const int rows = 128;
const int columns = 8;

int get_midpoint(int min, int max) {
  return ((max - min) / 2) + min;
}

bool seat_map[1024] = {};

void init_seat_map() {
  for(int i = 0; i < 1024; i++) {
    seat_map[i] = false;
  }
}

int get_missing_seat_id() {
  for(int i = 1; i < 1023; i++) {
    if(!seat_map[i] && seat_map[i-1] && seat_map[i+1]) {
      return i;
    }
  }
  return -1;
}

int get_row_id(string boarding_pass) {
  int min_row = 0, max_row = rows;
  int min_col = 0, max_col = columns;
  for(char c : boarding_pass) {
    if (c == 'F') {
      max_row = get_midpoint(min_row, max_row);
    } else if (c == 'B') {
      min_row = get_midpoint(min_row, max_row);
    } else if (c == 'L') {
      max_col = get_midpoint(min_col, max_col);
    } else if (c == 'R') {
      min_col = get_midpoint(min_col, max_col);
    }
  }

  int seat_id = (min_row * 8) + min_col;
  seat_map[seat_id] = true;

  return seat_id;
}

int main() {
  cout << "=======================================" << endl;
  cout << "Samples" << endl;
  cout << "=======================================" << endl;
  vector<string> boarding_passes = {"BFFFBBFRRR", "FFFBBBFRRR", "BBFFBBFRLL"};

  for(string boarding_pass : boarding_passes) {
    cout << get_row_id(boarding_pass) << endl;
  }
  cout << endl;
  cout << "=======================================" << endl;
  cout << "Inputs" << endl;
  cout << "=======================================" << endl;

  string boarding_pass = "";
  int max_id = -1;
  init_seat_map();
  while(getline(cin,boarding_pass)) {
    int next_id = get_row_id(boarding_pass);
    if(next_id > max_id) max_id = next_id;
  }
  cout << "Max Seat ID: " << max_id << endl;
  cout << "My likely seat: " << get_missing_seat_id() << endl;
}
