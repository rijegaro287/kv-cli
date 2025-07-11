#pragma once

#include "string_conversion.h"
#include "logger.h"
#include "constants.h"

#define TYPE_DELIMETER KV_PARSER_TYPE_DELIMITER
#define KEY_DELIMETER KV_PARSER_KEY_DELIMITER
#define VALUE_DELIMETER KV_PARSER_VALUE_DELIMITER

typedef struct _db_entry_t {
  int64_t type;
  uint8_t key[SM_BUFFER_SIZE];
  void *value;
} db_entry_t;

static int64_t set_integer_value(db_entry_t *dest, uint8_t *str_value, uint64_t type_size);
static int64_t set_float_value(db_entry_t *dest, uint8_t *str_value);
static int64_t set_double_value(db_entry_t *dest, uint8_t *str_value);
static int64_t set_bool_value(db_entry_t *dest, uint8_t *str_value);
static int64_t set_string_value(db_entry_t *dest, uint8_t *str_value);
extern int64_t set_entry_value(db_entry_t *dest, uint8_t *str_value);
extern db_entry_t* create_entry(uint8_t *key, uint8_t *value, uint8_t *type);
extern int64_t update_entry(db_entry_t *entry, uint8_t* key, uint8_t* value, uint8_t* type);
extern db_entry_t* parse_line(uint8_t *line);
extern void parse_entry(db_entry_t *entry, uint8_t *dest, uint64_t max_len);
extern void free_entry(db_entry_t *entry);
extern void print_entry(db_entry_t *entry);
