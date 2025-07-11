#include "logger.h"

extern void set_log_level(uint64_t log_level) {
  LOG_LEVEL = log_level;
}

extern void logger(uint64_t log_level, uint8_t* message, ...) {
  if (message == NULL) {
    printf("Error: NULL message passed to logger\n");
    return;
  }
  
  va_list argp;
  va_start(argp, message);

  if (log_level >= LOG_LEVEL) {
    vprintf(message, argp);
  }

  va_end(argp);
}
