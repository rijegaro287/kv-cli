# pragma once

#include <stdio.h>
#include <stdint.h>

#include "kv_controller.h"

#define CLI_STR_COMMAND_LOAD "load"
#define CLI_STR_COMMAND_USE "use"
#define CLI_STR_COMMAND_LIST "list"
#define CLI_STR_COMMAND_HELP "help"
#define CLI_STR_COMMAND_EXIT "exit"

typedef struct _cli_db_t {
  uint8_t path[LINE_BUFFER_SIZE];
  uint8_t id[CLI_CMD_BUFFER_SIZE];
  db_t *db;
} cli_db_t;

typedef struct _cli_cmd_t {
  uint8_t cmd[CLI_CMD_BUFFER_SIZE];
  uint8_t param_1[CLI_CMD_BUFFER_SIZE];
  uint8_t param_2[CLI_CMD_BUFFER_SIZE];
  uint8_t param_3[CLI_CMD_BUFFER_SIZE];
} cli_cmd_t;

static cli_db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];
static uint64_t db_count = 0;

static cli_cmd_t *get_cmd();

static int64_t load_command(cli_cmd_t *cmd);
static int64_t list_command(cli_cmd_t *cmd);
static int64_t use_command(cli_cmd_t *cmd);

static int64_t start_use(uint64_t db_idx);

extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type);

extern void start_cli();
extern void free_cli_db(cli_db_t* cli_db);
