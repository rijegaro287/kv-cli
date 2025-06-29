#include <stdio.h>

#include "kv_controller.h"

int main(int argc, char const *argv[]) {
  void *db_ptr = malloc(sizeof(void));
  if (load_db(TEST_DB_PATH, db_ptr, storage_structure) != 0) {
    perror("Error: Failed to load database into memory");
  }

  /* Run CLI Loop */

  return 0;
}
