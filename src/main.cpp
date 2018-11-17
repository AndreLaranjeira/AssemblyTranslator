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

// Enums:
typedef enum {
  WRONG_NUM_OF_ARGS = 1,
  CANNOT_OPEN_INPUT_FILE = 2,
  PRE_PROCESSING_FAILED = 3,
  TRANSLATION_FAILED = 4,
  CANNOT_OPEN_OUTPUT_FILE = 5
} ExitTypes;

// Function headers:

// Main function:
int main(int argc, char const *argv[]) {

  // Variable declarations:
  AsmFile *input, *output;
  ErrorLogger error_logger("Main.cpp");
  string message;
  Translator translator;

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

  if(input->pre_process() != 0) {
    message = "File name is " + (string) argv[1] + ".asm";
    error_logger.log_error(FATAL, PRE_PROCESSING, message);
    delete input;
    exit(PRE_PROCESSING_FAILED);
  }

  if(DEBUG)
    input->print_buffer();

  delete input;

  // Load operations into translator.

  // Translate file.

  // Generate output file.

  cout << "::File translation was successful!" << endl << endl;

  // Clean up.

  return 0;

}

// Function implementations:
