#include <iostream>
#include <vector>

int main() {
  int nextValue = -1;

  std::vector<int> values = {};
  while(std::cin >> nextValue) {
    values.push_back(nextValue);
  }

  for(int i = 0; i < values.size(); i++) {
    for(int j = i + 1; j < values.size(); j++) {
      if(values.at(i) + values.at(j) == 2020) {
        std::cout << values.at(i) << " + " << values.at(j) << " = 2020" << std::endl;
        std::cout << values.at(i) * values.at(j) << std::endl;
      }
      for(int k = j + 1; k < values.size(); k++) {
        if(values.at(i) + values.at(j) + values.at(k) == 2020) {
          std::cout << values.at(i) << " + " << values.at(j) << " + " << values.at(k) << " = 2020" << std::endl;
          std::cout << values.at(i) * values.at(j) * values.at(k) << std::endl;
        }
      }
    }
  }
}
