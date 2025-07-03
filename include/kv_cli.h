# pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "kv_controller.h"

#define CLI_COMMAND_LOAD "load"
#define CLI_COMMAND_USE "use"
#define CLI_COMMAND_LIST "list"
#define CLI_COMMAND_HELP "help"
#define CLI_COMMAND_EXIT "exit"

#define CLI_COMMAND_PUT "put"
#define CLI_COMMAND_GET "get"
#define CLI_COMMAND_DELETE "delete"
#define CLI_COMMAND_PRINT "print"

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


extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type);
extern void start_cli();
static int64_t start_use(uint64_t db_idx);
static cli_cmd_t *get_cmd(uint8_t *msg);
static int64_t load_command(cli_cmd_t *cmd_ptr);
static int64_t list_command(cli_cmd_t *cmd_ptr);
static int64_t use_command(cli_cmd_t *cmd_ptr);
static int64_t put_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);
extern void free_cli_command(cli_cmd_t *cmd_ptr);
extern void free_cli_db(cli_db_t *cli_db);
static int64_t print_command(cli_db_t *cli_db);
