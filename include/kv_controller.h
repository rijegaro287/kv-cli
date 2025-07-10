#pragma once

#include "kv_parser.h"
#include "linked_list.h"
#include "logger.h"
#include "constants.h"

typedef struct _db_t {
  uint8_t storage_type[SM_BUFFER_SIZE];
  void *storage;
} db_t;

static int64_t copy_file(uint8_t *file_path, uint8_t *copy_path);
extern db_t* create_db(uint8_t *storage_type);
extern int64_t load_db(db_t *db, uint8_t *file_path);
extern int64_t save_db(db_t *db, uint8_t *file_path);
extern int64_t insert_entry(db_t *db, db_entry_t *entry);
extern int64_t put_entry(db_t *db, uint8_t *key, uint8_t *value, uint8_t *type);
extern int64_t delete_entry(db_t *db, uint8_t *key);
extern db_entry_t* get_entry_by_idx(db_t *db, uint64_t idx);
extern db_entry_t* get_entry_by_key(db_t *db, uint8_t *key);
extern void free_db(db_t *db);
extern void print_db(db_t *db);