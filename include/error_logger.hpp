// Error logger module - Header file.

// Define guard:
#ifndef ERROR_H_
#define ERROR_H_

// Includes:
#include <iostream>
#include <string>

// Namespace:
using namespace std;

// Enumerations:
typedef enum {
  FATAL,
  STANDARD,
  WARNING
} ErrorSeverity;

typedef enum {
  ARGS = 1,
  IO = 2,
  TRANSLATION = 3,
  INSTRUCTION = 4,
  OVERFLOW = 5,
  OTHER = 6
} ErrorType;

// Class headers:
class ErrorLogger {

  private:
    // Variables:
      string context;

  public:
    // Class methods:
    ErrorLogger(string);
    ~ErrorLogger();
    int print_error(ErrorSeverity, ErrorType, string = "", int = -1);

};

#endif // ERROR_H_
