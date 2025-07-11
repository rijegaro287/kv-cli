#include "hash_table.h"

static int64_t calculate_hash_code(uint8_t *key, uint64_t size) {
  if (key == NULL) {
    logger(3, "Error: key parameter is NULL\n");
    return -1;
  }

  int64_t hash_code = 0;
  
  uint8_t idx = 0;
  uint8_t character;
  while((character = key[idx++]) != '\0') {
    hash_code += character;
  }
  hash_code %= size;

  return hash_code;
}

extern hash_table_t* create_hash_table(uint64_t len) {
  hash_table_t *hash = malloc(sizeof(hash_table_t));
  if (hash == NULL) {
    logger(3, "Failed to allocate memory for hash table.");
    return NULL;
  }
  
  list_t **content = malloc(sizeof(list_t*)*len);
  if (content == NULL) {
    logger(3, "Failed to allocate memory for hash table contents.");
    free_hash_table(hash);
    return NULL;
  }
  
  hash->content = content;
  hash->size = len;

  for (uint64_t idx = 0; idx < hash->size; idx++) {
    hash->content[idx] = create_list();
    if (hash->content[idx] == NULL) {
      logger(3, "Failed to create list for index %d\n", idx);
      free_hash_table(hash);
      return NULL;
    }
  }
  
  return hash;
}

extern int64_t hash_insert(hash_table_t *hash, db_entry_t *entry) {
  if (hash == NULL || entry == NULL) {
    logger(3, "Error: NULL pointer passed to hash_insert\n");
    return -1;
  }
  
  int64_t hash_code = calculate_hash_code(entry->key, hash->size);
  if (hash_code < 0) {
    return -1;
  }
  list_t *list = hash->content[hash_code];
  if (list == NULL) {
    return -1;
  }
  return list_insert(list, entry);
}

extern int64_t hash_put(hash_table_t *hash, uint8_t *key, uint8_t* value, uint8_t* type) {
  if (hash == NULL || key == NULL || value == NULL || type == NULL) {
    logger(3, "Error: NULL pointer passed to hash_put\n");
    return -1;
  }
  
  int64_t hash_code = calculate_hash_code(key, hash->size);
  if (hash_code < 0) {
    return -1;
  }
  list_t *list = hash->content[hash_code];
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
    
    if (hash_insert(hash, entry) < 0) {      
      logger(3, "Error: Failed to insert entry into list.\n");
      return -1;
    }
  }
  return 0;
}

extern int64_t hash_delete(hash_table_t *hash, uint8_t *key) {
  if (hash == NULL || key == NULL) {
    logger(3, "Error: NULL pointer passed to hash_delete\n");
    return -1;
  }
  
  int64_t hash_code = calculate_hash_code(key, hash->size);
  if (hash_code < 0) {
    return -1;
  }
  list_t *list = hash->content[hash_code];
  return list_delete(list, key);
}

extern db_entry_t *hash_get_entry(hash_table_t *hash, uint8_t *key) {
  if (hash == NULL || key == NULL) {
    logger(3, "Error: NULL pointer passed to hash_get_entry\n");
    return NULL;
  }
  
  int64_t hash_code = calculate_hash_code(key, hash->size);
  if (hash_code < 0) {
    return NULL;
  }
  list_t *list = hash->content[hash_code];
  db_entry_t *entry = list_get_entry_by_key(list, key);
  return entry;
}

extern int64_t hash_save(FILE *file, hash_table_t *hash) {
  if (file == NULL || hash == NULL) {
    logger(3, "Error: NULL pointer passed to hash_save\n");
    return -1;
  }
  
  for (uint64_t idx = 0; idx < hash->size; idx++) {
    list_t *list = hash->content[idx];
    if (list_save(file, list) < 0) {
      logger(3, "Error: Failed to save hash table entry\n");
      return -1;
    }
  }
  return 0;
}

extern void free_hash_table(hash_table_t *hash) { 
  if (hash == NULL) return;

  for (uint64_t idx = 0; idx < hash->size; idx++) {
    list_t *list = hash->content[idx];
    if (list == NULL) continue;
    free_list(list);
  }

  free(hash->content);
  free(hash);
}

extern void hash_print(hash_table_t *hash) {
  if (hash == NULL) {
    logger(3, "Error: NULL pointer passed to hash_print\n");
    return;
  }
  
  for (uint64_t idx = 0; idx < hash->size; idx++) {
    list_t *list = hash->content[idx];
    if (list->size > 0) {
      list_print(list);
    }
  }
}
