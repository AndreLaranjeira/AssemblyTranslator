// Translator module - Header file.

// Define guard:
#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

// Includes:
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <string>

#include "error_logger.hpp"

// Namespace:
using namespace std;

// Structs:
struct Instruction {
  bool needs_label;
  unsigned int num_parameters;

  Instruction(bool p_needs_label, unsigned int p_num_parameters) {
    needs_label = p_needs_label;
    num_parameters = p_num_parameters;
  }

};

// Class headers:
class Translator {

  private:
    // Variables:
    bool debug_mode;
    list <pair<unsigned int, string>> asm_buffer;
    list <string> ia32_buffer;
    map <string, Instruction*> instructions_table;

    // Methods:
    void append_sub_routines();
    void load_asm_buffer();

  public:
    // Class methods:
    Translator();
    ~Translator();
    void translate_file(string, string);

    // Debug methods:
    void print_asm_buffer();
    void print_ia32_buffer();

    // Getters:

    // Setters:
    void set_debug_mode(bool);

};

#endif // TRANSLATOR_H_
