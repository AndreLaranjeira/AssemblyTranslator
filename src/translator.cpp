// Translator module - Source code.

// Includes:
#include "translator.hpp"

// Class methods:
Translator::Translator(string p_name) : error_logger(p_name) {
  name = p_name;
}

Translator::~Translator() {
  for(auto const& instruction_pair : instructions_table) {
    delete instruction_pair.second;
  }
}

int Translator::add_instruction(Instruction *instruction) {

  // Give warning if we overwrite anything.
  if(instructions_table.count(instruction->name) != 0)
    error_logger.log_error(WARNING, DATA_OVERWRITE,
                           "Instruction " + name + ". " +
                           "Original definition kept!");

  // Save instruction.
  instructions_table[instruction->name] = instruction;

  return 0;

}

int Translator::load_default_mnemonics() {
  int status = 0;
  status += this->add_instruction(new InstructionAdd());
  status += this->add_instruction(new InstructionSub());
  status += this->add_instruction(new InstructionMult());
  status += this->add_instruction(new InstructionDiv());
  status += this->add_instruction(new InstructionJmp());
  status += this->add_instruction(new InstructionJmpn());
  status += this->add_instruction(new InstructionJmpp());
  status += this->add_instruction(new InstructionJmpz());
  status += this->add_instruction(new InstructionCopy());
  status += this->add_instruction(new InstructionLoad());
  status += this->add_instruction(new InstructionStore());
  status += this->add_instruction(new InstructionInput());
  status += this->add_instruction(new InstructionOutput());
  status += this->add_instruction(new InstructionCInput());
  status += this->add_instruction(new InstructionCOutput());
  status += this->add_instruction(new InstructionSInput());
  status += this->add_instruction(new InstructionSOutput());
  status += this->add_instruction(new InstructionStop());

  status += this->add_instruction(new InstructionSection());
  status += this->add_instruction(new InstructionSpace());
  status += this->add_instruction(new InstructionConst());
  return status;
}

int Translator::append_functions(deque <Line> functions) {

  for(auto const& function : functions)
    ia32_buffer.push_back(function);

  return 0;

}

int Translator::translate_asm_buffer() {

  int status = 0;
  Instruction *instruction;
  deque <string> operand_list;

  // Adiciona global _start
  operand_list.push_back("_start");
  ia32_buffer.push_back(Line(1, "", "global", operand_list));
  operand_list.clear();

  // Itera por todas as linhas do arquivo assembly inventado
  for(auto const& line : this->asm_buffer){
    instruction = this->instructions_table[line.operation];
    if(instruction->validate(line)) {
      for(auto const& translated_line : instruction->translate(line)) {
        ia32_buffer.push_back(translated_line);
      }
    } else {
      error_logger.log_error(ERROR, INSTRUCTION,
      "Instruction "+ line.operation +" does not exists! Ignoring line and going on...",
      line.number);
      status++;
    }
  }

  /*
  operand_list.push_back(".text");
  ia32_buffer.push_back(Line(1, "", "section", operand_list));
  operand_list.clear();


  ia32_buffer.push_back(Line(1, "_start", "", operand_list));
  */

  ia32_buffer.push_back(Line(-1,"","",operand_list));

  return status;

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

    if(instruction.second->needs_label)
      cout << "True" << endl;

    else
      cout << "False" << endl;

    cout << "Minimum number of parameters: " << instruction.second->min_num_par;
    cout << endl;

    cout << "Maximum number of parameters: " << instruction.second->max_num_par;
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
