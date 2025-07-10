#include "hash_table.h"

static uint64_t calculate_hash_code(uint8_t *key, uint64_t size) {
  uint64_t hash_code = 0;
  
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
    printf("Failed to allocate memory for hash table.");
  }
  
  list_t **content = malloc(sizeof(list_t*)*len);
  if (content == NULL) {
    printf("Failed to allocate memory for hash table contents.");
    free_hash_table(hash);
    return NULL;
  }

  for (uint64_t idx = 0; idx < len; idx++) {
    if ((content[idx] = create_list()) == NULL) {
      printf("Failed to create list for index %d\n", idx);
      free_hash_table(hash);
      return NULL;
    }
  }

  hash->content = content;
  hash->size = len;
  
  return hash;
}

extern int64_t hash_insert(hash_table_t *hash, db_entry_t *entry) {
  uint64_t hash_code = calculate_hash_code(entry->key, hash->size);
  list_t *list = hash->content[hash_code];
  list_insert(list, entry);
}

extern int64_t hash_put(hash_table_t *hash, uint8_t *key, uint8_t* value, uint8_t* type) {
  uint64_t hash_code = calculate_hash_code(key, hash->size);
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
  uint64_t hash_code = calculate_hash_code(key, hash->size);
  list_t *list = hash->content[hash_code];
  return list_delete(list, key);
}

extern db_entry_t *hash_get_entry(hash_table_t *hash, uint8_t *key) {
  uint64_t hash_code = calculate_hash_code(key, hash->size);
  list_t *list = hash->content[hash_code];
  db_entry_t *entry = list_get_entry_by_key(list, key);
  return entry;
}

extern int64_t hash_save(FILE *file, hash_table_t *hash) {
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
  for (uint64_t idx = 0; idx < hash->size; idx++) {
    list_t *list = hash->content[idx];
    if (list->size > 0) {
      list_print(list);
    }
  }
}