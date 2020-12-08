#include <iostream>
#include <vector>

using namespace std;

struct instruction_struct {
  string operation;
  int value;
};

struct register_struct {
  int ip;
  int acc;
};

const string NOP = "nop";
const string JMP = "jmp";
const string ACC = "acc";

register_struct run_program(vector<instruction_struct> *program, int flip_op) {
  register_struct registers = {0, 0};
  vector<bool> visited(program->size());

  instruction_struct next_instruction = program->at(registers.ip);
  while(!visited[registers.ip]) {
    visited[registers.ip] = true;

    if((next_instruction.operation == NOP && flip_op != registers.ip)
        || (next_instruction.operation == JMP && flip_op == registers.ip)) { 
      registers.ip++;
    }
    else if((next_instruction.operation == JMP && flip_op != registers.ip)
          || (next_instruction.operation == NOP && flip_op == registers.ip)) {
      registers.ip += next_instruction.value;
    }
    else if(next_instruction.operation == ACC) { 
      registers.acc += next_instruction.value; registers.ip++; 
    }

    if(registers.ip == program->size()) break;
    if(registers.ip < 0 || registers.ip > program->size()) return {-1, -1};
    next_instruction = program->at(registers.ip);
  }

  return registers;
}

int main() {
  vector<instruction_struct> program = {};

  cout << "Loading program..." << endl;
  string next_line = "";
  while(getline(cin, next_line)) {
    instruction_struct instruction = {};
    instruction.operation = next_line.substr(0, next_line.find(" "));
    instruction.value = stoi(next_line.substr(next_line.find(" ")));

    program.push_back(instruction);
  }

  cout << "Running until we loop..." << endl;
  cout << "Default ACC: " << run_program(&program, -1).acc << endl;

  for(int i = 0; i < program.size(); i++) {
    instruction_struct instruction = program[i];
    if(instruction.operation != ACC) {
      register_struct registers = run_program(&program, i);
      if(registers.ip == program.size()) {
        cout << "Flipped instruction " << i << endl;
        cout << "ACC: " << registers.acc << endl;
      } 
    }
  }

}
