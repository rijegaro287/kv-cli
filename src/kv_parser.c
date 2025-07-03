#include "kv_parser.h"

static int64_t set_integer_value(db_entry_t *dest, uint8_t *str_value, uint64_t type_size) {
  uint64_t value;
  if (str_to_int64(str_value, &value) < 0) {
    perror("Error: Failed to convert string to integer");
    return -1;
  }
  
  dest->value = malloc(type_size);
  if (dest->value == NULL) {
    perror("Error: Failed to allocate memory for value");
    return -1;
  }
  
  switch (type_size) {
  case sizeof(int8_t):
    *(int8_t*)dest->value = (int8_t)value;
    break;
  case sizeof(int16_t):
    *(int16_t*)dest->value = (int16_t)value;
    break;
  case sizeof(int32_t):
    *(int32_t*)dest->value = (int32_t)value;
    break;
  case sizeof(int64_t):
    *(int64_t*)dest->value = (int64_t)value;
    break;
  default:
    perror("Invalid type size for int value");
    free(dest->value);
    return -1;
  }

  return 0;
}

static int64_t set_float_value(db_entry_t *dest, uint8_t *str_value) {
  float value;
  if (str_to_float(str_value, &value) < 0) {
    perror("Error: Failed to convert string to float");
    return -1;
  }
  
  dest->value = malloc(sizeof(float));
  if (dest->value == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  *(float*)dest->value = value;
  
  return 0;
}

static int64_t set_double_value(db_entry_t *dest, uint8_t *str_value) {
  double value;
  if (str_to_double(str_value, &value) < 0) {
    perror("Error: Failed to convert string to double");
    return -1;
  }
  
  dest->value = malloc(sizeof(double));
  if (dest->value == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  *(double*)dest->value = value;
  
  return 0;
}

static int64_t set_bool_value(db_entry_t *dest, uint8_t *str_value) {
  bool value;
  if (str_to_bool(str_value, &value) < 0) {
    perror("Error: Failed to convert string to bool");
    return -1;
  }
  
  dest->value = malloc(sizeof(bool));
  if (dest->value == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  *(bool*)dest->value = value;
  
  return 0;
}

static int64_t set_string_value(db_entry_t *dest, uint8_t *str_value) {
  uint64_t str_len = strlen(str_value);
  dest->value = calloc(str_len + 1, sizeof(uint8_t));
  if (dest->value == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  strcpy(dest->value, str_value);
  
  return 0;
}

extern int64_t set_entry_value(db_entry_t *dest, uint8_t *str_value) {
  void *prev_value = NULL;
  if (dest->value != NULL) {
    prev_value = dest->value;
  }

  int64_t result = -1;
  switch (dest->type) {
  case INT8_TYPE:
    result = set_integer_value(dest, str_value, sizeof(int8_t));
    break;
  case INT16_TYPE:
    result = set_integer_value(dest, str_value, sizeof(int16_t));
    break;
  case INT32_TYPE:
    result = set_integer_value(dest, str_value, sizeof(int32_t));
    break;
  case INT64_TYPE:
    result = set_integer_value(dest, str_value, sizeof(int64_t));
    break;
  case FLOAT_TYPE:
    result = set_float_value(dest, str_value);
    break;
  case DOUBLE_TYPE:
    result = set_double_value(dest, str_value);
    break;
  case BOOL_TYPE:
    result = set_bool_value(dest, str_value);
    break;
  case STR_TYPE:
    result = set_string_value(dest, str_value);
    break;
  default:
    fprintf(stderr, "Error: data type %ld is not a valid datatype.\n", dest->type);
    return -1;
  }

  if (result < 0) {
    perror("Error: Failed to set entry value\n");
    return -1;
  }

  if (prev_value != NULL) {
    free(prev_value);
  }

  return result;
}

extern db_entry_t* parse_line(uint8_t *line) {
  if (strcmp(line, "\n") == 0 || line[0] == '#') return NULL;  
  uint8_t *type, *key, *value;
  if ((type = strtok(line, TYPE_DELIMETER)) == NULL ||
      (key = strtok(NULL, KEY_DELIMETER)) == NULL ||
      (value = strtok(NULL, VALUE_DELIMETER)) == NULL) {
    perror("Error: Failed to tokenize an entry\n");
    return NULL;
  }

  db_entry_t *entry = create_entry(key, value, type);
  if (entry == NULL) {
    perror("Error: Failed to create entry object\n");
  }

  return entry;
}

extern db_entry_t* create_entry(uint8_t *key, uint8_t *value, uint8_t *type) {
 db_entry_t *entry = malloc(sizeof(db_entry_t));
  if (entry == NULL) {
    perror("Error: failed to allocated memory for database entry\n");
    return NULL;
  }
  
  entry->type = map_data_type_str(type);
  strncpy(entry->key, key, CLI_CMD_BUFFER_SIZE);
  if (set_entry_value(entry, value) < 0) {
    fprintf(stderr, "Error: Failed to create entry with key \"%s\"", key);
    return NULL;
  }


  if (entry->type < 0 ||
      entry->key == NULL ||
      entry->value == NULL) {
    perror("Error: Failed to create entry object\n");
    free_entry(entry);
    return NULL;
  }

  return entry;
}

extern void free_entry(db_entry_t *entry) {
  if (entry == NULL) return;
  
  if (entry->value != NULL) {
    free(entry->value);
  }
  free(entry);
}

extern void print_entry(db_entry_t *entry) {
  switch (entry->type) {
  case INT8_TYPE:
    printf("type: %d, key: %s, value: %d\n", entry->type,
                                             entry->key,
                                             *(uint8_t*)entry->value);
    break;
  case INT16_TYPE:
    printf("type: %d, key: %s, value: %d\n", entry->type,
                                             entry->key,
                                             *(uint16_t*)entry->value);
    break;
  case INT32_TYPE:
    printf("type: %d, key: %s, value: %d\n", entry->type,
                                             entry->key,
                                             *(uint32_t*)entry->value);
    break;
  case INT64_TYPE:
    printf("type: %d, key: %s, value: %d\n", entry->type,
                                             entry->key,
                                             *(uint64_t*)entry->value);
    break;
  case BOOL_TYPE:
    printf("type: %d, key: %s, value: %d\n", entry->type,
                                             entry->key,
                                             *(bool*)entry->value);
    break;
  case FLOAT_TYPE:
    printf("type: %d, key: %s, value: %f\n", entry->type,
                                             entry->key,
                                             *(float*)entry->value);
    break;
  case DOUBLE_TYPE:
    printf("type: %d, key: %s, value: %f\n", entry->type,
                                             entry->key,
                                             *(double*)entry->value);
    break;
  case STR_TYPE:
    printf("type: %d, key: %s, value: %s\n", entry->type,
                                             entry->key, 
                                             (uint8_t*)entry->value);
    break;
  default:
    perror("Error: Invalid Data Type");
  }
}
