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

#include "operation.hpp"

// Namespace:
using namespace std;

// Class headers:
class Translator {

  private:
    // Variables:
    bool debug_mode;
    list <pair<unsigned int, string>> asm_buffer;
    list <string> ia32_buffer;
    map <string, Operation*> opcodes_table;

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
