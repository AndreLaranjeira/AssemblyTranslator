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
#define DEBUG false

// Function headers:

// Main function:
int main(int argc, char const *argv[]) {

  ErrorLogger error_logger("Main.cpp");
  Translator translator;

  translator.set_debug_mode(DEBUG); // Sets up possible debug mode.

  // Check argument number.
  if(argc != 2) {
      error_logger.print_error(FATAL, ARGS, "./tradutor programa");
      exit(ARGS);
  }

  // Load input file.

  // Load operations into translator.

  // Translate file.

  // Generate output file.

  cout << "::File translation was successful!" << endl << endl;

  return 0;

}

// Function implementations:
