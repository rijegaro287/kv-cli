# pragma once

#include "kv_controller.h"

#define CLI_COMMAND_LOAD "load"
#define CLI_COMMAND_RELOAD "reload"
#define CLI_COMMAND_CLOSE "close"
#define CLI_COMMAND_LIST "list"
#define CLI_COMMAND_USE "use"
#define CLI_COMMAND_HELP "help"
#define CLI_COMMAND_EXIT "exit"

#define CLI_COMMAND_PUT "put"
#define CLI_COMMAND_GET "get"
#define CLI_COMMAND_DELETE "delete"
#define CLI_COMMAND_PRINT "print"

typedef struct _cli_db_t {
  uint8_t path[BG_BUFFER_SIZE];
  uint8_t id[SM_BUFFER_SIZE];
  db_t *db;
} cli_db_t;

typedef struct _cli_cmd_t {
  uint8_t cmd[SM_BUFFER_SIZE];
  uint8_t param_1[SM_BUFFER_SIZE];
  uint8_t param_2[SM_BUFFER_SIZE];
  uint8_t param_3[SM_BUFFER_SIZE];
} cli_cmd_t;

static cli_db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];
static uint64_t db_count = 0;

extern cli_db_t **get_db_list();
extern uint64_t get_db_count();

extern int64_t get_command_from_user(uint8_t *msg, uint8_t *dest, uint64_t max_len);
extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type);
extern cli_cmd_t *create_command(uint8_t *command);
extern int64_t start_use(uint64_t db_idx);
extern int64_t load_command(cli_cmd_t *cmd_ptr);
extern int64_t reload_command(cli_cmd_t *cmd_ptr);
extern int64_t close_command(cli_cmd_t *cmd_ptr);
extern int64_t list_command(cli_cmd_t *cmd_ptr);
extern int64_t use_command(cli_cmd_t *cmd_ptr);
extern int64_t put_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);
extern int64_t get_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);
extern int64_t delete_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);
extern int64_t print_command(cli_db_t *cli_db);
extern int64_t main_help_command();
extern int64_t use_help_command();

extern void start_cli();
extern void free_cli_db(cli_db_t *cli_db);
extern void free_cli_command(cli_cmd_t *cmd_ptr);
extern void free_db_list();
