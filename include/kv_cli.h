# pragma once

#include <stdio.h>
#include <stdint.h>

#include "kv_controller.h"

#define CLI_STR_COMMAND_LOAD "load"
#define CLI_STR_COMMAND_LIST "list"
#define CLI_STR_COMMAND_HELP "help"
#define CLI_STR_COMMAND_EXIT "exit"

typedef struct _cli_db_t {
  uint8_t path[DB_PATH_BUFFER_SIZE];
  uint8_t id[DB_ID_BUFFER_SIZE];
  db_t *db;
} cli_db_t;

typedef struct _cli_command_t {
  uint8_t *cmd;
  uint8_t *param_1;
  uint8_t *param_2;
  uint8_t *param_3;
} cli_command_t;

static cli_db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];
static uint64_t db_count = 0;

static int64_t load_command(cli_command_t *command);
static int64_t list_command(cli_command_t *command);

extern void start_cli();
