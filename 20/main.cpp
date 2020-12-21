#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace std;

struct tile_struct {
  int num = -1;
  vector<vector<char>> data = {};
  long top = -1, right = -1, bottom = -1, left = -1;
  bool matched = false;

  long hash_left() {
    if(left == -1) {
      string str = "";
      for(int i = 0; i < data.size(); i++) {
        str += string(1,data[i][0]);
      }
      left = hash<string>{}(str);
    }

    return left;
  }
 
  long hash_right() {
    if(right == -1) {
      string str = "";
      for(int i = 0; i < data.size(); i++) {
        str += string(1,data[i][data[i].size() - 1]);
      }
      right = hash<string>{}(str);
    }

    return right;
  }

  long hash_top() {
    if(top == -1) {
      string str = "";
      for(int i = 0; i < data[0].size(); i++) {
        str += string(1,data[0][i]);
      }
      top = hash<string>{}(str);
    }

    return top;
  }

  long hash_bottom() {
    if(bottom == -1) {
      string str = "";
      for(int i = 0; i < data[data.size()-1].size(); i++) {
        str += string(1,data[data.size()-1][i]);
      }
      bottom = hash<string>{}(str);
    }

    return bottom;
  }

};

void print_tile(tile_struct &tile) {
  cout << "[" << tile.num << "]" << endl;
  for(auto& line : tile.data) {
    for(auto& entry : line) {
      cout << string(1, entry);
    }
    cout << endl;
  }
  cout << endl;
}

void load_tiles(unordered_map<int, tile_struct> &tiles) {
  const string tile_num_del = "Tile ";
  string next_line = "";
  tile_struct tile = {};
  while(getline(cin, next_line)) {
    int del_pos = next_line.find(tile_num_del); 
    if(del_pos != string::npos) {
      tile.num = stoi(next_line.substr(tile_num_del.length()));
    } else if (next_line == "") {
      tiles[tile.num] = tile;
      tile = {};
    } else {
      vector<char> line = {};
      for(char c : next_line) {
        line.push_back(c);
      }
      tile.data.push_back(line);
    }
  }
}

void rotate_tile_cw(tile_struct &tile) {
  int tile_i = -1;
  int tile_j = -1;
  for(int i = 0; i < tile.data.size(); i++) {
    for(int j = i; j < tile.data[i].size() - i - 1; j++) {
      tile_i = tile.data.size() - 1 - i;
      tile_j = tile.data.size() - 1 - j;

      tile.data[i][j] ^= tile.data[j][tile_i];
      tile.data[j][tile_i] ^= tile.data[i][j];
      tile.data[i][j] ^= tile.data[j][tile_i];

      tile.data[i][j] ^= tile.data[tile_i][tile_j];
      tile.data[tile_i][tile_j] ^= tile.data[i][j];
      tile.data[i][j] ^= tile.data[tile_i][tile_j];

      tile.data[i][j] ^= tile.data[tile_j][i];
      tile.data[tile_j][i] ^= tile.data[i][j];
      tile.data[i][j] ^= tile.data[tile_j][i];
    }
  }
  tile.left = -1;
  tile.right = -1;
  tile.top = -1;
  tile.bottom = -1;      
}

void rotate_grid_cw(vector<vector<int>> &grid) {
  int grid_i = -1;
  int grid_j = -1;
  for(int i = 0; i < grid.size(); i++) {
    for(int j = i; j < grid[i].size() - i - 1; j++) {
      grid_i = grid.size() - 1 - i;
      grid_j = grid[i].size() - 1 - j;

      grid[i][j] ^= grid[j][grid_i];
      grid[j][grid_i] ^= grid[i][j];
      grid[i][j] ^= grid[j][grid_i];

      grid[i][j] ^= grid[grid_i][grid_j];
      grid[grid_i][grid_j] ^= grid[i][j];
      grid[i][j] ^= grid[grid_i][grid_j];

      grid[i][j] ^= grid[grid_j][i];
      grid[grid_j][i] ^= grid[i][j];
      grid[i][j] ^= grid[grid_j][i];
    }
  }
}

void flip_tile_vert(tile_struct &tile) {
  int tile_row = -1;
  for(int i = 0; i < tile.data.size() / 2; i++) {
    for(int j = 0; j < tile.data[i].size(); j++) {
      tile_row = tile.data.size() - 1 - i;
      tile.data[i][j] ^= tile.data[tile_row][j];
      tile.data[tile_row][j] ^= tile.data[i][j];
      tile.data[i][j] ^= tile.data[tile_row][j];
    }
  }
  tile.bottom = -1;
  tile.top = -1;
  tile.left = -1;
  tile.right = -1;
}

