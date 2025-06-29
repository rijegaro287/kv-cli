#include "kv_parser.h"

static int64_t set_integer_value(db_entry_t *dest, uint8_t *str_value, uint64_t type_size) {
  uint64_t value_ptr;
  if (str_to_int64(str_value, &value_ptr) < 0) {
    perror("Error: Failed to convert string to integer");
    return -1;
  }
  
  dest->value_ptr = malloc(type_size);
  if (dest->value_ptr == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  switch (type_size) {
  case sizeof(int8_t):
    *(int8_t*)dest->value_ptr = (int8_t)value_ptr;
    break;
  case sizeof(int16_t):
    *(int16_t*)dest->value_ptr = (int16_t)value_ptr;
    break;
  case sizeof(int32_t):
    *(int32_t*)dest->value_ptr = (int32_t)value_ptr;
    break;
  case sizeof(int64_t):
    *(int64_t*)dest->value_ptr = (int64_t)value_ptr;
    break;
  }

  return 0;
}

static int64_t set_float_value(db_entry_t *dest, uint8_t *str_value) {
  float value_ptr;
  if (str_to_float(str_value, &value_ptr) < 0) {
    perror("Error: Failed to convert string to float");
    return -1;
  }
  
  dest->value_ptr = malloc(sizeof(float));
  if (dest->value_ptr == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  *(float*)dest->value_ptr = value_ptr;
  
  return 0;
}

static int64_t set_double_value(db_entry_t *dest, uint8_t *str_value) {
  double value_ptr;
  if (str_to_double(str_value, &value_ptr) < 0) {
    perror("Error: Failed to convert string to double");
    return -1;
  }
  
  dest->value_ptr = malloc(sizeof(double));
  if (dest->value_ptr == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  *(double*)dest->value_ptr = value_ptr;
  
  return 0;
}

static int64_t set_bool_value(db_entry_t *dest, uint8_t *str_value) {
  bool value_ptr;
  if (str_to_bool(str_value, &value_ptr) < 0) {
    perror("Error: Failed to convert string to bool");
    return -1;
  }
  
  dest->value_ptr = malloc(sizeof(bool));
  if (dest->value_ptr == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  *(bool*)dest->value_ptr = value_ptr;
  
  return 0;
}

static int64_t set_string_value(db_entry_t *dest, uint8_t *str_value) {
  uint64_t str_len = strlen(str_value);
  dest->value_ptr = calloc(str_len + 1, sizeof(uint8_t));
  if (dest->value_ptr == NULL) {
    perror("Error: Failed to allocate memory");
    return -1;
  }
  
  strcpy(dest->value_ptr, str_value);
  
  return 0;
}

static int64_t set_entry_value(db_entry_t *dest, uint8_t *str_value) {
  switch (dest->type) {
  case INT8_TYPE:
    return set_integer_value(dest, str_value, sizeof(int8_t));
  case INT16_TYPE:
    return set_integer_value(dest, str_value, sizeof(int16_t));
  case INT32_TYPE:
    return set_integer_value(dest, str_value, sizeof(int32_t));
  case INT64_TYPE:
    return set_integer_value(dest, str_value, sizeof(int64_t));
  case FLOAT_TYPE:
    return set_float_value(dest, str_value);
  case DOUBLE_TYPE:
    return set_double_value(dest, str_value);
  case BOOL_TYPE:
    return set_bool_value(dest, str_value);
  case STR_TYPE:
    return set_string_value(dest, str_value);
  default:
    fprintf(stderr, "Error: data type %ld is not a valid datatype.\n", dest->type);
    return -1;
  }
}

extern db_entry_t* create_db_entry(uint8_t *type, uint8_t *key, uint8_t *value) {
  db_entry_t *entry_ptr = malloc(sizeof(db_entry_t));
  entry_ptr->type = map_data_type_str(type);
  entry_ptr->key = key;
  entry_ptr->value_ptr = NULL;
  set_entry_value(entry_ptr, value);
  return entry_ptr;
}

extern void print_entry(db_entry_t *entry) {
  uint64_t *value_ptr = entry->value_ptr;
  switch (entry->type) {
  case INT8_TYPE:
  case INT16_TYPE:
  case INT32_TYPE:
  case INT64_TYPE:
  case BOOL_TYPE:
    printf("type: %d\nkey: %s\nvalue: %d\n", entry->type, entry->key, *value_ptr);
    break;
  case FLOAT_TYPE:
  case DOUBLE_TYPE:
    printf("type: %d\nkey: %s\nvalue: %f\n", entry->type, entry->key, *value_ptr);
    break;
  case STR_TYPE:
    printf("type: %d\nkey: %s\nvalue: %s\n", entry->type, entry->key, value_ptr);
    break;
  default:
    perror("Error: Invalid Data Type");
  }
}