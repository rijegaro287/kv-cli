#pragma once

#include <stdint.h>

#include "constants.h"
#include "kv_parser.h"
#include "linked_list.h"

typedef struct _db_t {
  uint64_t id;
  uint8_t storage_type[CLI_CMD_BUFFER_SIZE];
  void *storage;
} db_t;

extern db_entry_t *get_by_idx(db_t *db_ptr, uint64_t idx);
extern db_entry_t *get_by_key(db_t *db_ptr, uint8_t *key);
extern int64_t insert_entry(db_t *db_ptr, db_entry_t *entry_ptr);
extern db_t *create_db(uint8_t *storage_type);
extern int64_t load_db(db_t *db_ptr, uint8_t *file_path);
extern void free_db(db_t *db);


extern void print_db(db_t *db_ptr);
