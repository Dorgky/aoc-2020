#include <iostream> 
#include <vector> 
#include <unordered_map>
#include <functional>

using namespace std;

void load_deck(vector<int> &deck) {
  string next_line = "";
  while(getline(cin, next_line)) {
    if(next_line == "") {
      return;
    }
    if(next_line.find("Player") != string::npos) {
      continue;
    }
    deck.push_back(stoi(next_line));
  }
}

int calculate_score(vector<int> &deck) {
  int score = 0;
  for(int i = 0; i < deck.size(); i++) {
    score += (deck[i] * (deck.size() - i));
  }
  return score;
}

long calculate_hash(vector<int> &deck) {
  string str = "";
  for(auto& card : deck) {
    str += card;
  }
  return hash<string>{}(str);
}

void play_game(vector<int> &deck_1, vector<int> &deck_2) {
  while(deck_1.size() != 0 && deck_2.size() != 0) {
    int card_1 = deck_1.front();
    deck_1.erase(deck_1.begin());
    int card_2 = deck_2.front();
    deck_2.erase(deck_2.begin());

    if(card_1 > card_2) {
      deck_1.push_back(card_1);
      deck_1.push_back(card_2);
    } else {
      deck_2.push_back(card_2);
      deck_2.push_back(card_1);
    }
  }
}

void play_recursive_game(vector<int> &deck_1, vector<int> &deck_2) {
  unordered_map<long, bool> previous_decks = {};
  while(deck_1.size() != 0 && deck_2.size() != 0) {
    long deck_1_hash = calculate_hash(deck_1);
    if(previous_decks[deck_1_hash]) {
      deck_2.clear();  
      return;
    }
    previous_decks[deck_1_hash] = true;

    int card_1 = deck_1.front();
    deck_1.erase(deck_1.begin());
    int card_2 = deck_2.front();
    deck_2.erase(deck_2.begin());

    if(deck_1.size() >= card_1 && deck_2.size() >= card_2) {
      vector<int> sub_deck_1 = deck_1;
      sub_deck_1.resize(card_1);
      vector<int> sub_deck_2 = deck_2;
      sub_deck_2.resize(card_2);
      play_recursive_game(sub_deck_1, sub_deck_2);
      if(sub_deck_1.size() > 0) {
        deck_1.push_back(card_1);
        deck_1.push_back(card_2);
      } else {
        deck_2.push_back(card_2);
        deck_2.push_back(card_1);
      }      
    } else if(card_1 > card_2) {
      deck_1.push_back(card_1);
      deck_1.push_back(card_2);
    } else {
      deck_2.push_back(card_2);
      deck_2.push_back(card_1);
    }
  }
}

int main() {
  vector<int> player_1_deck = {}, player_2_deck = {};
  load_deck(player_1_deck);
  load_deck(player_2_deck);

  vector<int> player_1_deck_copy = player_1_deck, player_2_deck_copy = player_2_deck;
  play_game(player_1_deck, player_2_deck);
  play_recursive_game(player_1_deck_copy, player_2_deck_copy);

  int score = 0;
  if(player_1_deck.size() > 0) {
    score = calculate_score(player_1_deck);
  } else {
    score = calculate_score(player_2_deck);
  }

  cout << "Winning score for [Part 1]: [" << score << "]" << endl;

  if(player_1_deck_copy.size() > 0) {
    score = calculate_score(player_1_deck_copy);
  } else {
    score = calculate_score(player_2_deck_copy);
  }
  cout << "Winning score for [Part 2]: [" << score << "]" << endl;
}
