// Assembly file module - Header file.

// Define guard:
#ifndef ASSEMBLY_FILE_H_
#define ASSEMBLY_FILE_H_

// Includes:
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>

// Namespace:
using namespace std;

// Structs:
struct Line {
  string label;
  string operation;
  list <string> operand_list;

  Line(string p_label, string p_operation, list <string> p_operand_list) {
    label = p_label;
    operation = p_operation;
    operand_list = p_operand_list;
  }

};

// Classes:
class AsmFile {

  private:
    // Variables:
    fstream file_pointer;
    string file_ext;
    string file_name;

    // Methods:
    list <string> split_string(string, string);
    string format_line(string);

  public:
    // Class methods:
    AsmFile(string, string, ios::openmode);
    ~AsmFile();
    list <Line> parse_file();

    // Debug methods:

    // Getters:

    // Setters:

};

#endif // ASSEMBLY_FILE_H_
