#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "unity.h"
#include "kv_cli.h"

static cli_cmd_t* create_and_validate_command(const char* command_format, ...);
static int64_t load_and_validate_database(const char* path, const char* id, const char* storage_type, int64_t expected_db_count);
static int64_t validate_failed_load(const char* path, const char* id, const char* storage_type, int64_t expected_db_count);
static void validate_command_properties(cli_cmd_t *cmd, const char* expected_cmd,
                                        const char* expected_param1, const char* expected_param2, const char* expected_param3);
static int64_t put_key_value(cli_db_t *cli_db, const char* key, const char* value, const char* type);
static int64_t get_key_value(cli_db_t *cli_db, const char* key);
static int64_t delete_key_value(cli_db_t *cli_db, const char* key);
static void test_create_cli_db_valid_inputs();
static void test_create_cli_db_null_inputs();
static void test_create_cli_db_invalid_inputs();
static void test_create_command_valid_input();
static void test_create_command_all_commands();
static void test_create_command_null_inputs();
static void test_create_command_invalid_inputs();
static void test_load_command_valid_inputs();
static void test_load_command_null_inputs();
static void test_load_command_invalid_inputs();
static void test_load_command_max_databases_reached();
static void test_load_command_duplicate_id();
static void test_load_command_duplicate_path();
static void test_close_command_valid_inputs();
static void test_close_command_null_inputs();
static void test_close_command_invalid_inputs();
static void test_reload_command_valid_inputs();
static void test_reload_command_null_inputs();
static void test_reload_command_invalid_inputs();
static void test_list_command_valid_no_databases();
static void test_list_command_valid_with_databases();
static void test_list_command_null_inputs();
static void test_use_command_null_inputs();
static void test_use_command_invalid_inputs();
static void test_put_command_valid_inputs();
static void test_put_command_update_value();
static void test_put_command_null_inputs();
static void test_put_command_invalid_inputs();
static void test_get_command_valid_inputs();
static void test_get_command_null_inputs();
static void test_get_command_invalid_inputs();
static void test_delete_command_valid_inputs();
static void test_delete_command_null_inputs();
static void test_delete_command_invalid_inputs();
static void test_print_command_valid_inputs();
static void test_print_command_null_inputs();
static void test_main_help_command_valid();
static void test_use_help_command_valid();
static void test_free_cli_command_valid();
static void test_free_cli_command_null_input();
static void test_free_cli_db_valid();
static void test_free_cli_db_null_input();

extern void setUp(void);
extern void tearDown(void);
extern int main();
