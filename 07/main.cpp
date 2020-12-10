#include <iostream>
#include <vector>

using namespace std;

struct bag_struct;
struct bag_link {
  int bag;
  int number;
};

struct bag_struct {
  string name;
  vector<bag_link> links;
};

void print_bags(vector<bag_struct> *bags) {
  for(bag_struct bag : *bags) {
    cout << "=====" << endl;
    cout << "Bag: " << bag.name << endl;
    cout << "=====" << endl;
    cout << "Links" << endl;
    cout << "=====" << endl;
    for(bag_link link : bag.links) { 
      cout << "[" << link.number << "x] " << bags->at(link.bag).name << endl;
    }
    cout << "=====" << endl;
    cout << endl;
  }
}

string ltrim(string str) {
  int trim_position = 0; 
  while (trim_position < str.length() && isspace(str[trim_position])) trim_position++;
  return str.substr(trim_position);
}

string rtrim(string str) {
  int trim_position = str.length() - 1;
  while(trim_position >= 0 && isspace(str[trim_position])) trim_position--;
  return str.substr(0, ++trim_position);
}

string trim(string str) {
  return ltrim(rtrim(str));
}

string strip_bags(string bag_name) {
  int bags_location = bag_name.find("bags");
  int bag_location = bag_name.find("bag");

  if(bags_location != string::npos) 
    return trim(bag_name.substr(0, bags_location));
  else if (bag_location != string::npos)
    return trim(bag_name.substr(0, bag_location));
  else
    return bag_name; 
}

bool find_linked_bag(vector<bag_struct> *bags, vector<bag_link> linked_bags, string bag_name) {
  for(int i = 0; i < linked_bags.size(); i++) {
    if(bags->at(linked_bags[i].bag).name == bag_name) return true;
  }
  return false;
}

int get_bag(vector<bag_struct> *bags, string new_bag_name) {
  for(int i = 0; i < bags->size(); i++) {
    if((*bags)[i].name == new_bag_name) return i;
  }
  bag_struct bag = {new_bag_name, {}};
  bags->push_back(bag);
  return get_bag(bags, new_bag_name);
}

int find_at_least_one(vector<bag_struct> *bags, vector<int> searched_so_far, string searched_bag) {
  int found = 0;
  if(searched_so_far.size() == 0) {
    for(bag_struct bag : *bags) {
      vector<int> new_searched_for = searched_so_far;
      new_searched_for.push_back(get_bag(bags, bag.name));
      found += find_at_least_one(bags, new_searched_for, searched_bag);
    }
  } else {
    int desired_index = get_bag(bags, searched_bag);
    for(bag_link link : bags->at(searched_so_far.back()).links) {
      if(desired_index == link.bag) {
        return 1;
      }
    }

    for(bag_link link : bags->at(searched_so_far.back()).links) {
      vector<int> new_searched_for = searched_so_far;
      new_searched_for.push_back(link.bag);
      int result = find_at_least_one(bags, new_searched_for, searched_bag);
      if(result > 0) return ++found;
    }
  }

  return found;
}

int find_bags_in_bag(vector<bag_struct> *bags, vector<int> searched_so_far, string searched_bag) {
  int found = 0;
  if(searched_so_far.size() == 0) {
    vector<int> new_searched_for = searched_so_far;
    new_searched_for.push_back(get_bag(bags, searched_bag));
    found = find_bags_in_bag(bags, new_searched_for, searched_bag);
  } else {
    if(bags->at(searched_so_far.back()).links.size() == 0) return 1;
    for(bag_link link : bags->at(searched_so_far.back()).links) {
      vector<int> new_searched_for = searched_so_far;
      new_searched_for.push_back(link.bag);
      int result = find_bags_in_bag(bags, new_searched_for, searched_bag);
      found += (link.number * result);
    }
    found++;
  }

  return found;
}


int main() {
  const string bag_link_delimiter = "contain";
  const string link_delimiter = ",";
  const string no_links = "no other bags";

  string next_bag = "";
  vector<bag_struct> bags = {};
  while(getline(cin, next_bag)) {
    string bag_name = next_bag.substr(0, next_bag.find(bag_link_delimiter));
    bag_name = strip_bags(bag_name);
    string links = next_bag.substr(next_bag.find(bag_link_delimiter) + bag_link_delimiter.length());

    bag_struct container = bags[get_bag(&bags, bag_name)];

    int next_link = 0;
    while(next_link != string::npos) {
      next_link = links.find(link_delimiter);
      string link = trim(links.substr(0, next_link == string::npos ? links.length() - 1 : next_link));
      links = links.substr(next_link == string::npos ? 0 : ++next_link);

      if(link == no_links) continue;

      int count = stoi(link.substr(0, link.find(" ")));
      string linked_bag_name = link.substr(link.find(" ") + 1);

      linked_bag_name = strip_bags(linked_bag_name);
      bag_struct linked_bag = bags[get_bag(&bags, linked_bag_name)];
      if(!find_linked_bag(&bags, container.links, linked_bag_name)) {
        bag_link linked_bag_struct = {get_bag(&bags, linked_bag_name), count};
        container.links.push_back(linked_bag_struct);
        bags[get_bag(&bags, container.name)] = container;
      } else {
        cout << "DUPLICATE LINK!!" << endl;
        cout << container.name << endl;
        cout << linked_bag_name << endl;
        print_bags(&bags);
        exit(12);
      }
    }   
  }

  string desired_bag = "shiny gold";
  //print_bags(&bags);
  cout << find_at_least_one(&bags, {}, desired_bag) << endl;
  cout << find_bags_in_bag(&bags, {}, desired_bag) - 1 << endl;
}
