// Assembly file module - Source code.

// Includes:
#include "asm_file.hpp"

// Class methods:
AsmFile::AsmFile(string p_file_name, string p_file_ext, ios::openmode mode):
         error_logger(p_file_name) {

  file_name = p_file_name;
  file_ext = p_file_ext;

  file_pointer.open(file_name + file_ext, mode);

}

AsmFile::~AsmFile() {
  if(file_pointer.is_open())
    file_pointer.close();
}

bool AsmFile::is_open() {
  return file_pointer.is_open();
}

int AsmFile::parse_file() {

  bool parse_error = false;
  deque <string> operand_list;
  int line_num;
  regex command("^(?:(.*): ?)?([^ :]*)(?: (.*))?$");
  smatch search_matches;
  string condition, file_line, formated_line, label, operation, operands;

  cout << "::Parsing " << file_name << file_ext << " ..." << endl;

  // Clear AsmFile buffer to receive results.
  buffer.clear();

  // Start line counter.
  line_num = 1;

  // Iterate over the original code file
  while (getline(file_pointer, file_line)) {

    // Removes comments and replaces extra spaces
    formated_line = format_line(file_line);

    if(regex_search(formated_line, search_matches, command)) {

      label = search_matches[1].str();
      operation = search_matches[2].str();
      operands = search_matches[3].str();
      operand_list = split_string(", ", operands);

      if(formated_line != "")
        buffer.push_back(Line(line_num, label, operation, operand_list));

    }

    else
      error_logger.log_error(ERROR, INSTRUCTION,
                  "Instruction didn't match needed format.", line_num);

    line_num++;

  }

  if(parse_error) {
    cout << "::Parsing " << file_name << file_ext << " failed!" << endl;
    return -1;
  }

  cout << "::Parsing " << file_name << file_ext << " was successful!" << endl;
  return 0;

}

int AsmFile::pre_process() {

  bool pre_error = false;
  deque <string> operand_list;
  int line_num;
  regex command("^(?:(.*): ?)?([^ :]*)(?: (.*))?$");
  regex signed_number("-?[0-9]+");
  smatch search_matches;
  string condition, file_line, formated_line, label, operation, operands;

  cout << "::Pre-processing " << file_name << file_ext << " ..." << endl;

  // Clear AsmFile buffer to receive results.
  buffer.clear();

  // Start line counter.
  line_num = 1;

  // Iterate over the original code file
  while (getline(file_pointer, file_line)) {

    // Removes comments and replaces extra spaces
    formated_line = format_line(file_line);

    if(regex_search(formated_line, search_matches, command)) {

      label = search_matches[1].str();
      operation = search_matches[2].str();
      operands = search_matches[3].str();
      operand_list = split_string(", ", operands);

      operand_list = replace_aliases(operand_list, aliases_table);

      if(operation == "EQU") {

        // Label redefined.

        if(aliases_table.count(label) > 0)  {
          error_logger.log_error(ERROR, INSTRUCTION,
                                 "EQU directive redefines label.", line_num);
          pre_error = true;
        }

        // Invalid label.

        else if(!valid_label(label)) {
          error_logger.log_error(ERROR, INSTRUCTION,
                                 "EQU directive uses invalid label.", line_num);
          pre_error = true;
        }

        // The value of an alias should always be a signed number.

        else if(!regex_match(operand_list[0], signed_number)) {
          error_logger.log_error(ERROR, INSTRUCTION,
                       "EQU directive aliases only signed numbers.", line_num);
          pre_error = true;
        }

        else
          aliases_table[label] = operand_list[0];

      }

      else if(operation == "IF") {

        condition = operand_list[0];

        // We might get a label before the IF statement.

        if(label != "") {
          error_logger.log_error(ERROR, INSTRUCTION,
                       "A label was placed before an IF directive!", line_num);
          pre_error = true;
        }

        // Invalid IF condition.

        else if(condition != "1" && condition != "0") {
          error_logger.log_error(ERROR, INSTRUCTION,
                      "IF directive has an invalid condition!", line_num);
          pre_error = true;
        }

        // If condition == 0, discard a line.

        else if(condition == "0" && !file_pointer.eof()) {
          getline(file_pointer, file_line);  // Discard the next line;
          line_num++;
        }

        // If condition == "1", then we don't need to do anything!

      }

      else if(formated_line != "")
        buffer.push_back(Line(line_num, label, operation, operand_list));

    }

    else
      error_logger.log_error(ERROR, INSTRUCTION,
                  "Instruction didn't match needed format.", line_num);

    line_num++;

  }

  if(pre_error) {
    cout << "::Pre-processing " << file_name << file_ext << " failed!" << endl;
    return -1;
  }

  cout << "::Pre-processing " << file_name << file_ext << " was successful!";
  cout << endl;
  return 0;

}

int AsmFile::print_buffer() {

  cout << endl;
  cout << "[DEBUG] Buffer contents for file " + file_name + file_ext << endl;

  for(auto const& line : buffer) {
    cout << line.number << " - ";

    if(line.label != "")
      cout << line.label << ": ";

    if(line.operation != "")
      cout << line.operation;

    for(auto iter = line.operand_list.begin(); iter != line.operand_list.end();
        iter++) {

      if (iter != line.operand_list.begin())
        cout << ", ";

      else
        cout << " ";

      cout << *iter;

    }

    cout << endl;

  }

  cout << endl;

  return 0;

}

// Getters:
deque <Line> AsmFile::get_buffer() {
  return buffer;
}

// Private methods:
bool AsmFile::valid_label(string label) {

  bool valid = true;
  regex valid_chars("[a-zA-Z0-9_]+");

  if(label.length() > 50 || !isalpha(label.at(0))
     || !regex_match(label, valid_chars))
    valid = false;

  return valid;

}

deque <string> AsmFile::replace_aliases(deque <string> operand_list,
                                        map <string, string> aliases_table) {

  deque <string> results;
  string word;

  for(auto const& operand : operand_list) {

    word = "";

    for(auto const& pair : aliases_table) {
      if(operand == pair.first) {
        word = pair.second;
        break;
      }
    }

    if(word != "")
      results.push_back(word);

    else
      results.push_back(operand);

  }

  return results;

}

deque <string> AsmFile::split_string(string delimeter, string input) {

  deque <string> results;
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
