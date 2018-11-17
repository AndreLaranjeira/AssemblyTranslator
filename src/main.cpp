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
  Translator translator("IA-32 traslator");

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

  if(input->pre_process() != 0) {
    message = "File name is " + (string) argv[1] + ".asm";
    error_logger.log_error(FATAL, PRE_PROCESSING, message);
    delete input;
    exit(PRE_PROCESSING_FAILED);
  }

  // Load input file contents in the translator.

  if(DEBUG)
    input->print_buffer();

  cout << "::Loading translator contents..." << endl;

  translator.set_asm_buffer(input->get_buffer());

  delete input;

  // Load instructions into translator.
  translator.add_instruction("ADD", false, 1, 1);
  translator.add_instruction("SUB", false, 1, 1);
  translator.add_instruction("MULT", false, 1, 1);
  translator.add_instruction("DIV", false, 1, 1);
  translator.add_instruction("JMP", false, 1, 1);
  translator.add_instruction("JMPN", false, 1, 1);
  translator.add_instruction("JMPP", false, 1, 1);
  translator.add_instruction("JMPZ", false, 1, 1);
  translator.add_instruction("COPY", false, 2, 2);
  translator.add_instruction("LOAD", false, 1, 1);
  translator.add_instruction("STORE", false, 1, 1);
  translator.add_instruction("INPUT", false, 1, 1);
  translator.add_instruction("OUTPUT", false, 1, 1);
  translator.add_instruction("C_INPUT", false, 1, 1);
  translator.add_instruction("C_OUTPUT", false, 1, 1);
  translator.add_instruction("S_INPUT", false, 2, 2);
  translator.add_instruction("S_OUTPUT", false, 2, 2);
  translator.add_instruction("STOP", false, 0, 0);

  translator.add_instruction("SECTION", false, 1, 1);
  translator.add_instruction("SPACE", true, 0, 1);
  translator.add_instruction("CONST", true, 1, 1);

  cout << "::Translator contents successfully loaded!" << endl;

  if(DEBUG)
    translator.print_instructions();


  // Translate file.

  // Generate output file.

  cout << "::File translation was successful!" << endl << endl;

  // Clean up.

  return 0;

}

// Function implementations:
