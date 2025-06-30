# pragma once

#include <stdio.h>
#include <stdint.h>

#include "kv_controller.h"

#define CLI_STR_COMMAND_LOAD "load"
#define CLI_STR_COMMAND_HELP "help"
#define CLI_STR_COMMAND_EXIT "exit"

typedef struct _cli_command_t {
  uint8_t *cmd;
  uint8_t *first_param;
  uint8_t *second_param;
} cli_command_t;

enum CLI_COMMANDS {
  CLI_COMMAND_LOAD,
  CLI_COMMAND_HELP,
  CLI_COMMAND_EXIT
};

static db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];
static uint8_t db_ids[KV_CLI_MAX_OPEN_DATABASES][LINE_BUFFER_SIZE];
static uint64_t db_count = 0;

extern int64_t main_menu();
extern void start_cli();

extern int64_t open_db(uint8_t *db_path, uint8_t *db_storage, uint8_t *db_id);