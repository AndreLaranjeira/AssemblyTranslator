// Translator module - Header file.

// Define guard:
#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

// Includes:
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

#include "error_logger.hpp"
#include "line.hpp"

// Namespace:
using namespace std;

// Structs:
struct Instruction {
  bool needs_label;
  unsigned int min_num_par;
  unsigned int max_num_par;

  Instruction(bool p_needs_label, unsigned int p_min_num_par,
              unsigned int p_max_num_par) {
    needs_label = p_needs_label;
    min_num_par = p_min_num_par;
    max_num_par = p_max_num_par;
  }

};

// Class headers:
class Translator {

  private:
    // Variables:
    bool debug_mode;
    deque <Line> asm_buffer;
    deque <string> ia32_buffer;
    ErrorLogger error_logger;
    map <string, Instruction> instructions_table;
    string name;

    // Methods:
    void append_sub_routines();

  public:
    // Class methods:
    Translator(string);
    ~Translator();
    int add_instruction(string, bool, unsigned int, unsigned int);
    void translate_buffer();

    // Debug methods:
    void print_asm_buffer();
    void print_ia32_buffer();
    void print_instructions();

    // Getters:

    // Setters:
    void set_asm_buffer(deque <Line>);
    void set_debug_mode(bool);

};

#endif // TRANSLATOR_H_
