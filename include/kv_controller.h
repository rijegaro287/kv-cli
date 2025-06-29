#pragma once

#include <stdint.h>

#include "constants.h"
#include "kv_parser.h"
#include "linked_list.h"

typedef struct _db_t {
  uint64_t id;
  void *storage;
} db_t;

static uint8_t storage_structure[2] = KV_STORAGE_STRUCTURE;
extern db_entry_t *get_by_idx(db_t *db_ptr, uint64_t idx);
extern db_entry_t *get_by_key(db_t *db_ptr, uint8_t *key);
extern int64_t insert_entry(db_t *db_ptr, db_entry_t *entry_ptr);
extern db_t *create_db(uint8_t *storage_structure);
extern int64_t load_db(db_t *dest, uint8_t *file_path, uint8_t *storage_structure);

extern void print_db(db_t *db_ptr);
