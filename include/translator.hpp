// Translator's core module - Header file.

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

// Namespace:
using namespace std;

// Class headers:
class Translator {

  private:
    // Variables:
    bool debug_mode;
    list <pair<unsigned int, string>> buffer;
    map <string, Operation*> opcodes_table;

    // Methods:
    list <string> split_string(string, string)
    string format_line(string);

  public:
    // Class methods:
    Translator();
    ~Translator();

    // Getters:

    // Setters:
    void set_debug_mode(bool);

};

#endif // TRANSLATOR_H_
