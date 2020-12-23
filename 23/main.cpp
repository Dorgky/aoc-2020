#include <iostream>

using namespace std;

const int cups_to_move = 3;

bool cup_is_picked_up(int *cups, int current_cup, int destination_cup) {
  for(int i = 0; i < cups_to_move; i++) {
    current_cup = cups[current_cup];
    if(current_cup == destination_cup) return true;
  } 
  return false;
}

void move_cups(int *cups, int current_cup, int destination_cup) {
  int saved_cup_head = cups[current_cup];
  int saved_cup_tail = cups[cups[saved_cup_head]];
  int next_unsaved_cup = cups[saved_cup_tail];

  cups[current_cup] = next_unsaved_cup;
  cups[saved_cup_tail] = cups[destination_cup];
  cups[destination_cup] = saved_cup_head;
}

const bool part1 = false;

int main() {
  string next_line = "";
  cin >> next_line;

  const int cup_count = (part1 ? next_line.length() : 1000000) + 1;
  int *cups = (int*)malloc(sizeof(int) * (cup_count));
  int previous_cup = 0;
  int starting_cup = -1;
  for(int i = 1; i < cup_count; i++) {
    if(i < next_line.length() + 1) {
      int this_cup = stoi(string(1,next_line[i-1]));
      cups[previous_cup] = this_cup;
      previous_cup = this_cup;
      if(starting_cup == -1) starting_cup = this_cup;
    } else {
      cups[previous_cup] = i;
      previous_cup = i;
    }
  }
  cups[previous_cup] = starting_cup;

  const int max_rounds = part1 ? 100 : 10000000;
  int current_cup = starting_cup;
  for(int i = 0; i < max_rounds; i++) {
    int destination_cup = current_cup;
    do {
      --destination_cup;
      if(destination_cup == 0) destination_cup = cup_count - 1;
    } while (cup_is_picked_up(cups, current_cup, destination_cup));

    move_cups(cups, current_cup, destination_cup);

    current_cup = cups[current_cup];
  }

  current_cup = 1;
  if(part1) {
    do {
      current_cup = cups[current_cup];
      cout << current_cup;
    } while(current_cup != 1);
    cout << endl;
  } else {
    current_cup = 1;
    current_cup = cups[current_cup];
    long result = current_cup;
    cout << result << " * ";
    current_cup = cups[current_cup];
    cout << current_cup << " = ";
    result *= current_cup;
    cout << result << endl;
  }

  free(cups);
}
