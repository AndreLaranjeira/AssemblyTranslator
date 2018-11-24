// Translator module - Source code.

// Includes:
#include "translator.hpp"

// Class methods:
Translator::Translator(string p_name) : error_logger(p_name) {
  name = p_name;
}

Translator::~Translator() {

}

int Translator::add_instruction(string name, bool p_needs_label,
                                unsigned int p_min_num_par,
                                unsigned int p_max_num_par) {

  // Uppercase the instruction name to avoid confusion.
  for (auto & c: name)
    c = toupper(c);

  // Give warning if we overwrite anything.
  if(instructions_table.count(name) != 0)
    error_logger.log_error(WARNING, DATA_OVERWRITE,
                           "Instruction " + name + ". " +
                           "Original definition kept!");

  // Save instruction.
  instructions_table.insert(make_pair(name, Instruction(p_needs_label,
                            p_min_num_par, p_max_num_par)));

  return 0;

}

int Translator::append_functions(deque <Line> functions) {

  for(auto const& function : functions)
    ia32_buffer.push_back(function);

  return 0;

}

int Translator::translate_asm_buffer() {

  // This is a PLACEHOLDER CODE meant for TESTS ONLY!

  deque <string> operand_list;

  operand_list.push_back(".text");
  ia32_buffer.push_back(Line(1, "", "section", operand_list));
  operand_list.clear();

  operand_list.push_back("_start");
  ia32_buffer.push_back(Line(1, "", "global", operand_list));
  operand_list.clear();

  ia32_buffer.push_back(Line(1, "_start", "", operand_list));

  return 0;

}

// Debug methods:
void Translator::print_asm_buffer() {

}

void Translator::print_ia32_buffer() {

}

void Translator::print_instructions() {

  cout << endl;
  cout << "[DEBUG] Instructions for translator " + name << ":" << endl;
  cout << endl;

  for(auto const& instruction : instructions_table) {

    cout << instruction.first << ": " << endl;
    cout << "Requires label: ";

    if(instruction.second.needs_label)
      cout << "True" << endl;

    else
      cout << "False" << endl;

    cout << "Minimum number of parameters: " << instruction.second.min_num_par;
    cout << endl;

    cout << "Maximum number of parameters: " << instruction.second.max_num_par;
    cout << endl;

    cout << endl;

  }

}

// Getters:
deque <Line> Translator::get_ia32_buffer() {
  return ia32_buffer;
}

// Setters:
void Translator::set_asm_buffer(deque <Line> p_asm_buffer) {
  asm_buffer = p_asm_buffer;
}

void Translator::set_debug_mode(bool p_debug_mode) {
  debug_mode = p_debug_mode;
}
