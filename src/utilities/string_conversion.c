#include "string_conversion.h"

extern int64_t map_data_type_str(uint8_t *type) {
  if (strcmp(type, INT8_TYPE_STR) == 0) return INT8_TYPE;
  else if (strcmp(type, INT16_TYPE_STR) == 0) return INT16_TYPE;
  else if (strcmp(type, INT32_TYPE_STR) == 0) return INT32_TYPE;
  else if (strcmp(type, INT64_TYPE_STR) == 0) return INT64_TYPE;
  else if (strcmp(type, FLOAT_TYPE_STR) == 0) return FLOAT_TYPE;
  else if (strcmp(type, DOUBLE_TYPE_STR) == 0) return DOUBLE_TYPE;
  else if (strcmp(type, BOOL_TYPE_STR) == 0) return BOOL_TYPE;
  else if (strcmp(type, STR_TYPE_STR) == 0) return STR_TYPE;
  else {
    fprintf(stderr, "Error: data type %s is not a valid datatype.\n", type);
    return -1;
  }
}

extern int64_t str_to_int64(uint8_t *str_value, int64_t *dest) {
  errno = 0;
  uint8_t *end;
  int64_t int_value = strtoll(str_value, (char**)&end, 10);

  if (str_value == end) {
    fprintf(stderr, "Error %s is not a number", str_value);
    return -1;
  }
  else if (errno == ERANGE) {
    fprintf(stderr, "Error %s value is out of range", str_value);
    return -1;
  }

  memcpy(dest, &int_value, sizeof(int64_t));

  return 0;
}

extern int64_t str_to_float(uint8_t *str_value, float *dest) {
  errno = 0;
  uint8_t *end;
  float float_value = strtof(str_value, (char**)&end);

  if (str_value == end) {
    fprintf(stderr, "Error %s is not a number", str_value);
    return -1;
  }
  else if (errno == ERANGE) {
    fprintf(stderr, "Error %s value is out of range", str_value);
    return -1;
  }

  memcpy(dest, &float_value, sizeof(float));

  return 0;
}

extern int64_t str_to_double(uint8_t *str_value, double *dest) {
  errno = 0;
  uint8_t *end;
  double double_value = strtod(str_value, (char**)&end);

  if (str_value == end) {
    fprintf(stderr, "Error %s is not a number", str_value);
    return -1;
  }
  else if (errno == ERANGE) {
    fprintf(stderr, "Error %s value is out of range", str_value);
    return -1;
  }

  memcpy(dest, &double_value, sizeof(double));

  return 0;
}

extern int64_t str_to_bool(uint8_t *str_value, bool *dest) {
  if (strcmp(str_value, "true") == 0) {
    bool bool_value = true;
    memcpy(dest, &bool_value, sizeof(bool));
    return true;
  }
  else if (strcmp(str_value, "false") == 0) {
    bool bool_value = false;
    memcpy(dest, &bool_value, sizeof(bool));
  }
  else {
    fprintf(stderr, "Error: Invalid boolean value %s\n", str_value);
    return -1;
  }

  return 0;
}
