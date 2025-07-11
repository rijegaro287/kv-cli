#pragma once

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include "logger.h"

#define INT8_TYPE_STR "int8"
#define INT16_TYPE_STR "int16"
#define INT32_TYPE_STR "int32"
#define INT64_TYPE_STR "int64"
#define FLOAT_TYPE_STR "float"
#define DOUBLE_TYPE_STR "double"
#define BOOL_TYPE_STR "bool"
#define STR_TYPE_STR "string"

enum ENTRY_VALUE_TYPE {
  INT8_TYPE,
  INT16_TYPE,
  INT32_TYPE,
  INT64_TYPE,
  FLOAT_TYPE,
  DOUBLE_TYPE,
  BOOL_TYPE,
  STR_TYPE
};

extern int64_t map_datatype_from_str(uint8_t *type);
extern void map_datatype_to_str(uint64_t type, uint8_t *dest, uint64_t max_len);
extern void map_value_to_str(uint64_t type, void *value, uint8_t *dest, uint64_t max_len);
extern int64_t str_to_int64(uint8_t *str_value, int64_t *dest);
extern int64_t str_to_float(uint8_t *str_value, float *dest);
extern int64_t str_to_double(uint8_t *str_value, double *dest);
extern int64_t str_to_bool(uint8_t *str_value, bool *dest);
