// Line data strucutre module - Header file.

// Define guard:
#ifndef LINE_H_
#define LINE_H_

// Includes:
#include <string>

// Namespace:
using namespace std;

// Structs:
struct Line {
  int number;
  string label;
  string operation;
  deque <string> operand_list;

  Line(int p_number, string p_label, string p_operation,
       deque <string> p_operand_list) {
    number = p_number;
    label = p_label;
    operation = p_operation;
    operand_list = p_operand_list;
  }

};

#endif // LINE_H_
