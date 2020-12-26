#include <iostream>

using namespace std;

const long subject_number = 7;
const long my_mod = 20201227;

long find_secret(long pub_key) {
  long value  = 1;

  long secret = 0;
  while(value != pub_key) {
    secret++;
    value = (value * subject_number) % my_mod;
  }
  return secret;
}

long get_shared_key(long pub_key, long additional_iterations) {
  long value = 1;
  for(long i = 0; i < additional_iterations; i++) {
    value = (value * pub_key) % my_mod;
  }
  return value;
}

int main() {
  long pub_key_1, pub_key_2;
  string next_line = "";

  getline(cin, next_line);
  pub_key_1 = stol(next_line);
  getline(cin, next_line);
  pub_key_2 = stol(next_line);

  long priv_key_1 = find_secret(pub_key_1);
  long priv_key_2 = find_secret(pub_key_2);


  cout << "Part 1 [" << get_shared_key(pub_key_1, priv_key_2) << "]" << endl;
}
