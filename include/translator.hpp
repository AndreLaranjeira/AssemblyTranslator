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
#include "instruction.hpp"

// Namespace:
using namespace std;

// Class headers:
class Translator {

  private:
    // Variables:
    bool debug_mode;
    deque <Line*> asm_buffer, ia32_buffer;
    ErrorLogger error_logger;
    map <string, Instruction*> instructions_table;
    string name;

    // Methods:

  public:
    // Class methods:
    Translator(string);
    ~Translator();
    int add_instruction(Instruction*);
    int load_default_mnemonics();
    int append_functions(deque <Line*>);
    int translate_asm_buffer();

    // Debug methods:
    void print_asm_buffer();
    void print_ia32_buffer();
    void print_instructions();

    // Getters:
    deque <Line*> get_ia32_buffer();

    // Setters:
    void set_asm_buffer(deque <Line*>);
    void set_debug_mode(bool);

};

#endif // TRANSLATOR_H_
