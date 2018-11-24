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
  ERROR,
  WARNING
} ErrorSeverity;

typedef enum {
  ARGS = 1,
  IO = 2,
  DATA_OVERWRITE = 3,
  INSTRUCTION = 4,
  PRE_PROCESSING = 5,
  TRANSLATION = 6,
  OTHER = 7
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
    int log_error(ErrorSeverity, ErrorType, string = "", int = -1);

};

#endif // ERROR_H_
