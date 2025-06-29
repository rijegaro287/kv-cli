#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "kv_parser.h"

static void read_line(const uint8_t *line[LINE_BUFFER_SIZE], uint8_t *dest) {

}

int main(int argc, char const *argv[]) {
  FILE *database_ptr = fopen(TEST_DB_PATH, "r+");
  if (database_ptr == NULL) {
    perror("Error: Failed to read the database file.");
    return -1;
  }

  uint8_t line_buffer[LINE_BUFFER_SIZE];
  // if (fgets(line_buffer, LINE_BUFFER_SIZE, database_ptr) == NULL) {
    //   perror("Error: Failed to read line from file");
    // }
    
  while (fgets(line_buffer, LINE_BUFFER_SIZE, database_ptr) != NULL) {
    printf("%s", line_buffer);
  }


  if (fclose(database_ptr) == EOF) {
    perror("Error: Failed to close the database file");
    return -1;
  }

  return 0;
}
