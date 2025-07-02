#include "kv_controller.h"

extern db_entry_t *get_by_idx(db_t *db_ptr, uint64_t idx) {
  db_entry_t *entry;
  if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_by_idx((list_t*)db_ptr->storage, idx);
    if (entry == NULL) {
      perror("Error: Failed to insert entry into list");
      return NULL;
    }
  }
  else if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
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
  if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_by_key((list_t*)db_ptr->storage, key);
    if (entry == NULL) {
      perror("Error: Failed to insert entry into list");
      return NULL;
    }
  }
  else if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
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
  if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    if (list_insert((list_t*)db_ptr->storage, entry_ptr) < 0) {
      perror("Error: Failed to insert entry into list");
      return -1;
    }
    return 0;
  }
  else if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    return -1;
  }
  else {
    perror("Error: Invalid storage structure");
    return -1;
  }
}

extern db_t *create_db(uint8_t *storage_type) {
  if (storage_type == NULL) {
    perror("Error: storage_type parameter is NULL");
    return NULL;
  }
  
  db_t *db_ptr = malloc(sizeof(db_t));
  if (db_ptr == NULL) {
    perror("Error: Failed to allocate memory for db_t");
    return NULL;
  }
  strcpy(db_ptr->storage_type, storage_type);

  if(strcmp(storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    db_ptr->storage = create_list();
    if (db_ptr->storage == NULL) {
      perror("Error: Failed to create list storage");
      free(db_ptr);
      return NULL;
    }
  }
  else if(strcmp(storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    free(db_ptr);
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    free(db_ptr);
    return NULL;
  }

  return db_ptr;
}

extern int64_t load_db(db_t *db_ptr, uint8_t *file_path) {
  FILE *db_file_ptr = fopen(file_path, "r");
  if (db_file_ptr == NULL) {
    perror("Error: Failed to read the database file.\n");
    return -1;
  }

  uint8_t line_buffer[LINE_BUFFER_SIZE];
  while (fgets(line_buffer, LINE_BUFFER_SIZE, db_file_ptr) != NULL) {
    db_entry_t *entry = parse_line(line_buffer);
    if (entry == NULL) {
      perror("Error: Failed to create entry object\n");
      return -1;
    }

    if (insert_entry(db_ptr, entry) < 0) {
      perror("Error: Failed to insert entry into storage\n");
      return -1;
    }
  }

  if (fclose(db_file_ptr) == EOF) {
    perror("Error: Failed to close the database file\n");
    return -1;
  }

  return 0;
}

extern void free_db(db_t *db) {
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    if (db->storage != NULL) {
      free_list(db->storage);
    }
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Uninplemented: free hash storage");
  }
  else {
    free(db->storage);
  }
  free(db);
}

extern void print_db(db_t *db_ptr) {
  if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    list_print((list_t*)db_ptr->storage);
  }
  else if (strcmp(db_ptr->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
  }
  else {
    perror("Error: Invalid storage structure");
  }
}
