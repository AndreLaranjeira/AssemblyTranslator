// Translator's core module - Source code.

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

// Setters:
void Translator::set_debug_mode(bool value) {
  debug_mode = value;
}

// Private methods:
list <string> Translator::split_string(string delimeter, string input) {

  list <string> results;
  size_t position;
  string result;

  while((position = input.find(delimeter)) != string::npos) {
    result = input.substr(0, position);
    results.push_back(result);
    input.erase(0, position + delimeter.size());
  }

  results.push_back(input);

  return results;

}

string Translator::format_line(string line) {

  regex colon(":"), comment(";.*"), first_space("^ "), last_space(" $");
  regex offset_plus1(" \\+"), offset_plus2("\\+ "), spaces_and_tabs("[ \t]+");
  string formated_line;

  // Removes comments and extra tabs and spaces.

  formated_line = regex_replace(line, comment, "");
  formated_line = regex_replace(formated_line, colon, ": ");
  formated_line = regex_replace(formated_line, spaces_and_tabs, " ");
  formated_line = regex_replace(formated_line, offset_plus1, "+");
  formated_line = regex_replace(formated_line, offset_plus2, "+");
  formated_line = regex_replace(formated_line, first_space, "");
  formated_line = regex_replace(formated_line, last_space, "");

  // Converts the whole string to uppercase.

  for (auto & c: formated_line)
    c = toupper(c);

  return formated_line;

}
