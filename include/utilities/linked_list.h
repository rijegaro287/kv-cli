#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "kv_parser.h"

typedef struct _node_t {
  db_entry_t *entry;
  struct _node_t* next;
} node_t;

typedef struct _list_t {
  uint64_t size;
  node_t* head;
} list_t;

extern list_t* create_list();

extern int64_t list_insert(list_t* list, db_entry_t *entry);

extern db_entry_t *list_get_by_idx(list_t* list, uint64_t idx);

extern db_entry_t *list_get_by_key(list_t* list, uint8_t *idx);

extern void list_update(list_t* list, db_entry_t *entry, uint64_t idx);

extern void list_delete(list_t* list, uint64_t idx);

extern void list_clean(list_t* list);

extern void list_print(list_t* list);
