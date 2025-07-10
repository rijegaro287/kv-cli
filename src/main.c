#include <stdio.h>

#include "kv_controller.h"
#include "kv_cli.h"

#include "hash_table.h"

int main(int argc, char const *argv[]) {
  // start_cli();
  hash_table_t *hash = create_hash_table(4);
  db_entry_t *entry1 = create_entry("key1", "0", "int8");
  db_entry_t *entry2 = create_entry("key2", "2", "int8");
  db_entry_t *entry3 = create_entry("key3", "3", "int8");
  db_entry_t *entry4 = create_entry("key4", "4", "int8");
  db_entry_t *entry5 = create_entry("key5", "5", "int8");
  db_entry_t *entry6 = create_entry("key6", "6", "int8");

  hash_insert(hash, entry1);
  hash_insert(hash, entry2);
  hash_insert(hash, entry3);
  hash_insert(hash, entry4);
  hash_insert(hash, entry5);
  hash_insert(hash, entry6);

  printf("%s\n", (hash_get_entry(hash, "key4"))->key);
  hash_print(hash);

  hash_delete(hash, "key2");

  printf("----\n");
  hash_print(hash);

  FILE *db_file = fopen(TEST_DB_PATH, "w");
  hash_save(db_file, hash);

  free_hash_table(hash);
  printf("odkeodke");
  return 0;
}
