#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const char NORTH = 'N';
const char EAST = 'E';
const char SOUTH = 'S';
const char WEST = 'W';
const char LEFT = 'L';
const char RIGHT = 'R';
const char FORWARD = 'F';
const int TRIG_ADJ = 90;
const int MAX_DEGREES = 360;
const double DEG_TO_RAD = M_PI/180;

const vector<char> CARDINALS {NORTH, EAST, SOUTH, WEST};
const vector<int> DEGREES {0, 90, 180, 270};
const vector<char> ROTATIONS {LEFT, FORWARD, RIGHT};
const vector<char> MOVEMENTS {FORWARD, NORTH, EAST, SOUTH, WEST};

struct directions_struct {
  char direction;
  int magnitude;
};

struct position_struct {
  int horizontal;
  int vertical;
  int facing;
};

void load_data(vector<directions_struct> *directions) {
  string next_direction = "";
  while(cin >> next_direction) {
    directions->push_back(
        {next_direction[0], 
        stoi(next_direction.substr(1))});
  }
}

void navigate_ship(vector<directions_struct> *directions, position_struct *position) {
  for(auto next_direction : *directions) {
    auto rotation_location = find(ROTATIONS.begin(), ROTATIONS.end(), next_direction.direction);
    auto cardinal_location = find(CARDINALS.begin(), CARDINALS.end(), next_direction.direction);
    auto movement_location = find(MOVEMENTS.begin(), MOVEMENTS.end(), next_direction.direction);

    int my_movement = position->facing;
    if(rotation_location != ROTATIONS.end()) {
      int rotation = rotation_location - ROTATIONS.begin();
      rotation--; // Adjust for L/F/R = -1/0/1
      my_movement += rotation * next_direction.magnitude;
      my_movement %= MAX_DEGREES;
      position->facing = my_movement;
    } else if(cardinal_location != CARDINALS.end()) {
      my_movement = DEGREES[cardinal_location - CARDINALS.begin()];
    }

    if(movement_location != MOVEMENTS.end()) {
      double adjusted_facing = (my_movement - TRIG_ADJ) % MAX_DEGREES;
      int vertical_change = round(sin(adjusted_facing*DEG_TO_RAD) * next_direction.magnitude);
      int horizontal_change = round(cos(adjusted_facing*DEG_TO_RAD) * next_direction.magnitude);

      position->vertical += vertical_change;
      position->horizontal += horizontal_change;
    }
  }
}

void navigate_waypoint(vector<directions_struct> * directions, position_struct *position, position_struct *waypoint) {
  for(auto next_direction : *directions) {
    auto rotation_location = find(ROTATIONS.begin(), ROTATIONS.end(), next_direction.direction);
    auto cardinal_location = find(CARDINALS.begin(), CARDINALS.end(), next_direction.direction);
    auto movement_location = find(MOVEMENTS.begin(), MOVEMENTS.end(), next_direction.direction);

    if(rotation_location != ROTATIONS.end()) {
      double rotation = rotation_location - ROTATIONS.begin();
      rotation--; // Adjust for L/F/R = -1/0/1
      rotation *= -1; // And flip for rotation
      rotation *= next_direction.magnitude;
      while(rotation < 0) rotation += MAX_DEGREES;
      while(rotation >= MAX_DEGREES) rotation -= MAX_DEGREES;

      double s = sin(rotation*DEG_TO_RAD);
      double c = cos(rotation*DEG_TO_RAD);
      double horizontal = waypoint->horizontal;
      double vertical = waypoint->vertical;
      int new_horizontal = round(horizontal * c - vertical * s);
      int new_vertical = round(vertical * c + horizontal * s);
      
      waypoint->vertical = new_vertical;
      waypoint->horizontal = new_horizontal;
    } else if(cardinal_location != CARDINALS.end()) {
      int direction_deg = DEGREES[cardinal_location - CARDINALS.begin()];   
      direction_deg -= TRIG_ADJ;
      direction_deg = MAX_DEGREES - direction_deg;
      direction_deg %= MAX_DEGREES;

      double c = cos(direction_deg*DEG_TO_RAD);
      double s = sin(direction_deg*DEG_TO_RAD);

      int horizontal_change = round(c * next_direction.magnitude);
      int vertical_change = round(s * next_direction.magnitude);
      waypoint->horizontal += horizontal_change;
      waypoint->vertical += vertical_change;
    }

    if(next_direction.direction == FORWARD) {
      position->horizontal += next_direction.magnitude * waypoint->horizontal;
      position->vertical += next_direction.magnitude * waypoint->vertical;
    }
  }
}

int main() {
  vector<directions_struct> directions = {};
  load_data(&directions); 

  position_struct position = {0,0,90};
  navigate_ship(&directions, &position);
  cout << "Part 1: " << endl;
  cout << "Current status: [" << position.horizontal << "," << position.vertical << "," << position.facing << "]" << endl;
  cout << abs(position.horizontal) + abs(position.vertical) << endl;

  position = {0,0,0};
  position_struct waypoint = {10,1,0};
  navigate_waypoint(&directions, &position, &waypoint);
  cout << "Part 2: " << endl;
  cout << "Current status: [" << position.horizontal << "," << position.vertical << "," << position.facing << "]" << endl;
  cout << abs(position.horizontal) + abs(position.vertical) << endl;
}
