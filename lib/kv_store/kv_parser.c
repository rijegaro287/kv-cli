#include "kv_parser.h"

static int64_t set_integer_value(db_entry_t *dest, uint8_t *str_value, uint64_t type_size) {
  if (dest == NULL || str_value == NULL) {
    logger(3, "Error: NULL pointer passed to set_integer_value\n");
    return -1;
  }
  
  uint64_t value;
  if (str_to_int64(str_value, &value) < 0) {
    logger(3, "Error: Failed to convert string to integer\n");
    return -1;
  }
  
  dest->value = malloc(type_size);
  if (dest->value == NULL) {
    logger(3, "Error: Failed to allocate memory for value\n");
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
    logger(3, "Invalid type size for int value\n");
    free(dest->value);
    return -1;
  }

  return 0;
}

static int64_t set_float_value(db_entry_t *dest, uint8_t *str_value) {
  if (dest == NULL || str_value == NULL) {
    logger(3, "Error: NULL pointer passed to set_float_value\n");
    return -1;
  }
  
  float value;
  if (str_to_float(str_value, &value) < 0) {
    logger(3, "Error: Failed to convert string to float\n");
    return -1;
  }
  
  dest->value = malloc(sizeof(float));
  if (dest->value == NULL) {
    logger(3, "Error: Failed to allocate memory\n");
    return -1;
  }
  
  *(float*)dest->value = value;
  
  return 0;
}

static int64_t set_double_value(db_entry_t *dest, uint8_t *str_value) {
  if (dest == NULL || str_value == NULL) {
    logger(3, "Error: NULL pointer passed to set_double_value\n");
    return -1;
  }
  
  double value;
  if (str_to_double(str_value, &value) < 0) {
    logger(3, "Error: Failed to convert string to double\n");
    return -1;
  }
  
  dest->value = malloc(sizeof(double));
  if (dest->value == NULL) {
    logger(3, "Error: Failed to allocate memory\n");
    return -1;
  }
  
  *(double*)dest->value = value;
  
  return 0;
}

static int64_t set_bool_value(db_entry_t *dest, uint8_t *str_value) {
  if (dest == NULL || str_value == NULL) {
    logger(3, "Error: NULL pointer passed to set_bool_value\n");
    return -1;
  }
  
  bool value;
  if (str_to_bool(str_value, &value) < 0) {
    logger(3, "Error: Failed to convert string to bool\n");
    return -1;
  }
  
  dest->value = malloc(sizeof(bool));
  if (dest->value == NULL) {
    logger(3, "Error: Failed to allocate memory\n");
    return -1;
  }
  
  *(bool*)dest->value = value;
  
  return 0;
}

static int64_t set_string_value(db_entry_t *dest, uint8_t *str_value) {
  if (dest == NULL || str_value == NULL) {
    logger(3, "Error: NULL pointer passed to set_string_value\n");
    return -1;
  }
  
  uint64_t str_len = strlen(str_value);
  dest->value = calloc(str_len + 1, sizeof(uint8_t));
  if (dest->value == NULL) {
    logger(3, "Error: Failed to allocate memory\n");
    return -1;
  }
  
  strcpy(dest->value, str_value);
  
  return 0;
}

extern int64_t set_entry_value(db_entry_t *dest, uint8_t *str_value) {
  if (dest == NULL || str_value == NULL) {
    logger(3, "Error: NULL pointer passed to set_entry_value\n");
    return -1;
  }
  
  if (dest->value != NULL) {
    free(dest->value);
    dest->value = NULL;
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
    logger(3, "Error: data type %ld is not a valid datatype.\n", dest->type);
    return -1;
  }

  if (result < 0) {
    logger(3, "Error: Failed to set entry value\n");
    return -1;
  }

  return result;
}

extern db_entry_t* create_entry(uint8_t *key, uint8_t *value, uint8_t *type) {
  if (key == NULL || value == NULL || type == NULL) {
    logger(3, "Error: NULL pointer passed to create_entry\n");
    return NULL;
  }
  
 db_entry_t *entry = malloc(sizeof(db_entry_t));
  if (entry == NULL) {
    logger(3, "Error: failed to allocated memory for database entry\n");
    return NULL;
  }
  
  entry->type = map_datatype_from_str(type);
  entry->value = NULL;
  strncpy(entry->key, key, SM_BUFFER_SIZE);
  if (set_entry_value(entry, value) < 0) {
    logger(3, "Error: Failed to create entry with key \"%s\"\n", key);
    return NULL;
  }

  if (entry->type < 0 ||
      entry->key == NULL ||
      entry->value == NULL) {
    logger(3, "Error: Failed to create entry object\n");
    free_entry(entry);
    return NULL;
  }

  return entry;
}

