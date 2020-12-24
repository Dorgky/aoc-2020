#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

bool check_space(unordered_map<string, bool> &flipped_tiles, int x, int y) {
    string location = to_string(x) + ":" + to_string(y);
    if(flipped_tiles[location]) {
      return true;
    }
    else {
      flipped_tiles.erase(location);
      return false;
    }
}

int get_neighbors(unordered_map<string, bool> &flipped_tiles, int x, int y) {
  int flipped_neighbors = 0;
  if(check_space(flipped_tiles, x, y-1)) flipped_neighbors++;
  if(check_space(flipped_tiles, x+1, y-1)) flipped_neighbors++;
  if(check_space(flipped_tiles, x-1, y+1)) flipped_neighbors++;
  if(check_space(flipped_tiles, x, y+1)) flipped_neighbors++;
  if(check_space(flipped_tiles, x+1, y)) flipped_neighbors++;
  if(check_space(flipped_tiles, x-1, y)) flipped_neighbors++;
  return flipped_neighbors;
}

void try_flip_neighbor_tile(unordered_map<string, bool> &flipped_tiles, unordered_set<string> &tiles_to_flip, int x, int y) {
  string location = to_string(x) + ":" + to_string(y);
  if(!check_space(flipped_tiles, x, y) 
      && tiles_to_flip.count(location) == 0) {
    if(get_neighbors(flipped_tiles, x, y) == 2) {
      tiles_to_flip.insert(location);
    }
  }
}

int main() {
  unordered_map<string, bool> flipped_tiles = {};
  string next_line = "";
  while(getline(cin, next_line)) {
    int x = 0, y = 0;
    while(next_line.length() > 0) {
      string op = "";
      if(next_line.length() > 1) {
        op = next_line.substr(0,2);
      } else { 
        op = next_line.substr(0,1);
      }

      if (op == "nw") {
        y--;
      } else if (op == "ne") {
        y--;
        x++;
      } else if (op == "sw") {
        y++;
        x--;
      } else if (op == "se") {
        y++;
      } else {
        op = op.substr(0,1);
        if (op == "e") {
          x++;
        } else if (op == "w") {
          x--;
        } else {
          cout << "Something went wrong: [" << op << "]" << endl;
        }     
      }

      next_line = next_line.substr(op.length());
    }
    string location = to_string(x) + ":" + to_string(y);
    if(flipped_tiles[location]) {
      flipped_tiles.erase(location);
    } else {
      flipped_tiles[location] = true;
    }
  }
  cout << "Black tiles part 1: [" << flipped_tiles.size() << "]" << endl;

  const int flip_cycles = 100;
  unordered_set<string> tiles_to_flip = {};
  for(int i = 0; i < flip_cycles; i++) {
    for(auto& tile : flipped_tiles) {
      int del_pos = tile.first.find(":");
      int x = stoi(tile.first.substr(0,del_pos));
      int y = stoi(tile.first.substr(del_pos+1));

      int flipped_neighbors = get_neighbors(flipped_tiles, x, y);
      if(flipped_neighbors == 0 || flipped_neighbors > 2) {
        string location = to_string(x) + ":" + to_string(y);
        tiles_to_flip.insert(location);
      }

      try_flip_neighbor_tile(flipped_tiles, tiles_to_flip, x, y-1);
      try_flip_neighbor_tile(flipped_tiles, tiles_to_flip, x+1, y-1);
      try_flip_neighbor_tile(flipped_tiles, tiles_to_flip, x-1, y+1);
      try_flip_neighbor_tile(flipped_tiles, tiles_to_flip, x, y+1);
      try_flip_neighbor_tile(flipped_tiles, tiles_to_flip, x+1, y);
      try_flip_neighbor_tile(flipped_tiles, tiles_to_flip, x-1, y);
    } 

    for(auto& tile_to_flip : tiles_to_flip) {
      if(flipped_tiles[tile_to_flip]) {
        flipped_tiles.erase(tile_to_flip);
      } else {
        flipped_tiles[tile_to_flip] = true;
      }
    }
    tiles_to_flip.clear();
  }

  cout << "Black tiles part 2: [" << flipped_tiles.size() << "]" << endl;
}
