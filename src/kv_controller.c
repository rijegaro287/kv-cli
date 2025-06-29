#include "kv_controller.h"

extern db_entry_t *get_by_idx(db_t *db_ptr, uint64_t idx) {
  db_entry_t *entry;
  if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_by_idx((list_t*)db_ptr->storage, idx);
    if (entry == NULL) {
      perror("Error: Failed to insert entry into list");
      return NULL;
    }
  }
  else if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    return NULL;
  }
  return entry;
}

extern db_entry_t *get_by_key(db_t *db_ptr, uint8_t *key) {
  db_entry_t *entry;
  if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_by_key((list_t*)db_ptr->storage, key);
    if (entry == NULL) {
      perror("Error: Failed to insert entry into list");
      return NULL;
    }
  }
  else if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_HASH) == 0) {
    // db_ptr = malloc(sizeof hash_t)
    printf("Unimplemented: Hash Table");
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    return NULL;
  }
  return entry;
}

extern int64_t insert_entry(db_t *db_ptr, db_entry_t *entry_ptr) {
  if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_LIST) == 0) {
    if (list_insert((list_t*)db_ptr->storage, entry_ptr) < 0) {
      perror("Error: Failed to insert entry into list");
      return -1;
    }
    return 0;
  }
  else if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_HASH) == 0) {
    // db_ptr = malloc(sizeof hash_t)
    printf("Unimplemented: Hash Table");
    return -1;
  }
  else {
    perror("Error: Invalid storage structure");
    return -1;
  }
}

extern int64_t load_db(db_t *db_ptr, uint8_t *file_path, uint8_t *storage_structure) {
  FILE *db_file_ptr = fopen(file_path, "r+");
  if (db_file_ptr == NULL) {
    perror("Error: Failed to read the database file.\n");
    return -1;
  }

  uint8_t line_buffer[LINE_BUFFER_SIZE];
  while (fgets(line_buffer, LINE_BUFFER_SIZE, db_file_ptr) != NULL) {
    if (strcmp(line_buffer, "\n") == 0 || line_buffer[0] == '#') continue;  
    uint8_t *type, *key, *value;
    if ((type = strtok(line_buffer, TYPE_DELIMETER)) == NULL ||
        (key = strtok(NULL, KEY_DELIMETER)) == NULL ||
        (value = strtok(NULL, VALUE_DELIMETER)) == NULL) {
      perror("Error: Failed to tokenize an entry");
      continue;
    }
    
    db_entry_t *entry = create_db_entry(type, key, value);
    if (entry->type < 0 || entry->key == NULL || entry->value_ptr == NULL) {
      perror("Error: Failed to create entry object");
      continue; 
    }

    if(insert_entry(db_ptr, entry) < 0) {
      perror("Error: Failed to insert entry into storage");
    }
  }

  if (fclose(db_file_ptr) == EOF) {
    perror("Error: Failed to close the database file\n");
    return -1;
  }

  return 0;
}

extern db_t *create_db(uint8_t *storage_structure) {
  db_t *db_ptr = malloc(sizeof(db_t));
  db_ptr->id = 0;
  if(strcmp(storage_structure, KV_STORAGE_STRUCTURE_LIST) == 0) {
    db_ptr->storage = malloc(sizeof(list_t));
  }
  else if(strcmp(storage_structure, KV_STORAGE_STRUCTURE_HASH) == 0) {
    // db_ptr = malloc(sizeof hash_t)
    printf("Unimplemented: Hash Table");
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    return NULL;
  }

  if (db_ptr == NULL) {
    perror("Error: Failed to allocated storage memory");
    return NULL;
  }

  return db_ptr;
}

extern void print_db(db_t *db_ptr) {
  if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_LIST) == 0) {
    list_print((list_t*)db_ptr->storage);
  }
  else if (strcmp(storage_structure, KV_STORAGE_STRUCTURE_HASH) == 0) {
    // db_ptr = malloc(sizeof hash_t)
    printf("Unimplemented: Hash Table");
  }
  else {
    perror("Error: Invalid storage structure");
  }
}
