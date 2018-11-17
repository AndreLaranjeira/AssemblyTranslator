// Assembly file module - Header file.

// Define guard:
#ifndef ASSEMBLY_FILE_H_
#define ASSEMBLY_FILE_H_

// Includes:
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

#include "error_logger.hpp"

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

// Classes:
class AsmFile {

  private:
    // Variables:
    deque <Line> buffer;
    ErrorLogger error_logger;
    fstream file_pointer;
    map <string, string> aliases_table;
    string file_ext;
    string file_name;

    // Methods:
    bool valid_label(string);
    deque <string> replace_aliases(deque <string>, map <string, string>);
    deque <string> split_string(string, string);
    string format_line(string);

  public:
    // Class methods:
    AsmFile(string, string, ios::openmode);
    ~AsmFile();
    bool is_open();
    int parse_file();
    int pre_process();
    int print_buffer();

    // Getters:
    deque <Line> get_buffer();

};

#endif // ASSEMBLY_FILE_H_
