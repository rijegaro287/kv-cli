#include "kv_controller.h"

extern db_t *create_db(uint8_t *storage_type) {
  if (storage_type == NULL) {
    perror("Error: storage_type parameter is NULL");
    return NULL;
  }
  
  db_t *db = malloc(sizeof(db_t));
  if (db == NULL) {
    perror("Error: Failed to allocate memory for db_t");
    return NULL;
  }
  strcpy(db->storage_type, storage_type);

  if(strcmp(storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    db->storage = create_list();
    if (db->storage == NULL) {
      perror("Error: Failed to create list storage");
      free(db);
      return NULL;
    }
  }
  else if(strcmp(storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    free(db);
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    free(db);
    return NULL;
  }

  return db;
}

extern int64_t load_db(db_t *db, uint8_t *file_path) {
  FILE *db_file = fopen(file_path, "r");
  if (db_file == NULL) {
    perror("Error: Failed to read the database file.\n");
    return -1;
  }

  uint8_t line_buffer[LINE_BUFFER_SIZE];
  while (fgets(line_buffer, LINE_BUFFER_SIZE, db_file) != NULL) {
    db_entry_t *entry = parse_line(line_buffer);
    if (entry == NULL) {
      perror("Error: Failed to create entry object\n");
      return -1;
    }

    if (insert_entry(db, entry) < 0) {
      perror("Error: Failed to insert entry into storage\n");
      return -1;
    }
  }

  if (fclose(db_file) == EOF) {
    perror("Error: Failed to close the database file\n");
    return -1;
  }

  return 0;
}

extern int64_t insert_entry(db_t *db, db_entry_t *entry) {
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    if (list_insert((list_t*)db->storage, entry) < 0) {
      perror("Error: Failed to insert entry into list");
      return -1;
    }
    return 0;
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    return -1;
  }
  else {
    perror("Error: Invalid storage structure");
    return -1;
  }
}

extern int64_t put_entry(db_t *db, uint8_t *key, uint8_t *value, uint8_t *type) {
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    if (list_put(db->storage, key, value, type) < 0) {
      perror("Error: Failed to put entry into list");
      return -1;
    }
    return 0;
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    return -1;
  }
  else {
    perror("Error: Invalid storage structure");
    return -1;
  }
}

extern db_entry_t *get_by_idx(db_t *db, uint64_t idx) {
  db_entry_t *entry;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_by_idx((list_t*)db->storage, idx);
    if (entry == NULL) {
      perror("Error: Failed to insert entry into list");
      return NULL;
    }
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    return NULL;
  }
  return entry;
}

extern db_entry_t *get_by_key(db_t *db, uint8_t *key) {
  db_entry_t *entry;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_by_key((list_t*)db->storage, key);
    if (entry == NULL) {
      perror("Error: Failed to insert entry into list");
      return NULL;
    }
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
    return NULL;
  }
  else {
    perror("Error: Invalid storage structure");
    return NULL;
  }
  return entry;
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

extern void print_db(db_t *db) {
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    list_print((list_t*)db->storage);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    printf("Unimplemented: Hash Table");
  }
  else {
    perror("Error: Invalid storage structure");
  }
}
