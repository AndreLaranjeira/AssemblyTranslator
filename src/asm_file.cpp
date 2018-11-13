// Assembly file module - Source code.

// Includes:
#include "asm_file.hpp"

// Class methods:
AsmFile::AsmFile(string p_file_name, string p_file_ext, ios::openmode mode) {
  file_name = p_file_name;
  file_ext = p_file_ext;

  file_pointer.open(file_name + file_ext, mode);

  if(!file_pointer.is_open()) {

    // Now we just need a class or function for printing errors.

  }

}

AsmFile::~AsmFile() {
  if(file_pointer.is_open())
    file_pointer.close();
}

list <Line> AsmFile::parse_file() {

  list <Line> parse_result;

  regex command("^(?:(.*): ?)?([^ :]*)(?: (.*))?$");
  smatch search_matches;
  string file_line, formated_line, label, operation, operands;

  while(getline(file_pointer, file_line)) {

    formated_line = format_line(file_line);

    if(regex_search(formated_line, search_matches, command)) {

      label = search_matches[1].str();
      operation = search_matches[2].str();
      operands = search_matches[3].str();

      // Now all we need to do is create a Line with the search_matches.
      // And append it to parse_result

    }

  }

  return parse_result;

}

// Private methods:
list <string> AsmFile::split_string(string delimeter, string input) {

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

string AsmFile::format_line(string line) {

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
