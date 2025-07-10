#include <stdio.h>

#include "linked_list.h"

extern list_t* create_list() {
  list_t* new_list = malloc(sizeof(list_t));
  if (new_list == NULL) {
    logger(3, "Error: Failed to allocated memory for a linked list.\n");
    return NULL;
  }
  new_list->size = 0;
  new_list->head = NULL;
  return new_list;
}

extern int64_t list_insert(list_t* list, db_entry_t *entry) {
  node_t* new_node = malloc(sizeof(node_t));
  if (new_node == NULL) {
    logger(3, "Error: Failed to allocated memory for a node.\n");
    return -1;
  }

  new_node->entry = entry;
  new_node->next = NULL;

  if(list->head == NULL) {
    list->head = new_node;
  }
  else {
    node_t* current_node = list->head;
    while(current_node->next != NULL) {
      if (strcmp(current_node->entry->key, entry->key) == 0) {
        logger(4, "Error: Entry with key \"%s\" already exists\n", entry->key);
        free(new_node);
        return -1;
      }
      current_node = current_node->next;
    }

    if (strcmp(current_node->entry->key, entry->key) == 0) {
      logger(4, "Error: Entry with key \"%s\" already exists\n", entry->key);
      free(new_node);
      return -1;
    }
    
    current_node->next = new_node;
  }

  list->size++;

  return 0;
}

extern db_entry_t* list_find(list_t *list, uint8_t *key) {
  node_t* current_node = list->head;
  while (current_node != NULL) {
    if (strcmp(current_node->entry->key, key) == 0) {
      return current_node->entry;
    }
    current_node = current_node->next;
  }
  return NULL;
}

extern int64_t list_update(db_entry_t *entry, uint8_t* key, uint8_t* value, uint8_t* type) {
  if (strlen(type) > 0) { 
    entry->type = map_data_type_str(type);
  }

  if (set_entry_value(entry, value) < 0) {
    logger(4, "Error: Failed to update entry with key \"%s\"\n", key);
    return -1;
  }

  return 0;
}

extern int64_t list_put(list_t* list, uint8_t* key, uint8_t* value, uint8_t* type) {
  db_entry_t *entry = list_find(list, key);
  if (entry != NULL) {
    if (list_update(entry, key, value, type)) {
      logger(3, "Error: Failed to update an entry\n");
      return -1;
    }
  }
  else {
    entry = create_entry(key, value, type);
    if (entry == NULL) {
      logger(3, "Error: Failed to create entry.\n");
      return -1;
    }
    
    if (list_insert(list, entry) < 0) {      
      logger(3, "Error: Failed to insert entry into list.\n");
      return -1;
    }
  }
  return 0;
}

extern int64_t list_delete(list_t* list, uint8_t *key) {
  node_t* previous_node = NULL;
  node_t* current_node = list->head;
  while (current_node != NULL) {
    if (strcmp(current_node->entry->key, key) == 0) {
      previous_node->next = current_node->next;
      free_node(current_node);
      list->size--;
      return 0;
    }
    previous_node = current_node;
    current_node = current_node->next;
  }
  return -1;
}

extern db_entry_t *list_get_entry_by_idx(list_t* list, uint64_t idx) {
  if (idx >= list->size) {
    logger(3, "Index %d out of range for list\n", idx);
    return NULL;
  }
  uint64_t current_idx = 0;
  node_t* current_node = list->head;
  while (current_node != NULL) {
    if (current_idx == idx) {
      return current_node->entry;
    }
    current_idx++;
    current_node = current_node->next;
  }
  return NULL;
}

extern db_entry_t *list_get_entry_by_key(list_t* list, uint8_t *key) {
  node_t* current_node = list->head;
  while (current_node != NULL) {
    if (strcmp(current_node->entry->key, key) == 0) {
      return current_node->entry;
    }
    current_node = current_node->next;
  }
  return NULL;
}

extern void free_node(node_t *node) {
  if (node == NULL) return;
  free_entry(node->entry);
  free(node);
}

extern void free_list(list_t *list) {
  if (list == NULL) return;

  if (list->head == NULL) {
    free(list);
    return;
  }

  node_t *current_node = list->head;
  node_t *next_node;
  while (current_node != NULL) {
    next_node = current_node->next;
    free_node(current_node);
    current_node = next_node;
  }

  free(list);
}

extern void list_print(list_t *list) {
  if (list->size == 0) {
    logger(3, "Linked list is empty\n");
    return;
  }

  for (uint64_t i = 0; i < list->size; i++) {
    db_entry_t *entry = list_get_entry_by_idx(list, i);
    if(entry == NULL) {
      logger(3, "Error: Entry not found\n");
      return;
    }
    print_entry(entry);
  }
}
