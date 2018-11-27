// Line class module - Header file.

// Define guard:
#ifndef LINE_H_
#define LINE_H_

// Includes:
#include <string>
#include <deque>

// Namespace:
using namespace std;

// Structs:
class Line {
public:
  int number;
  string label;
  string operation;
  deque <string> operand_list;
  string comment;

  Line(int p_number, string p_label, string p_operation,
       deque <string> p_operand_list);

  string to_string();
};

#endif // LINE_H_