void flip_grid_vert(vector<vector<int>> &grid) {
  int grid_row = -1;
  for(int i = 0; i < grid.size() / 2; i++) {
    for(int j = 0; j < grid[i].size(); j++) {
      grid_row = grid.size() - 1 - i;
      grid[i][j] ^= grid[grid_row][j];
      grid[grid_row][j] ^= grid[i][j];
      grid[i][j] ^= grid[grid_row][j];
    }
  }
}

void find_possible_match(unordered_map<int, tile_struct> &tiles, vector<vector<int>> &tile_grid, int i, int j) {
  long left = -1, right = -1, top = -1, bottom = -1;
  int max_i = -1, max_j = -1;
  if(i == 0) {
    top = 0;
  } else {
    max_i = tile_grid.size() - 1;
    if(max_i >= i - 1) {
      max_j = tile_grid[i-1].size() - 1;
      if(max_j >= j) {
        top = tiles[tile_grid[i-1][j]].hash_bottom();
      }
    }
  }

  if(j == 0) {
    left = 0;
  } else {
    max_i = tile_grid.size() - 1;
    if(max_i >= i) {
      max_j = tile_grid[i].size() - 1;
      if(max_j >= j - 1) {
        left = tiles[tile_grid[i][j - 1]].hash_right();
      }
    }
  }

  max_i = tile_grid.size() - 1;
  if(max_i >= i + 1) {
    max_j = tile_grid[i + 1].size() - 1;
    if(max_j >= j) {
      bottom = tiles[tile_grid[i + 1][j]].hash_top();
    }
  }

  max_i = tile_grid.size() - 1;
  if(max_i >= i) {
    max_j = tile_grid[i].size() - 1;
    if(max_j >= j + 1) {
      right = tiles[tile_grid[i][j + 1]].hash_left();
    }
  }

  for(auto& tile_it : tiles) {
    auto& tile = tile_it.second;
    if(tile.matched) {
      continue;
    }

    bool top_match = top == 0, bottom_match = false, left_match = left == 0, right_match = false;

    if(top == 0 || left == 0) {
      for(int ok = 0; ok < 4; ok++) {
        top_match = top == 0;
        left_match = left == 0;
        for(auto& anti_tile_it : tiles) {
          auto& anti_tile = anti_tile_it.second;
          if(anti_tile.matched || anti_tile.num == tile.num) {
            continue;
          }
          for(int k = 0; k < 4; k++) {
            if(top == 0 && tile.hash_top() == anti_tile.hash_bottom()) {
              top_match = false;
            }
            if(left == 0 && tile.hash_left() == anti_tile.hash_right()) {
              left_match = false;
            }
            rotate_tile_cw(anti_tile);
          }
          flip_tile_vert(anti_tile);

          for(int k = 0; k < 4; k++) {
            if(top == 0 && tile.hash_top() == anti_tile.hash_bottom()) {
              top_match = false;
            }
            if(left == 0 && tile.hash_left() == anti_tile.hash_right()) {
              left_match = false;
            }
            rotate_tile_cw(anti_tile);
          }
        }
        if(((top == 0 && top_match) || top != 0) && ((left == 0 && left_match) || left != 0)) {
          break;
        }
        rotate_tile_cw(tile);
      }
    }

    for(int k = 0; k < 4; k++) {
      top_match = (top == 0) ? top_match : (top == tile.hash_top());
      right_match = right == tile.hash_right();
      bottom_match = bottom == tile.hash_bottom();
      left_match = (left == 0) ? left_match : (left == tile.hash_left());
      if((top_match || top == -1) &&
          (right_match || right == -1) && 
          (bottom_match || bottom == -1) &&
          (left_match || left == -1)) {
        if(tile_grid.size() == i) {
          tile_grid.push_back({});
        }
        tile_grid[i].push_back(tile.num);
        tile.matched = true;
        return;
      } else {
        rotate_tile_cw(tile);
      }
    }

    flip_tile_vert(tile);
    for(int k = 0; k < 4; k++) {
      top_match = (top == 0) ? top_match : (top == tile.hash_top());
      right_match = right == tile.hash_right();
      bottom_match = bottom == tile.hash_bottom();
      left_match = (left == 0) ? left_match : (left == tile.hash_left());
      if((top_match || top == -1) &&
          (right_match || right == -1) && 
          (bottom_match || bottom == -1) &&
          (left_match || left == -1)) {
        if(tile_grid.size() == i) {
          tile_grid.push_back({});
        }
        tile_grid[i].push_back(tile.num);
        tile.matched = true;
        return;
      } else {
        rotate_tile_cw(tile);
      }
    }
  }
}

