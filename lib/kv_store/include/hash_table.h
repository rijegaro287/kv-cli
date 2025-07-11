#pragma once

#include "linked_list.h"

typedef struct _hash_table_t {
  list_t **content;
  uint64_t size;
} hash_table_t;

static int64_t calculate_hash_code(uint8_t *key, uint64_t size);

extern hash_table_t* create_hash_table(uint64_t size);
extern int64_t hash_insert(hash_table_t *hash, db_entry_t *entry);
extern int64_t hash_put(hash_table_t *hash, uint8_t *key, uint8_t* value, uint8_t* type);
extern int64_t hash_delete(hash_table_t *hash, uint8_t *key);
extern db_entry_t *hash_get_entry(hash_table_t *hash, uint8_t *key);
extern int64_t hash_save(FILE *file, hash_table_t *hash);
extern void free_hash_table(hash_table_t *hash);
extern void hash_print(hash_table_t *hash);
