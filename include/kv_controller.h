#pragma once

#include <stdint.h>

#include "constants.h"
#include "kv_parser.h"
#include "linked_list.h"

static uint8_t storage_structure[2] = KV_STORAGE_STRUCTURE;

static void *create_db(uint8_t *storage_structure);

extern db_entry_t get_entry(void *db_ptr, uint8_t *key);
extern int64_t insert_entry(void *db_ptr, db_entry_t *entry_ptr);
extern int64_t load_db(uint8_t *file_path, void *dest, uint8_t *storage_structure);

extern void print_db(void *db_ptr);