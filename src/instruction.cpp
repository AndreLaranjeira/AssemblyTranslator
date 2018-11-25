#include <deque>
#include <iostream>
#include "line.hpp"
#include "instruction.hpp"

using namespace std;

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

deque<Line> Instruction::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

bool Instruction::validate(Line line)
{
  if(needs_label && line.label == "") {
    cout << "Operation: " << line.operation << " Has no obligatory label!" <<endl;
    return false;
  }

  size_t num_operands = 0; // line.operand_list.size();
  for(auto const& operand : line.operand_list){
    if(operand != ""){
      num_operands++;
    }
  }
  if(min_num_par > num_operands || num_operands > max_num_par) {
    cout << "Operation: " << line.operation << " Has wrong number of arguments!" <<endl;
    cout << "Expected number between " << min_num_par << " and " << max_num_par << endl;
    cout << "Found: " << line.operand_list.size() << endl;
    for(auto const& operand : line.operand_list){
      cout << "Operand: " << operand << endl;
    }
    return false;
  }

  if(name != line.operation) {
    cout << "Operation: " << line.operation << " Operation does not match!" <<endl;
    return false;
  }

  return true;
}


deque<Line> InstructionAdd::translate(Line original)
{
  // ADD OP -> add eax, [OP]
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("eax");
  operands.push_back("["+ original.operand_list.front() +"]");
  Line new_line(original.number, original.label, "add", operands);
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionSub::translate(Line original)
{
  // SUB OP -> sub eax, [OP]
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("eax");
  operands.push_back("["+ original.operand_list.front() +"]");
  Line new_line(original.number, original.label, "sub", operands);
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionMult::translate(Line original)
{
  // MULT OP -> imul dword [OP] ; Already saves on eax
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("dword ["+ original.operand_list.front() +"]");
  Line new_line(original.number, original.label, "imul", operands);
  lines.push_back(new_line);

  // TODO: Test for overflow and call subroutine

  return lines;
}

deque<Line> InstructionDiv::translate(Line original)
{
  // DIV OP -> cdq ; Extends sign | idiv dword [OP] ; already saves on eax
  deque<string> operands;
  deque<Line> lines;

  Line new_line(original.number, original.label, "cdq", operands);
  lines.push_back(new_line);

  operands.push_back("dword ["+ original.operand_list.front() +"]");
  new_line.label = "";
  new_line.operation = "idiv";
  new_line.operand_list = operands;
  lines.push_back(new_line);

  // TODO: Test for overflow and call subroutine

  return lines;
}

deque<Line> InstructionJmp::translate(Line original)
{
  // JMP OP -> jmp OP
  deque<string> operands;
  deque<Line> lines;

  operands.push_back(original.operand_list.front());
  Line new_line(original.number, original.label, "jmp", operands);
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionJmpn::translate(Line original)
{
  // JMPN OP -> cmp eax, 0 ; generates flags | jl OP ; jumps if negative
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("eax");
  operands.push_back("0");
  Line new_line(original.number, original.label, "cmp", operands);
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original.operand_list.front());
  new_line.label = "";
  new_line.operation = "jl";
  new_line.operand_list = operands;
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionJmpp::translate(Line original)
{
  // JMPP OP -> cmp eax, 0 ; generates flags | jg OP ; jumps if positive
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("eax");
  operands.push_back("0");
  Line new_line(original.number, original.label, "cmp", operands);
  lines.push_back(new_line);

  operands.clear();
  operands.push_back(original.operand_list.front());
  new_line.label = "";
  new_line.operation = "jg";
  new_line.operand_list = operands;
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionJmpz::translate(Line original)
{
  // JMPZ OP -> jz OP ; jumps if zero
  deque<string> operands;
  deque<Line> lines;

  operands.push_back(original.operand_list.front());
  Line new_line(original.number, original.label, "jz", operands);
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionCopy::translate(Line original)
{
  // COPY OP1, OP2 -> mov ebx, dword [OP1] | mov dword [OP2], ebx
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("ebx");
  operands.push_back("dword [" + original.operand_list[0] + "]");
  Line new_line(original.number, original.label, "mov", operands);
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("dword [" + original.operand_list[1] + "]");
  operands.push_back("ebx");
  new_line.label = "";
  new_line.operand_list = operands;
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionLoad::translate(Line original)
{
  // LOAD OP -> mov eax, dword [OP]
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("eax");
  operands.push_back("dword [" + original.operand_list[0] + "]");
  Line new_line(original.number, original.label, "mov", operands);
  lines.push_back(new_line);

  return lines;
}

deque<Line> InstructionStore::translate(Line original)
{
  // STORE OP -> mov dword [OP], eax
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("dword [" + original.operand_list[0] + "]");
  operands.push_back("eax");
  Line new_line(original.number, original.label, "mov", operands);
  lines.push_back(new_line);

  return lines;
}

// TODO: determinar certinho como chamar as subrotinas
deque<Line> InstructionInput::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: determinar certinho como chamar as subrotinas
deque<Line> InstructionOutput::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: determinar certinho como chamar as subrotinas
deque<Line> InstructionCInput::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: determinar certinho como chamar as subrotinas
deque<Line> InstructionCOutput::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: determinar certinho como chamar as subrotinas
deque<Line> InstructionSInput::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: determinar certinho como chamar as subrotinas
deque<Line> InstructionSOutput::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

deque<Line> InstructionStop::translate(Line original)
{
  // STOP -> mov eax, 0 | mov ebx, 0 | int 0x80
  deque<string> operands;
  deque<Line> lines;

  operands.push_back("eax");
  operands.push_back("0");
  Line new_line(original.number, original.label, "mov", operands);
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("ebx");
  operands.push_back("0");
  new_line.label = "";
  new_line.operand_list = operands;
  lines.push_back(new_line);

  operands.clear();
  operands.push_back("80h");
  new_line.operand_list = operands;
  new_line.operation = "int";
  lines.push_back(new_line);

  return lines;
}

// TODO: implementar SECTION
deque<Line> InstructionSection::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: implementar SPACE
deque<Line> InstructionSpace::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}

// TODO: implementar CONST
deque<Line> InstructionConst::translate(Line original)
{
  Line new_line(original);
  new_line.label = "placeholder_"+original.label;
  deque<Line> lines;
  lines.push_back(original);
  return lines;
}
