#include <iostream>
#include <vector>

using namespace std;

enum op {NOP = ' ', ADD = '+', MULT = '*', OPEN = '(', CLOSE = ')'};

struct stack_op_struct {
  op operation;
  long value;
  long multi_brackets;

  stack_op_struct(op operation_, long value_, long multi_brackets_) {
    operation = operation_;
    value = value_;
    multi_brackets = multi_brackets_;
  }
};

vector<string> split(string &str) {
  vector<string> str_list = {};

  for(int i = 0; i < str.length(); i++) {
    if(str[i] >= '0' && str[i] <= '9') {
      str_list.push_back(to_string(stol(str.substr(i))));
      while(str[i+1] >= '0' && str[i+1] <= '9') {
        i++;
      }
    } else if(str[i] != ' ') {
      str_list.push_back(string(1, str[i]));
    } 
  }

  return str_list;
}

op parse_next_op(string &next_value) {
  if(next_value == "(") {
    return op::OPEN; 
  } else if (next_value == ")") {
    return op::CLOSE;
  } else if (next_value == "+") {
    return op::ADD;
  } else if (next_value == "*") {
    return op::MULT;
  } else {
    return op::NOP;
  }
}

long process_op(op &operation, long value, long &previous_value) {
  switch(operation) {
    case op::ADD:
      previous_value += value;
      break;
    case op::MULT:
      previous_value *= value;
      break;
    default:
      break;
  }

  operation = op::NOP;
  return previous_value;
}

const bool use_special_precedence = true;

int main() {
  long total_sum = 0;
  string next_line = "";

  vector<stack_op_struct> stack = {};
  stack_op_struct stack_op = {op::NOP, 0, 0};
  while(getline(cin, next_line)) {
    op current_op = op::ADD;
    long multi_brackets = 0;
    long line_sum = 0;

    vector<string> line = split(next_line); 
    for(auto next_token : line) {
      switch(parse_next_op(next_token)) {
        case op::NOP:
          process_op(current_op, stol(next_token), line_sum);
          break;
        case op::ADD:
          current_op = op::ADD;
          break;
        case op::MULT:
          current_op = op::MULT;
          if(use_special_precedence) {
            stack.push_back({current_op, line_sum, 0});
            line_sum = 0;
            current_op = op::ADD;
            multi_brackets++;
          }
          break;
        case op::OPEN:
          stack.push_back({current_op, line_sum, multi_brackets});
          multi_brackets = 0;
          line_sum = 0;
          current_op = op::ADD;
          break;
        case op::CLOSE:
          while(multi_brackets > 0) {
            multi_brackets--;
            stack_op = stack.back();
            stack.pop_back();
            current_op = stack_op.operation;          
            process_op(current_op, stack_op.value, line_sum);
          }
          stack_op = stack.back();
          stack.pop_back();
          multi_brackets = stack_op.multi_brackets;
          current_op = stack_op.operation;          
          process_op(current_op, stack_op.value, line_sum);
          break;
        default:
          break;
      }
    }

    while(stack.size() > 0) {
      stack_op = stack.back();
      stack.pop_back();
      multi_brackets = stack_op.multi_brackets;
      process_op(stack_op.operation, stack_op.value, line_sum);
    }
   
    total_sum += line_sum;
  }
  cout << "Total [" << total_sum << "]" << endl;
}
