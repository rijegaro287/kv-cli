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
  if (list == NULL || entry == NULL) {
    logger(3, "Error: NULL pointer passed to list_insert\n");
    return -1;
  }
  
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

extern int64_t list_put(list_t* list, uint8_t* key, uint8_t* value, uint8_t* type) {
  if (list == NULL || key == NULL || value == NULL || type == NULL) {
    logger(3, "Error: NULL pointer passed to list_put\n");
    return -1;
  }
  
  db_entry_t *entry = list_get_entry_by_key(list, key);
  if (entry != NULL) {
    if (update_entry(entry, key, value, type)) {
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
  if (list == NULL || key == NULL) {
    logger(3, "Error: NULL pointer passed to list_delete\n");
    return -1;
  }
  
  node_t* previous_node = NULL;
  node_t* current_node = list->head;
  while (current_node != NULL) {
    if (strcmp(current_node->entry->key, key) == 0) {
      if (current_node == list->head) {
        list->head = current_node->next;
      }
      else {
        previous_node->next = current_node->next;
      }
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
  if (list == NULL) {
    logger(3, "Error: NULL pointer passed to list_get_entry_by_idx\n");
    return NULL;
  }
  
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
  if (list == NULL || key == NULL) {
    logger(3, "Error: NULL pointer passed to list_get_entry_by_key\n");
    return NULL;
  }
  
  node_t* current_node = list->head;
  while (current_node != NULL) {
    if (strcmp(current_node->entry->key, key) == 0) {
      return current_node->entry;
    }
    current_node = current_node->next;
  }
  return NULL;
}

extern int64_t list_save(FILE *file, list_t *list) {
  if (file == NULL || list == NULL) {
    logger(3, "Error: NULL pointer passed to list_save\n");
    return -1;
  }
  
  node_t *current_node = list->head;
  while (current_node != NULL) {
    uint8_t entry_str[BG_BUFFER_SIZE];
    parse_entry(current_node->entry, entry_str, BG_BUFFER_SIZE);
    if (strlen(entry_str) == 0) {
      logger(3, "Error: Failed to parse entry\n");
      return -1;
    }

    if (fputs(entry_str, file) == EOF) {
      logger(3, "Error: Failed to write entry to file\n");
      return -1;
    }

    current_node = current_node->next;
  }
  return 0;
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
  if (list == NULL) {
    logger(3, "Error: NULL pointer passed to list_print\n");
    return;
  }
  
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
