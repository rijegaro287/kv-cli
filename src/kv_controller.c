#include "kv_controller.h"

extern db_t *create_db(uint8_t *storage_type) {
  if (storage_type == NULL) {
    logger(3, "Error: storage_type parameter is NULL\n");
    return NULL;
  }
  
  db_t *db = malloc(sizeof(db_t));
  if (db == NULL) {
    logger(3, "Error: Failed to allocate memory for db_t\n");
    return NULL;
  }
  strncpy(db->storage_type, storage_type, SM_BUFFER_SIZE);

  if(strcmp(storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    db->storage = create_list();

  }
  else if(strcmp(storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    db->storage = create_hash_table(KV_STORAGE_HASH_SIZE);
  }
  else {
    db->storage = NULL;
  }

  if (db->storage == NULL) {
    logger(3, "Error: Failed to create storage structure\n");
    free_db(db);
    return NULL;
  }

  return db;
}

extern int64_t load_db(db_t *db, uint8_t *file_path) {
  if (db == NULL || file_path == NULL) {
    logger(3, "Error: NULL pointer passed to load_db\n");
    return -1;
  }
  
  FILE *db_file = fopen(file_path, "r");
  if (db_file == NULL) {
    logger(3, "Error: Failed to read the database file.\n");
    return -1;
  }

  uint8_t line_buffer[BG_BUFFER_SIZE];
  while (fgets(line_buffer, BG_BUFFER_SIZE, db_file) != NULL) {
    db_entry_t *entry = parse_line(line_buffer);
    if (entry == NULL) {
      logger(3, "Error: Failed to create entry object\n");
      return -1;
    }

    if (insert_entry(db, entry) < 0) {
      logger(3, "Error: Failed to insert entry into storage\n");
      return -1;
    }
  }

  if (fclose(db_file) == EOF) {
    logger(3, "Error: Failed to close the database file\n");
    return -1;
  }

  return 0;
}

extern int64_t save_db(db_t *db, uint8_t *file_path) {
  if (db == NULL || file_path == NULL) {
    logger(3, "Error: NULL pointer passed to save_db\n");
    return -1;
  }
  
  uint8_t tmp_path[BG_BUFFER_SIZE];
  snprintf(tmp_path, BG_BUFFER_SIZE, "%s.tmp", file_path);
  tmp_path[BG_BUFFER_SIZE - 1] = '\0';
  
  FILE *new_file = fopen(tmp_path, "w");
  if (new_file == NULL) {
    logger(3, "Error: Failed to create temporary database file.\n");
    fclose(new_file);
    return -1;
  }

  int64_t result;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    result = list_save(new_file, (list_t*)db->storage);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    result = hash_save(new_file, (hash_table_t*)db->storage);
  }
  else {
    logger(3, "Error: Invalid storage structure\n");
    result = -1;
  }

  fclose(new_file);
  
  if (result < 0) {
    logger(3, "Error: Failed to save database to a file");
  }
  else {
    remove(file_path);
    rename(tmp_path, file_path);
  }
  
  return result;
}

extern int64_t insert_entry(db_t *db, db_entry_t *entry) {
  if (db == NULL || entry == NULL) {
    logger(3, "Error: NULL pointer passed to insert_entry\n");
    return -1;
  }
  
  int64_t result;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    result = list_insert((list_t*)db->storage, entry);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    result = hash_insert((hash_table_t*)db->storage, entry);
  }
  else {
    logger(3, "Error: Invalid storage structure\n");
    result = -1;
  }

  if (result < 0) {
    logger(3, "Error: Failed to insert entry to storage\n");
  }

  return result;
}

extern int64_t put_entry(db_t *db, uint8_t *key, uint8_t *value, uint8_t *type) {
  if (db == NULL || key == NULL || value == NULL || type == NULL) {
    logger(3, "Error: NULL pointer passed to put_entry\n");
    return -1;
  }
  
  uint64_t result;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    result = list_put((list_t*)db->storage, key, value, type);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    result = hash_put((hash_table_t*)db->storage, key, value, type);
  }
  else {
    logger(3, "Error: Invalid storage structure\n");
    result = -1;
  }

  if (result < 0) {
    logger(3, "Error: Failed to put entry into storage\n");
  }

  return result;
}

extern int64_t delete_entry(db_t *db, uint8_t *key) {
  if (db == NULL || key == NULL) {
    logger(3, "Error: NULL pointer passed to delete_entry\n");
    return -1;
  }
  
  uint64_t result;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    result = list_delete((list_t*)db->storage, key);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    result = hash_delete((hash_table_t*)db->storage, key);
  }
  else {
    logger(3, "Error: Invalid storage structure\n");
    result = -1;
  }

  if (result < 0) {
    logger(3, "Error: Failed to delete an entry from storage\n");
  }

  return result;
}

extern db_entry_t* get_entry(db_t *db, uint8_t *key) {
  if (db == NULL || key == NULL) {
    logger(3, "Error: NULL pointer passed to get_entry\n");
    return NULL;
  }
  
  db_entry_t *entry;
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    entry = list_get_entry_by_key((list_t*)db->storage, key);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    entry = hash_get_entry((hash_table_t*)db->storage, key);
  }
  else {
    logger(3, "Error: Invalid storage structure\n");
    entry = NULL;
  }

  if (entry == NULL) {
    logger(3, "Error: Failed to get entry from storage\n");
  }

  return entry;
}

extern void free_db(db_t *db) {
  if (db == NULL) return;

  if (db->storage == NULL) {
    free(db);
    return;
  };

  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    free_list((list_t*)db->storage);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    free_hash_table((hash_table_t*)db->storage);
  }
  else {
    free(db->storage);
  }

  free(db);
}

extern void print_db(db_t *db) {
  if (db == NULL) {
    logger(3, "Error: NULL pointer passed to print_db\n");
    return;
  }
  
  if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_LIST) == 0) {
    list_print((list_t*)db->storage);
  }
  else if (strcmp(db->storage_type, KV_STORAGE_STRUCTURE_HASH) == 0) {
    hash_print((hash_table_t*)db->storage);
  }
  else {
    logger(3, "Error: Invalid storage structure\n");
  }
}
