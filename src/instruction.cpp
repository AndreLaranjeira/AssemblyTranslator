#include <deque>
#include <iostream>
#include <sstream>
#include "line.hpp"
#include "instruction.hpp"

using namespace std;

string lower_case(string line)
{
  string out = line;
  for (auto & c: out)
        c = tolower(c);
  return out;
}

size_t num_non_null_operands(deque<string> operands)
{
  size_t num_operands = 0;
  for(auto const& operand : operands){
    if(operand != ""){
      num_operands++;
    }
  }
  return num_operands;
}

Instruction::Instruction(string p_name = "",
                         bool p_needs_label = false,
                         unsigned int p_min_num_par = 0,
                         unsigned int p_max_num_par = 0) {
  name = p_name;
  for (auto & c: name)
    c = toupper(c);
  needs_label = p_needs_label;
  min_num_par = p_min_num_par;
  max_num_par = p_max_num_par;
}

deque<Line*> Instruction::translate(Line* original)
{
  Line* new_line = new Line(*original);
  new_line->label = "placeholder_"+original->label;
  deque<Line*> lines;
  lines.push_back(new_line);
  return lines;
}

bool Instruction::validate(Line* line)
{
  bool result = true;
  if(needs_label && line->label == "") {
    cout << "Operation: " << line->operation << " Has no obligatory label!" <<endl;
    result = false;
  }

  size_t num_operands = num_non_null_operands(line->operand_list);
  if(min_num_par > num_operands || num_operands > max_num_par) {
    cout << "Operation: " << line->operation << " Has wrong number of arguments!" <<endl;
    cout << "Expected number between " << min_num_par << " and " << max_num_par << endl;
    cout << "Found: " << line->operand_list.size() << endl;
    for(auto const& operand : line->operand_list){
      cout << "Operand: " << operand << endl;
    }
    result = false;
  }

  if(name != line->operation) {
    cout << "Operation: " << line->operation << " Operation does not match!" <<endl;
    result = false;
  }

  return result;
}

