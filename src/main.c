#include <stdio.h>

#include "kv_parser.h"
#include "string_conversion.h"

int main(int argc, char const *argv[]) {
  FILE *db_ptr = fopen(TEST_DB_PATH, "r+");
  if (db_ptr == NULL) {
    perror("Error: Failed to read the database file.\n");
    return -1;
  }

  uint8_t line_buffer[LINE_BUFFER_SIZE];
  // if (fgets(line_buffer, LINE_BUFFER_SIZE, db_ptr) == NULL) {
    //   perror("Error: Failed to read line from file\n");
    // }
    
  while (fgets(line_buffer, LINE_BUFFER_SIZE, db_ptr) != NULL) {
    printf("==============================\n");
    printf("%s", line_buffer);
    
    if (strcmp(line_buffer, "\n") == 0 || line_buffer[0] == '#') continue;

    uint8_t *type, *key, *value;
    type = strtok(line_buffer, ":");
    key = strtok(NULL, "=");
    value = strtok(NULL, "\n");

    if (type == NULL | key == NULL | value == NULL) continue;

    printf("Str Type: %s, Str Key: %s, Str Value: %s\n", type, key, value);

    db_entry_t *entry = create_db_entry(type, key, value);
    if (entry->type < 0) continue;

    print_entry(entry);
    printf("==============================\n");
  }


  if (fclose(db_ptr) == EOF) {
    perror("Error: Failed to close the database file\n");
    return -1;
  }

  return 0;
}
