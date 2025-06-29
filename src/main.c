#include <stdio.h>

#include "kv_controller.h"
#include "kv_cli.h"

int main(int argc, char const *argv[]) {
  void *db_ptr = malloc(sizeof(void));
  if (load_db(TEST_DB_PATH, db_ptr, storage_structure) != 0) {
    perror("Error: Failed to load database into memory");
  }

  /* Run CLI Loop */
  start_cli(db_ptr);

  return 0;
}
