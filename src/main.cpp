// Program that translates a made-up assembly language to IA-32 assembly.

// Includes:
#include <iostream>
#include <string>

#include "asm_file.hpp"
#include "error_logger.hpp"
#include "translator.hpp"

// Namespace:
using namespace std;

// Macros:
#define DEBUG true
#define FUNCTIONS_FILE "src/functions"

// Enums:
typedef enum {
  WRONG_NUM_OF_ARGS = 1,
  CANNOT_OPEN_INPUT_FILE = 2,
  PRE_PROCESSING_FAILED = 3,
  TRANSLATION_FAILED = 4,
  CANNOT_OPEN_FUNCTIONS_FILE = 5,
  CANNOT_OPEN_OUTPUT_FILE = 6
} ExitTypes;

// Function headers:

// Main function:
int main(int argc, char const *argv[]) {

  // Variable declarations:
  AsmFile *functions, *input, *output;
  ErrorLogger error_logger("Main.cpp");
  string message;
  Translator translator("IA-32 translator");

  translator.set_debug_mode(DEBUG); // Sets up possible debug mode.

  // Check the number of arguments.

  if(argc != 2) {
      error_logger.log_error(FATAL, ARGS, "./tradutor programa");
      exit(WRONG_NUM_OF_ARGS);
  }

  // Load input file.

  input = new AsmFile(argv[1], ".asm", ios::in);

  if(!input->is_open()) {
    message = "File name is " + (string) argv[1] + ".asm";
    error_logger.log_error(FATAL, IO, message);
    delete input;
    exit(CANNOT_OPEN_INPUT_FILE);
  }

  // Pre-process input file.

  if(input->pre_process(true) != 0) {
    message = "File name is " + (string) argv[1] + ".asm";
    error_logger.log_error(FATAL, PRE_PROCESSING, message);
    delete input;
    exit(PRE_PROCESSING_FAILED);
  }

  // Load input file contents in the translator.

  if(DEBUG)
    input->print_buffer();

  cout << "::Loading translator parameters..." << endl;

  translator.set_asm_buffer(input->get_buffer());

  delete input;

  // Load instructions into translator.
  translator.load_default_mnemonics();

  cout << "::Translator parameters successfully loaded!" << endl;

  if(DEBUG)
    translator.print_instructions();

  // Translate file.

  translator.translate_asm_buffer();

  // Append hand-written functions.

  cout << "::Appending auxiliary functions..." << endl;

  functions = new AsmFile(FUNCTIONS_FILE, ".asm", ios::in);

  if(!functions->is_open()) {
    message = "File name is " + (string) FUNCTIONS_FILE + ".asm";
    error_logger.log_error(FATAL, IO, message);
    delete functions;
    exit(CANNOT_OPEN_FUNCTIONS_FILE);
  }

  functions->parse_file(false);
  translator.append_functions(functions->get_buffer());

  delete functions;

  cout << "::Auxiliary functions successfully appended!" << endl;

  // Generate output file.

  cout << "::Generating output file..." << endl;

  output = new AsmFile(argv[1], ".s", ios::out);

  if(!output->is_open()) {
    message = "File name is " + (string) argv[1] + ".s";
    error_logger.log_error(FATAL, IO, message);
    delete output;
    exit(CANNOT_OPEN_OUTPUT_FILE);
  }

  output->set_buffer(translator.get_ia32_buffer());
  output->write_buffer_contents();

  delete output;

  cout << "::Output file successfully generated!" << endl;

  cout << "::File translation was successful!" << endl << endl;

  return 0;

}

// Function implementations:
