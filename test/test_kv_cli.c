#include "test_kv_cli.h"

static cli_cmd_t* create_and_validate_command(const char* command_format, ...) {
  uint8_t command[BG_BUFFER_SIZE];
  va_list args;
  va_start(args, command_format);
  vsnprintf(command, BG_BUFFER_SIZE, command_format, args);
  va_end(args);
  
  cli_cmd_t *cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  return cmd;
}

static int64_t load_and_validate_database(const char* path, const char* id, const char* storage_type, int64_t expected_db_count) {
  cli_cmd_t *cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_LOAD, path, id, storage_type);
  int64_t result = load_command(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  TEST_ASSERT_EQUAL(expected_db_count, get_db_count());
  
  if (result >= 0 && expected_db_count > 0) {
    cli_db_t *cli_db = get_db_list()[get_db_count()-1];
    TEST_ASSERT_EQUAL_STRING(path, cli_db->path);
    TEST_ASSERT_EQUAL_STRING(id, cli_db->id);
    TEST_ASSERT_NOT_NULL(cli_db->db);
    TEST_ASSERT_EQUAL_STRING(storage_type, cli_db->db->storage_type);
    TEST_ASSERT_NOT_NULL(cli_db->db->storage);
  }
  
  free_cli_command(cmd);
  return result;
}

static int64_t validate_failed_load(const char* path, const char* id, const char* storage_type, int64_t expected_db_count) {
  cli_cmd_t *cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_LOAD, path, id, storage_type);
  int64_t result = load_command(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, result);
  TEST_ASSERT_EQUAL(expected_db_count, get_db_count());
  
  free_cli_command(cmd);
  return result;
}

static void validate_command_properties(cli_cmd_t *cmd, const char* expected_cmd, 
                                        const char* expected_param1, const char* expected_param2, const char* expected_param3) {
  TEST_ASSERT_NOT_NULL(cmd);  
  TEST_ASSERT_EQUAL_STRING(expected_cmd, cmd->cmd);
  
  if (expected_param1) {
    TEST_ASSERT_EQUAL_STRING(expected_param1, cmd->param_1);
  }
  if (expected_param2) {
    TEST_ASSERT_EQUAL_STRING(expected_param2, cmd->param_2);
  }
  if (expected_param3) {
    TEST_ASSERT_EQUAL_STRING(expected_param3, cmd->param_3);
  }
}

static int64_t put_key_value(cli_db_t *cli_db, const char* key, const char* value, const char* type) {
  cli_cmd_t *cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_PUT, key, value, type);
  int64_t result = put_command(cli_db, cmd);
  free_cli_command(cmd);
  return result;
}

static int64_t get_key_value(cli_db_t *cli_db, const char* key) {
  cli_cmd_t *cmd = create_and_validate_command("%s %s", CLI_COMMAND_GET, key);
  int64_t result = get_command(cli_db, cmd);
  free_cli_command(cmd);
  return result;
}

static int64_t delete_key_value(cli_db_t *cli_db, const char* key) {
  cli_cmd_t *cmd = create_and_validate_command("%s %s", CLI_COMMAND_DELETE, key);
  int64_t result = delete_command(cli_db, cmd);
  free_cli_command(cmd);
  return result;
}

static void test_create_cli_db_valid_inputs() {
  logger(4, "*** test_create_cli_db_valid_inputs ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_EQUAL_STRING("test.db", cli_db->path);
  TEST_ASSERT_EQUAL_STRING("testdb", cli_db->id);
  TEST_ASSERT_NOT_NULL(cli_db->db);
  free_cli_db(cli_db);

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_HASH);
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_EQUAL_STRING("test.db", cli_db->path);
  TEST_ASSERT_EQUAL_STRING("testdb", cli_db->id);
  TEST_ASSERT_NOT_NULL(cli_db->db);
  free_cli_db(cli_db);
}

static void test_create_cli_db_null_inputs() {
  logger(4, "*** test_create_cli_db_null_inputs ***\n");
  cli_db_t *cli_db;
  
  cli_db = create_cli_db(NULL, NULL, NULL);
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db(NULL, "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", NULL, KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "testdb", NULL);
  TEST_ASSERT_NULL(cli_db);
}

static void test_create_cli_db_invalid_inputs() {
  logger(4, "*** test_create_cli_db_invalid_inputs ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "testdb", "");
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "testdb", "INVALID");
  TEST_ASSERT_NULL(cli_db);
}

static void test_create_command_valid_input() {
  logger(4, "*** test_create_command_valid_input ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "42", INT32_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_PUT, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("key1", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("42", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(INT32_TYPE_STR, cmd->param_3);
  free_cli_command(cmd);
}

static void test_create_command_all_commands() {
  logger(4, "*** test_create_command_all_commands ***\n");
  cli_cmd_t *cmd;

  cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_LOAD, "./test/test.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  validate_command_properties(cmd, CLI_COMMAND_LOAD, "./test/test.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s %s %s", CLI_COMMAND_RELOAD, "db1", KV_STORAGE_STRUCTURE_LIST);
  validate_command_properties(cmd, CLI_COMMAND_RELOAD, "db1", KV_STORAGE_STRUCTURE_LIST, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s", CLI_COMMAND_LIST);
  validate_command_properties(cmd, CLI_COMMAND_LIST, NULL, NULL, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s %s", CLI_COMMAND_USE, "db1");
  validate_command_properties(cmd, CLI_COMMAND_USE, "db1", NULL, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s", CLI_COMMAND_HELP);
  validate_command_properties(cmd, CLI_COMMAND_HELP, NULL, NULL, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s", CLI_COMMAND_EXIT);
  validate_command_properties(cmd, CLI_COMMAND_EXIT, NULL, NULL, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s %s", CLI_COMMAND_GET, "key1");
  validate_command_properties(cmd, CLI_COMMAND_GET, "key1", NULL, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s %s", CLI_COMMAND_DELETE, "key1");
  validate_command_properties(cmd, CLI_COMMAND_DELETE, "key1", NULL, NULL);
  free_cli_command(cmd);

  cmd = create_and_validate_command("%s", CLI_COMMAND_PRINT);
  validate_command_properties(cmd, CLI_COMMAND_PRINT, NULL, NULL, NULL);
  free_cli_command(cmd);
}

static void test_create_command_null_inputs() {
  logger(4, "*** test_create_command_null_inputs ***\n");
  TEST_ASSERT_NULL(create_command(NULL));
}

static void test_create_command_invalid_inputs() {
  logger(4, "*** test_create_command_invalid_inputs ***\n");

  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           "invalid", "./test/test.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("./test/test.db", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_3);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "invalid", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_3);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/test.db", "invalid", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("./test/test.db", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_3);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/test.db", "db1", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("./test/test.db", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_3);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           "invalid", "invalid", "invalid", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_3);
  free_cli_command(cmd);
}

static void test_load_command_valid_inputs() {
  logger(4, "*** test_load_command_valid_inputs ***\n");
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);

  free_db_list();
}

static void test_load_command_null_inputs() {
  logger(4, "*** test_load_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, load_command(NULL));
}

static void test_load_command_invalid_inputs() {
  logger(4, "*** test_load_command_invalid_inputs ***\n");
  validate_failed_load("", "db1", KV_STORAGE_STRUCTURE_LIST, 0);
  validate_failed_load("./test/data/test_1.db", "", KV_STORAGE_STRUCTURE_LIST, 0);
  validate_failed_load("./test/data/test_1.db", "db1", "", 0);
  validate_failed_load("", "", "", 0);
  validate_failed_load("invalid", "db1", KV_STORAGE_STRUCTURE_LIST, 0);
  validate_failed_load("./test/data/test_1.db", "db1", "invalid", 0);

  free_db_list();
}

static void test_load_command_max_databases_reached() {
  logger(4, "*** test_load_command_max_databases_reached ***\n");
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_LIST, 2);
  validate_failed_load("./test/data/test_3.db", "db3", KV_STORAGE_STRUCTURE_LIST, 2);

  free_db_list();
}

static void test_load_command_duplicate_id() {
  logger(4, "*** test_load_command_duplicate_id ***\n");
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  validate_failed_load("./test/data/test_2.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);

  free_db_list();
}

static void test_load_command_duplicate_path() {
  logger(4, "*** test_load_command_duplicate_path ***\n");
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  validate_failed_load("./test/data/test_1.db", "db2", KV_STORAGE_STRUCTURE_LIST, 1);

  free_db_list();
}

static void test_close_command_valid_inputs() {
  logger(4, "*** test_close_command_valid_inputs ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_CLOSE, "db1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, close_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);
  
  cli_db_t *remaining_db = get_db_list()[1];
  if (remaining_db != NULL) {
    TEST_ASSERT_EQUAL_STRING("db2", remaining_db->id);
    TEST_ASSERT_EQUAL_STRING("./test/data/test_2.db", remaining_db->path);
  }

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_CLOSE, "db2");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, close_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_close_command_null_inputs() {
  logger(4, "*** test_close_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, close_command(NULL));
}

static void test_close_command_invalid_inputs() {
  logger(4, "*** test_close_command_invalid_inputs ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_CLOSE, "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, close_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_CLOSE, "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, close_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_reload_command_valid_inputs() {
  logger(4, "*** test_reload_command_valid_inputs ***\n");
  cli_cmd_t *cmd;
  cli_db_t *reloaded_db;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "db1", KV_STORAGE_STRUCTURE_HASH);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  
  reloaded_db = get_db_list()[0];
  TEST_ASSERT_EQUAL_STRING("./test/data/test_1.db", reloaded_db->path);
  TEST_ASSERT_EQUAL_STRING("db1", reloaded_db->id);
  TEST_ASSERT_NOT_NULL(reloaded_db->db);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_HASH, reloaded_db->db->storage_type);

  free_cli_command(cmd);
  free_db_list();
}

static void test_reload_command_null_inputs() {
  logger(4, "*** test_reload_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, reload_command(NULL));
}

static void test_reload_command_invalid_inputs() {
  logger(4, "*** test_reload_command_invalid_inputs ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "", KV_STORAGE_STRUCTURE_HASH);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "db1", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "invalid", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "db1", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "invalid", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_list_command_valid_no_databases() {
  logger(4, "*** test_list_command_valid_no_databases ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, list_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_list_command_valid_with_databases() {
  logger(4, "*** test_list_command_valid_with_databases ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];
  
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, list_command(cmd));
  TEST_ASSERT_EQUAL(2, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_list_command_null_inputs() {
  TEST_ASSERT_EQUAL(-1, list_command(NULL));
}

static void test_use_command_null_inputs() {
  TEST_ASSERT_EQUAL(-1, use_command(NULL));
}

static void test_use_command_invalid_inputs() {
  logger(4, "*** test_use_command_invalid_inputs ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  
  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_USE, "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, use_command(cmd));
  free_cli_command(cmd);
  
  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_USE, "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, use_command(cmd));
  free_cli_command(cmd);

  free_db_list();
}

static void test_put_command_valid_inputs() {
  logger(4, "*** test_put_command_valid_inputs ***\n");
  cli_db_t *cli_db;
  
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "12", INT8_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key2", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key3", "3.14", FLOAT_TYPE_STR));
  
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);
  cli_db = get_db_list()[get_db_count()-1];
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "12", INT8_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key2", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key3", "3.14", FLOAT_TYPE_STR));
  
  free_db_list();
}

static void test_put_command_update_value() {
  logger(4, "*** test_put_command_update_value ***\n");
  cli_db_t *cli_db;
  db_entry_t *entry;

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "12", INT32_TYPE_STR));
  entry = get_entry(cli_db->db, "key1");
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL(*(int32_t*)entry->value, 12);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "152", INT32_TYPE_STR));
  entry = get_entry(cli_db->db, "key1");
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL(*(int32_t*)entry->value, 152);

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "23", ""));
  
  entry = get_entry(cli_db->db, "key1");
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL(*(int32_t*)entry->value, 23);

  free_db_list();
}

static void test_put_command_null_inputs() {
  logger(4, "*** test_put_command_null_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "31", INT32_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);

  TEST_ASSERT_EQUAL(-1, put_command(NULL, cmd));
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, NULL));
  TEST_ASSERT_EQUAL(-1, put_command(NULL, NULL));
  free_cli_command(cmd);

  free_db_list();
}

static void test_put_command_invalid_inputs() {
  logger(4, "*** test_put_command_invalid_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];  // Add this line to properly initialize cli_db

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "", "12", INT32_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "key1", "", INT32_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "key1", "12", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "key1", "12", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  free_db_list();
}

static void test_get_command_valid_inputs() {
  logger(4, "*** test_get_command_valid_inputs ***\n");
  cli_db_t *cli_db;

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "testkey", "13", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, get_key_value(cli_db, "testkey"));

  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "testkey", "13", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, get_key_value(cli_db, "testkey"));

  free_db_list();
}

static void test_get_command_null_inputs() {
  logger(4, "*** test_get_command_null_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(NULL, cmd));
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, NULL));
  TEST_ASSERT_EQUAL(-1, get_command(NULL, NULL));
  free_cli_command(cmd);

  free_db_list();
}

static void test_get_command_invalid_inputs() {
  logger(4, "*** test_get_command_invalid_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_GET);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "nonexistent");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, cmd));
  free_cli_command(cmd);

  free_db_list();
}

static void test_delete_command_valid_inputs() {
  logger(4, "*** test_delete_command_valid_inputs ***\n");
  cli_db_t *cli_db;

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "deletekey", "31", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, delete_key_value(cli_db, "deletekey"));

  free_db_list();
}

static void test_delete_command_null_inputs() {
  logger(4, "*** test_delete_command_null_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);

  TEST_ASSERT_EQUAL(-1, delete_command(NULL, cmd));
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, NULL));
  TEST_ASSERT_EQUAL(-1, delete_command(NULL, NULL));
  free_cli_command(cmd);

  free_db_list();
}

static void test_delete_command_invalid_inputs() {
  logger(4, "*** test_delete_command_invalid_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_DELETE);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "nonexistent");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, cmd));
  free_cli_command(cmd);

  free_db_list();
}

static void test_print_command_valid_inputs() {
  logger(4, "*** test_print_command_valid_inputs ***\n");
  cli_db_t *cli_db;

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, print_command(cli_db));

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "printkey", "41", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, print_command(cli_db));

  free_db_list();
}

static void test_print_command_null_inputs() {
  logger(4, "*** test_print_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, print_command(NULL));
}

static void test_main_help_command_valid() {
  logger(4, "*** test_main_help_command_valid ***\n");
  TEST_ASSERT_GREATER_OR_EQUAL(0, main_help_command());
}

static void test_use_help_command_valid() {
  logger(4, "*** test_use_help_command_valid ***\n");
  TEST_ASSERT_GREATER_OR_EQUAL(0, use_help_command());
}

static void test_free_cli_command_valid() {
  logger(4, "*** test_free_cli_command_valid ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "12", INT32_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  
  free_cli_command(cmd);
}

static void test_free_cli_command_null_input() {
  logger(4, "*** test_free_cli_command_null_input ***\n");
  free_cli_command(NULL);
}

static void test_free_cli_db_valid() {
  logger(4, "*** test_free_cli_db_valid ***\n");
  cli_db_t *cli_db;

  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  free_db_list();
}

static void test_free_cli_db_null_input() {
  logger(4, "*** test_free_cli_db_null_input ***\n");
  free_cli_db(NULL);
}

extern void setUp(void) {
}

extern void tearDown(void) {
}

extern int main() {
  UNITY_BEGIN();
  
  RUN_TEST(test_create_cli_db_valid_inputs);
  RUN_TEST(test_create_cli_db_null_inputs);
  RUN_TEST(test_create_cli_db_invalid_inputs);

  RUN_TEST(test_create_command_valid_input);
  RUN_TEST(test_create_command_all_commands);
  RUN_TEST(test_create_command_null_inputs);
  RUN_TEST(test_create_command_invalid_inputs);
  
  RUN_TEST(test_load_command_valid_inputs);
  RUN_TEST(test_load_command_null_inputs);
  RUN_TEST(test_load_command_invalid_inputs);
  RUN_TEST(test_load_command_max_databases_reached);
  RUN_TEST(test_load_command_duplicate_id);
  RUN_TEST(test_load_command_duplicate_path);
  
  RUN_TEST(test_close_command_valid_inputs);
  RUN_TEST(test_close_command_null_inputs);
  RUN_TEST(test_close_command_invalid_inputs);

  RUN_TEST(test_reload_command_valid_inputs);
  RUN_TEST(test_reload_command_null_inputs);
  RUN_TEST(test_reload_command_invalid_inputs);
  
  RUN_TEST(test_list_command_valid_no_databases);
  RUN_TEST(test_list_command_valid_with_databases);
  RUN_TEST(test_list_command_null_inputs);
  
  RUN_TEST(test_use_command_null_inputs);
  RUN_TEST(test_use_command_invalid_inputs);
  
  RUN_TEST(test_put_command_valid_inputs);
  RUN_TEST(test_put_command_update_value);
  RUN_TEST(test_put_command_null_inputs);
  RUN_TEST(test_put_command_invalid_inputs);
  
  RUN_TEST(test_get_command_valid_inputs);
  RUN_TEST(test_get_command_null_inputs);
  RUN_TEST(test_get_command_invalid_inputs);
  
  RUN_TEST(test_delete_command_valid_inputs);
  RUN_TEST(test_delete_command_null_inputs);
  RUN_TEST(test_delete_command_invalid_inputs);
  
  RUN_TEST(test_print_command_valid_inputs);
  RUN_TEST(test_print_command_null_inputs);
  
  RUN_TEST(test_main_help_command_valid);
  RUN_TEST(test_use_help_command_valid);

  RUN_TEST(test_free_cli_command_valid);
  RUN_TEST(test_free_cli_command_null_input);

  RUN_TEST(test_free_cli_db_valid);
  RUN_TEST(test_free_cli_db_null_input);

  return UNITY_END();
}