deque<Line*> InstructionAdd::translate(Line* original)
{
  // ADD OP -> add eax, [OP]
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("["+ original->operand_list.front() +"]");
  new_line = new Line(original->number, original->label, "add", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionSub::translate(Line* original)
{
  // SUB OP -> sub eax, [OP]
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("["+ original->operand_list.front() +"]");
  new_line = new Line(original->number, original->label, "sub", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

const char InstructionMult::jump_name[] = "MULT_cont";
unsigned int InstructionMult::jump_name_counter = 0;
deque<Line*> InstructionMult::translate(Line* original)
{
  // MULT OP -> imul dword [OP] ; Already saves on eax
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;
  stringstream jump;
  jump << jump_name << jump_name_counter++;


  // imul dword [OP] ; Already saves on eax
  operands.push_back("dword ["+ original->operand_list.front() +"]");
  new_line = new Line(original->number, original->label, "imul", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  // pushf -> flags are in dword [esp]
  operands.clear();
  new_line = new Line(original->number, "", "pushf", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  // tidy up the stack
  operands.clear();
  new_line = new Line(original->number, "", "popf", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  // tests the bit OF in the flags
  operands.clear();
  operands.push_back("dword [esp-4]");
  operands.push_back("0800h"); // it is on bit 11
  new_line = new Line(original->number, "", "test", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  // jz MULT_cont1 ; if OF is 0, there's no overflow
  operands.clear();
  operands.push_back(jump.str());
  new_line = new Line(original->number, "", "jz", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  // call OverflowWarning ; calls subroutine to warn about overflow
  operands.clear();
  operands.push_back("OverflowWarning");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  // MULT_cont1: ; continues the program
  operands.clear();
  new_line = new Line(original->number, jump.str(), "", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionDiv::translate(Line* original)
{
  // DIV OP -> cdq ; Extends sign | idiv dword [OP] ; already saves on eax
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  new_line = new Line(original->number, original->label, "cdq", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.push_back("dword ["+ original->operand_list.front() +"]");
  new_line = new Line(original->number, "", "idiv", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionJmp::translate(Line* original)
{
  // JMP OP -> jmp OP
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back(original->operand_list.front());
  new_line = new Line(original->number, original->label, "jmp", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionJmpn::translate(Line* original)
{
  // JMPN OP -> cmp eax, 0 ; generates flags | jl OP ; jumps if negative
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("0");
  new_line = new Line(original->number, original->label, "cmp", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list.front());
  new_line = new Line(original->number, "", "jl", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionJmpp::translate(Line* original)
{
  // JMPP OP -> cmp eax, 0 ; generates flags | jg OP ; jumps if positive
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("0");
  new_line = new Line(original->number, original->label, "cmp", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list.front());
  new_line = new Line(original->number, "", "jg", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionJmpz::translate(Line* original)
{
  // JMPZ OP -> cmp eax, 0 ; generates flags | jz OP ; jumps if zero
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("0");
  new_line = new Line(original->number, original->label, "cmp", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list.front());
  new_line = new Line(original->number, original->label, "jz", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionCopy::translate(Line* original)
{
  // COPY OP1, OP2 -> mov ebx, dword [OP1] | mov dword [OP2], ebx
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("ebx");
  operands.push_back("dword [" + original->operand_list[0] + "]");
  new_line = new Line(original->number, original->label, "mov", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("dword [" + original->operand_list[1] + "]");
  operands.push_back("ebx");
  new_line = new Line(original->number, "", "mov", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionLoad::translate(Line* original)
{
  // LOAD OP -> mov eax, dword [OP]
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("dword [" + original->operand_list[0] + "]");
  new_line = new Line(original->number, original->label, "mov", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionStore::translate(Line* original)
{
  // STORE OP -> mov dword [OP], eax
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("dword [" + original->operand_list[0] + "]");
  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "mov", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}


deque<Line*> InstructionInput::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("LerInteiro");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("eax");
  new_line = new Line(original->number, "", "pop", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionOutput::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("EscreverInteiro");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("eax");
  new_line = new Line(original->number, "", "pop", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionCInput::translate(Line* original)
{
  // C_INPUT OP -> push eax | push OP | call LerChar | pop eax
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("LerChar");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("eax");
  new_line = new Line(original->number, "", "pop", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionCOutput::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("EscreverChar");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("eax");
  new_line = new Line(original->number, "", "pop", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionSInput::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[1]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("LerString");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("eax");
  new_line = new Line(original->number, "", "pop", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionSOutput::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  new_line = new Line(original->number, original->label, "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original->operand_list[1]);
  new_line = new Line(original->number, "", "push", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("EscreverString");
  new_line = new Line(original->number, "", "call", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("eax");
  new_line = new Line(original->number, "", "pop", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}


deque<Line*> InstructionStop::translate(Line* original)
{
  // STOP -> mov eax, 0 | mov ebx, 0 | int 0x80
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("eax");
  operands.push_back("1");
  new_line = new Line(original->number, original->label, "mov", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("ebx");
  operands.push_back("0");
  new_line = new Line(original->number, "", "mov", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("80h");
  new_line = new Line(original->number, "", "int", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionSection::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back("."+lower_case(original->operand_list[0]));
  new_line = new Line(original->number, original->label, "section", operands);
  lines.push_back(new_line);

  if(original->operand_list[0] == "TEXT") {
    operands.clear();
    new_line = new Line(original->number, "_start", "", operands);
    lines.push_back(new_line);
  }

  return lines;
}

bool InstructionSection::validate(Line* line)
{
  bool result = Instruction::validate(line);

  if (line->operand_list[0] != "TEXT" &&
      line->operand_list[0] != "DATA" &&
      line->operand_list[0] != "BSS") {
    result = false;
  }

  return result;
}

deque<Line*> InstructionSpace::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  if(num_non_null_operands(original->operand_list) > 0) {
    operands.push_back(original->operand_list[0]);
  } else {
    operands.push_back("1");
  }

  new_line = new Line(original->number, original->label, "resd", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}

deque<Line*> InstructionConst::translate(Line* original)
{
  deque<string> operands;
  deque<Line*> lines;
  Line* new_line;

  operands.push_back(original->operand_list[0]);
  new_line = new Line(original->number, original->label, "dd", operands);
  new_line->comment = original->to_string();
  lines.push_back(new_line);

  return lines;
}
