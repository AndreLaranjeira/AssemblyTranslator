#include <sstream>
#include <deque>
#include "line.hpp"

using namespace std;

Line::Line(int p_number, string p_label, string p_operation,
           deque <string> p_operand_list) {
  number = p_number;
  label = p_label;
  operation = p_operation;
  operand_list = p_operand_list;
  comment = "";
}

string Line::to_string()
{
  stringstream output;
  string out;
  if(label != "")
    output << label << ":\t";

  if(operation != "")
    output << operation;

  for(auto iter = operand_list.begin(); iter != operand_list.end();
      iter++) {

    if (iter != operand_list.begin())
      output << ", ";

    else
      output << " ";

    output << *iter;

  }
  if(comment != "")
    output << " ; " << comment;

  out = output.str();
  return out;
}
