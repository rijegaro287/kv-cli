# pragma once

#include <stdio.h>
#include <stdint.h>

#include "kv_controller.h"

#define CLI_STR_COMMAND_LOAD "load"
#define CLI_STR_COMMAND_LIST "list"
#define CLI_STR_COMMAND_HELP "help"
#define CLI_STR_COMMAND_EXIT "exit"

typedef struct _cli_command_t {
  uint8_t *cmd;
  uint8_t *param_1;
  uint8_t *param_2;
  uint8_t *param_3;
} cli_command_t;

enum CLI_COMMANDS {
  CLI_COMMAND_LOAD,
  CLI_COMMAND_LIST,
  CLI_COMMAND_HELP,
  CLI_COMMAND_EXIT
};

static db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];
static uint8_t db_ids[KV_CLI_MAX_OPEN_DATABASES][LINE_BUFFER_SIZE];
static uint64_t db_count = 0;

static int64_t main_menu(cli_command_t **command_ptr);
static int64_t load_command(cli_command_t *command);

extern void start_cli();
