#pragma once

#include "kv_parser.h"
#include "logger.h"

typedef struct _node_t {
  db_entry_t *entry;
  struct _node_t* next;
} node_t;

typedef struct _list_t {
  uint64_t size;
  node_t* head;
} list_t;

extern list_t* create_list();
extern int64_t list_insert(list_t *list, db_entry_t *entry);
extern int64_t list_put(list_t *list, uint8_t *key, uint8_t* value, uint8_t* type);
extern int64_t list_delete(list_t *list, uint8_t *key);
extern db_entry_t *list_get_entry_by_idx(list_t *list, uint64_t idx);
extern db_entry_t *list_get_entry_by_key(list_t *list, uint8_t *key);
extern int64_t list_save(FILE *file, list_t *list);
extern void free_node(node_t *node);
extern void free_list(list_t *list);
extern void list_print(list_t *list);
