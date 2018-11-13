// Translator module - Source code.

// Includes:
#include "translator.hpp"

// Class methods:
Translator::Translator() {
  // IDEA Perhaps we should use a singleton design pattern here.
}

Translator::~Translator() {
  for(auto const& pair : opcodes_table)
    delete opcodes_table[pair.first];
}

void translate_file(string input_name, string output_name) {

}

// Debug methods:
void print_asm_buffer() {

}

void print_ia32_buffer() {

}

// Setters:
void Translator::set_debug_mode(bool value) {
  debug_mode = value;
}

// Private methods:
void append_sub_routines() {

}

void load_asm_buffer() {

}