extern db_entry_t* parse_line(uint8_t *line) {
  if (line == NULL) {
    logger(3, "Error: NULL pointer passed to parse_line\n");
    return NULL;
  }
  
  if (strcmp(line, "\n") == 0 || line[0] == '#') return NULL;  
  uint8_t *type, *key, *value;
  if ((type = strtok(line, TYPE_DELIMETER)) == NULL ||
      (key = strtok(NULL, KEY_DELIMETER)) == NULL ||
      (value = strtok(NULL, VALUE_DELIMETER)) == NULL) {
    logger(3, "Error: Failed to tokenize an entry\n");
    return NULL;
  }

  db_entry_t *entry = create_entry(key, value, type);
  if (entry == NULL) {
    logger(3, "Error: Failed to create entry object\n");
  }

  return entry;
}

extern int64_t update_entry(db_entry_t *entry, uint8_t* key, uint8_t* value, uint8_t* type) {
  if (entry == NULL || key == NULL || value == NULL || type == NULL) {
    logger(3, "Error: NULL pointer passed to update_entry\n");
    return -1;
  }
  
  if (strlen(type) > 0) { 
    entry->type = map_datatype_from_str(type);
  }

  if (set_entry_value(entry, value) < 0) {
    logger(4, "Error: Failed to update entry with key \"%s\"\n", key);
    return -1;
  }

  return 0;
}

extern void parse_entry(db_entry_t *entry, uint8_t *dest, uint64_t max_len) {
  if (entry == NULL || dest == NULL) {
    logger(3, "Error: NULL pointer passed to parse_entry\n");
    return;
  }
  
  uint8_t type[SM_BUFFER_SIZE];
  uint8_t key[SM_BUFFER_SIZE];
  uint8_t value[SM_BUFFER_SIZE];

  map_datatype_to_str(entry->type, type, SM_BUFFER_SIZE);
  strncpy(key, entry->key, SM_BUFFER_SIZE);
  map_value_to_str(entry->type, entry->value, value, SM_BUFFER_SIZE);

  snprintf(dest, max_len, "%s%s%s%s%s%s", type,
                                          KV_PARSER_TYPE_DELIMITER,
                                          key,
                                          KV_PARSER_KEY_DELIMITER,
                                          value,
                                          KV_PARSER_VALUE_DELIMITER);
}

extern void free_entry(db_entry_t *entry) {
  if (entry == NULL) return;
  
  if (entry->value != NULL) {
    free(entry->value);
  }
  free(entry);
}

extern void print_entry(db_entry_t *entry) {
  if (entry == NULL) {
    logger(3, "Error: NULL pointer passed to print_entry\n");
    return;
  }
  
  uint8_t type[SM_BUFFER_SIZE];
  map_datatype_to_str(entry->type, type, SM_BUFFER_SIZE);

  logger(4, "%s\t%s\t", entry->key, type);

  switch (entry->type) {
  case INT8_TYPE:
    logger(4, "%" PRId8 "\n", *(int8_t*)entry->value);
    break;
  case INT16_TYPE:
    logger(4, "%" PRId16 "\n", *(int16_t*)entry->value);
    break;
  case INT32_TYPE:
    logger(4, "%" PRId32 "\n", *(int32_t*)entry->value);
    break;
  case INT64_TYPE:
    logger(4, "%" PRId64 "\n", *(int64_t*)entry->value);
    break;
  case BOOL_TYPE:
    logger(4, "%s\n", *(bool*)entry->value ? "true" : "false");
    break;
  case FLOAT_TYPE:
    logger(4, "%.7f\n", *(float*)entry->value);
    break;
  case DOUBLE_TYPE:
    logger(4, "%.15lf\n", *(double*)entry->value);
    break;
  case STR_TYPE:
    logger(4, "%s\n", (uint8_t*)entry->value);
    break;
  default:
    logger(3, "\nError: Invalid Data Type\n");
  }
}
