#include "string_conversion.h"

extern int64_t map_datatype_from_str(uint8_t *type) {
  if (type == NULL) {
    logger(3, "Error: NULL pointer passed to map_datatype_from_str\n");
    return -1;
  }

  if (strcmp(type, INT8_TYPE_STR) == 0) return INT8_TYPE;
  else if (strcmp(type, INT16_TYPE_STR) == 0) return INT16_TYPE;
  else if (strcmp(type, INT32_TYPE_STR) == 0) return INT32_TYPE;
  else if (strcmp(type, INT64_TYPE_STR) == 0) return INT64_TYPE;
  else if (strcmp(type, FLOAT_TYPE_STR) == 0) return FLOAT_TYPE;
  else if (strcmp(type, DOUBLE_TYPE_STR) == 0) return DOUBLE_TYPE;
  else if (strcmp(type, BOOL_TYPE_STR) == 0) return BOOL_TYPE;
  else if (strcmp(type, STR_TYPE_STR) == 0) return STR_TYPE;
  else {
    logger(3, "Error: data type %s is not a valid datatype.\n", type);
    return -1;
  }
}

extern void map_datatype_to_str(uint64_t type, uint8_t *dest, uint64_t max_len) {
  if (dest == NULL) {
    logger(3, "Error: NULL pointer passed to map_datatype_to_str\n");
    return;
  }

  switch (type) {
  case INT8_TYPE:
    strncpy(dest, INT8_TYPE_STR, max_len);
    break;
  case INT16_TYPE:
    strncpy(dest, INT16_TYPE_STR, max_len);
    break;
  case INT32_TYPE:
    strncpy(dest, INT32_TYPE_STR, max_len);
    break;
  case INT64_TYPE:
    strncpy(dest, INT64_TYPE_STR, max_len);
    break;
  case FLOAT_TYPE:
    strncpy(dest, FLOAT_TYPE_STR, max_len);
    break;
  case DOUBLE_TYPE:
    strncpy(dest, DOUBLE_TYPE_STR, max_len);
    break;
  case BOOL_TYPE:
    strncpy(dest, BOOL_TYPE_STR, max_len);
    break;
  case STR_TYPE:
    strncpy(dest, STR_TYPE_STR, max_len);
    break;
  default:
    dest[0] = '\0';
    return;
  }
  dest[max_len - 1] = '\0';
}

extern void map_value_to_str(uint64_t type, void *value, uint8_t *dest, uint64_t max_len) {
  if (value == NULL || dest == NULL) {
    logger(3, "Error: NULL pointer passed to map_value_to_str\n");
    return;
  }

  switch (type) {
  case INT8_TYPE:
    snprintf(dest, max_len, "%" PRId8, *(int8_t*)value);
    break;
  case INT16_TYPE:
    snprintf(dest, max_len, "%" PRId16, *(int16_t*)value);
    break;
  case INT32_TYPE:
    snprintf(dest, max_len, "%" PRId32, *(int32_t*)value);
    break;
  case INT64_TYPE:
    snprintf(dest, max_len, "%" PRId64, *(int64_t*)value);
    break;
  case FLOAT_TYPE:
    snprintf(dest, max_len, "%.7f", *(float*)value);
    break;
    case DOUBLE_TYPE:
    snprintf(dest, max_len, "%.15lf", *(double*)value);
    break;
    case BOOL_TYPE:
    snprintf(dest, max_len, "%s", *(bool*)value ? "true" : "false");
    break;
      case STR_TYPE:
    snprintf(dest, max_len, "%s", (uint8_t*)value);
    break;
  default:
    dest[0] = '\0';
    return;
  }
  dest[max_len - 1] = '\0';
}

extern int64_t str_to_int64(uint8_t *str_value, int64_t *dest) {
  if (str_value == NULL || dest == NULL) {
    logger(3, "Error: NULL pointer passed to str_to_int64\n");
    return -1;
  }

  errno = 0;
  uint8_t *end;
  int64_t int_value = strtoll(str_value, (char**)&end, 10);

  if (str_value == end) {
    logger(3, "Error %s is not a number\n", str_value);
    return -1;
  }
  else if (errno == ERANGE) {
    logger(3, "Error %s value is out of range\n", str_value);
    return -1;
  }

  memcpy(dest, &int_value, sizeof(int64_t));

  return 0;
}

extern int64_t str_to_float(uint8_t *str_value, float *dest) {
  if (str_value == NULL || dest == NULL) {
    logger(3, "Error: NULL pointer passed to str_to_float\n");
    return -1;
  }

  errno = 0;
  uint8_t *end;
  float float_value = strtof(str_value, (char**)&end);

  if (str_value == end) {
    logger(3, "Error %s is not a number\n", str_value);
    return -1;
  }
  else if (errno == ERANGE) {
    logger(3, "Error %s value is out of range\n", str_value);
    return -1;
  }

  memcpy(dest, &float_value, sizeof(float));

  return 0;
}

extern int64_t str_to_double(uint8_t *str_value, double *dest) {
  if (str_value == NULL || dest == NULL) {
    logger(3, "Error: NULL pointer passed to str_to_double\n");
    return -1;
  }

  errno = 0;
  uint8_t *end;
  double double_value = strtod(str_value, (char**)&end);

  if (str_value == end) {
    logger(3, "Error %s is not a number\n", str_value);
    return -1;
  }
  else if (errno == ERANGE) {
    logger(3, "Error %s value is out of range\n", str_value);
    return -1;
  }

  memcpy(dest, &double_value, sizeof(double));

  return 0;
}

extern int64_t str_to_bool(uint8_t *str_value, bool *dest) {
  if (str_value == NULL || dest == NULL) {
    logger(3, "Error: NULL pointer passed to str_to_bool\n");
    return -1;
  }
  
  if (strcmp(str_value, "true") == 0) {
    bool bool_value = true;
    memcpy(dest, &bool_value, sizeof(bool));
  }
  else if (strcmp(str_value, "false") == 0) {
    bool bool_value = false;
    memcpy(dest, &bool_value, sizeof(bool));
  }
  else {
    logger(3, "Error: Invalid boolean value %s\n", str_value);
    return -1;
  }
  return 0;
}
