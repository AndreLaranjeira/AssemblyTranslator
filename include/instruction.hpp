#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <deque>
#include "line.hpp"

using namespace std;

class Instruction {
public:
  string name;
  bool needs_label;
  unsigned int min_num_par;
  unsigned int max_num_par;

  Instruction(string, bool, unsigned int, unsigned int);
  virtual ~Instruction(){} // empty destructor

  virtual deque<Line> translate(Line);
  bool validate(Line);

};

class InstructionAdd:public Instruction {
public:
  InstructionAdd() : Instruction("ADD", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionSub:public Instruction {
public:
  InstructionSub() : Instruction("SUB", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionMult:public Instruction {
public:
  InstructionMult() : Instruction("MULT", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionDiv:public Instruction {
public:
  InstructionDiv() : Instruction("DIV", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionJmp:public Instruction {
public:
  InstructionJmp() : Instruction("JMP", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionJmpn:public Instruction {
public:
  InstructionJmpn() : Instruction("JMPN", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionJmpp:public Instruction {
public:
  InstructionJmpp() : Instruction("JMPP", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionJmpz:public Instruction {
public:
  InstructionJmpz() : Instruction("JMPZ", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionCopy:public Instruction {
public:
  InstructionCopy() : Instruction("COPY", false, 2, 2){}
  deque<Line> translate(Line);
};

class InstructionLoad:public Instruction {
public:
  InstructionLoad() : Instruction("LOAD", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionStore:public Instruction {
public:
  InstructionStore() : Instruction("STORE", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionInput:public Instruction {
public:
  InstructionInput() : Instruction("INPUT", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionOutput:public Instruction {
public:
  InstructionOutput() : Instruction("OUTPUT", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionCInput:public Instruction {
public:
  InstructionCInput() : Instruction("C_INPUT", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionCOutput:public Instruction {
public:
  InstructionCOutput() : Instruction("C_OUTPUT", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionSInput:public Instruction {
public:
  InstructionSInput() : Instruction("S_INPUT", false, 2, 2){}
  deque<Line> translate(Line);
};

class InstructionSOutput:public Instruction {
public:
  InstructionSOutput() : Instruction("S_OUTPUT", false, 2, 2){}
  deque<Line> translate(Line);
};

class InstructionStop:public Instruction {
public:
  InstructionStop() : Instruction("STOP", false, 0, 0){}
  deque<Line> translate(Line);
};

class InstructionSection:public Instruction {
public:
  InstructionSection() : Instruction("SECTION", false, 1, 1){}
  deque<Line> translate(Line);
};

class InstructionSpace:public Instruction {
public:
  InstructionSpace() : Instruction("SPACE", true, 0, 1){}
  deque<Line> translate(Line);
};

class InstructionConst:public Instruction {
public:
  InstructionConst() : Instruction("CONST", true, 1, 1){}
  deque<Line> translate(Line);
};

#endif /* INSTRUCTION_H_ */
