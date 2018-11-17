// Error logger module - Source code.

// Includes:
#include "error_logger.hpp"

// Class methods:
ErrorLogger::ErrorLogger(string p_context) {
  context = p_context;
}

ErrorLogger::~ErrorLogger() {

}

int ErrorLogger::log_error(ErrorSeverity severity, ErrorType type, string msg,
                           int line_num) {

  cerr << endl;

  switch (severity) {

    case FATAL:
      cerr << "[FATAL ERROR] ";
      break;

    case ERROR:
      cerr << "[ERROR] ";
      break;

    case WARNING:
      cerr << "[WARNING] ";
      break;

  }

  switch (type) {

    case ARGS:
      cerr << "Invalid number of arguments given to program!" << endl;
      break;

    case IO:
      cerr << "Could not open a given file!" << endl;
      break;

    case INSTRUCTION:
      cerr << "Invalid instruction found in assembly code!" << endl;
      break;

    case PRE_PROCESSING:
      cerr << "Could not pre-process a given file!" << endl;
      break;

    case TRANSLATION:
      cerr << "Could not translate a given file!" << endl;
      break;

    case OTHER:
      cerr << "Unknown error!" << endl;
      break;

  }

  cerr << "-- Context: " << context << endl;

  if(msg != "")
    cerr << "-- Info: " << msg << endl;

  if(line_num >= 0)
    cerr << "-- Line: " << line_num << endl;

  cerr << endl;

  return 0;

}