bool unused_tiles(unordered_map<int, tile_struct> &tiles) {
  for(auto& tile_it : tiles) {
    auto& tile = tile_it.second;
    if(!tile.matched) {
      return true;
    }
  }
  return false;
}

vector<string> build_image(unordered_map<int, tile_struct> &tiles, vector<vector<int>> &tile_grid) {
  vector<string> image = {};
  for(auto& row : tile_grid) {
    vector<string> sub_image = {};
    for(auto& col : row) {
      auto& tile = tiles[col];
      for(int i = 1; i < tile.data.size() - 1; i++) {
        if(sub_image.size() <= i - 1) sub_image.push_back("");
        for(int j = 1; j < tile.data[i].size() - 1; j++) {
          sub_image[i-1] += string(1,tile.data[i][j]);
        }
      }
    }
    for(auto& line : sub_image)
      image.push_back(line);
  }
  return image;
}

const string monster_1 = "                  # ";
const string monster_2 = "#    ##    ##    ###";
const string monster_3 = " #  #  #  #  #  #   ";

int count_monsters(vector<string> &image) {
  int monsters = 0;
  for(int line = 2; line < image.size(); line++) {
    for(int position = 0; position < image[line].length() - monster_1.length(); position++) {
      bool monster_found = true;
      for(int sub_position = 0; sub_position < monster_1.length(); sub_position++) {
        if(image[line][position + sub_position] != '#' &&
           monster_3[sub_position] == '#') {
          monster_found = false;
          break;
        }
        if(image[line-1][position + sub_position] != '#' &&
           monster_2[sub_position] == '#') {
          monster_found = false;
          break;
        }
        if(image[line-2][position + sub_position] != '#' &&
           monster_1[sub_position] == '#') {
          monster_found = false;
          break;
        }
      } 
      if(monster_found) monsters++;
    }
  } 

  return monsters;
}

int count_roughness(vector<string> &image, int &monsters) {
  long roughness = -1 * 15 * monsters;  

  for(auto& line : image) {
    for(auto& pixel : line) {
      if(pixel == '#') roughness++;
    }
  }

  return roughness;
}

int main() {
  unordered_map<int, tile_struct> tiles = {};
  load_tiles(tiles);

  vector<vector<int>> tile_grid = {};
  find_possible_match(tiles, tile_grid, 0, 0);
  while(unused_tiles(tiles)) {
    for(int i = 0; i < tile_grid.size(); i++) {
      find_possible_match(tiles, tile_grid, i, tile_grid[i].size()); 
    }
    find_possible_match(tiles, tile_grid, tile_grid.size(), 0); 

    for(auto& row : tile_grid) {
      for(auto& col : row) {
        cout << col << " ";        
      }
      cout << endl;
    }
    cout << endl;
  }

  long long result = tile_grid[0][0];
  result *= tile_grid[0][tile_grid[0].size()-1];
  result *= tile_grid[tile_grid.size()-1][0];
  result *= tile_grid[tile_grid.size()-1][tile_grid[tile_grid.size()-1].size()-1];

  cout << "[Part 1]: [" << result << "]" << endl;

  vector<string> image = build_image(tiles, tile_grid);
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 4; j++) {
      int monsters = count_monsters(image);
      if(monsters > 0) {
        cout << "Found [" << monsters << "] monsters" << endl;
        cout << "Sea roughness [" << count_roughness(image, monsters) << "]" << endl;
        return 0;
      }

      rotate_grid_cw(tile_grid);
      for(auto& tile_it : tiles) {
        auto& tile = tile_it.second;
        rotate_tile_cw(tile);
      }
      image = build_image(tiles, tile_grid);
    }

    flip_grid_vert(tile_grid);
    for(auto& tile_it : tiles) {
      auto& tile = tile_it.second;
      flip_tile_vert(tile);
    }
    image = build_image(tiles, tile_grid);
  }
}

