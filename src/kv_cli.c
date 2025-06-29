#include "kv_cli.h"

extern void start_cli() {
  if (open_db(TEST_DB_PATH, "L") < 0) {
    perror("Error: Failed to open database\n");
    return;
  }

  print_db(db_list[0]);
}

extern int64_t open_db(uint8_t *db_path, uint8_t *storage_structure) {
  if (db_count >= KV_CLI_MAX_OPEN_DATABASES) {
    perror("Reached max number of open databases. Nothing done");
    return -1;
  }

  db_t *new_db = create_db(storage_structure);
  if (new_db == NULL) {
    perror("Error: Failed to create new db\n");
    return -1;
  }
  new_db->id = db_count;
  
  if (load_db(new_db, db_path, storage_structure) < 0) {
    perror("Error: Failed to load db into memory\n");
    return -1;
  }
  
  db_list[db_count] = new_db;
  db_count++;
  
  return 0;
}
