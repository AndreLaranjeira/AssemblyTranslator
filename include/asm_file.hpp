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
#include "line.hpp"

// Namespace:
using namespace std;

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
    int parse_file(bool format_case = false);
    int pre_process(bool format_case = false);
    int write_buffer_contents();

    // Debug methods:
    void print_buffer();

    // Getters:
    deque <Line> get_buffer();

    // Setters:
    void set_buffer(deque <Line>);

};

#endif // ASSEMBLY_FILE_H_
