// Program that translates a made-up assembly language to IA-32 assembly.

// Includes:
#include <iostream>
#include <string>

#include "asm_file.hpp"
#include "translator.hpp"

// Namespace:
using namespace std;

// Macros:
#define DEBUG false

// Function headers:

// Main function:
int main(int argc, char const *argv[]) {

  Translator translator;

  translator.set_debug_mode(DEBUG); // Sets up possible debug mode.

  // Load operations into translator.

  // Load input file.

  // Translate file.

  // Generate output file.

  cout << "::File translation was successful!" << endl << endl;

  return 0;

}

// Function implementations:
