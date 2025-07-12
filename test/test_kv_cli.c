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

static void test_command_with_result(const char* cmd_format, int64_t expected_result, ...) {
  uint8_t command[BG_BUFFER_SIZE];
  va_list args;
  va_start(args, expected_result);
  vsnprintf(command, BG_BUFFER_SIZE, cmd_format, args);
  va_end(args);
  
  cli_cmd_t *cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  
  // The result validation depends on the command type
  // This is a generic helper, specific validation should be done in the test
  
  free_cli_command(cmd);
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

static void test_null_input_single_param(int64_t (*test_function)(void*), const char* function_name) {
  logger(4, "*** Testing %s with NULL input ***\n", function_name);
  TEST_ASSERT_EQUAL(-1, test_function(NULL));
}

static void test_null_input_dual_param(int64_t (*test_function)(void*, void*), void* valid_param1, void* valid_param2, const char* function_name) {
  logger(4, "*** Testing %s with NULL inputs ***\n", function_name);
  TEST_ASSERT_EQUAL(-1, test_function(NULL, valid_param2));
  TEST_ASSERT_EQUAL(-1, test_function(valid_param1, NULL));
  TEST_ASSERT_EQUAL(-1, test_function(NULL, NULL));
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
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_PUT, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("key1", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("value1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(STR_TYPE_STR, cmd->param_3);
  free_cli_command(cmd);
}

static void test_create_command_all_commands() {
  logger(4, "*** test_create_command_all_commands ***\n");
  cli_cmd_t *cmd;

  // Test LOAD command
  cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_LOAD, "./test/test.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  validate_command_properties(cmd, CLI_COMMAND_LOAD, "./test/test.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  free_cli_command(cmd);

  // Test RELOAD command
  cmd = create_and_validate_command("%s %s %s", CLI_COMMAND_RELOAD, "db1", KV_STORAGE_STRUCTURE_LIST);
  validate_command_properties(cmd, CLI_COMMAND_RELOAD, "db1", KV_STORAGE_STRUCTURE_LIST, NULL);
  free_cli_command(cmd);

  // Test LIST command
  cmd = create_and_validate_command("%s", CLI_COMMAND_LIST);
  validate_command_properties(cmd, CLI_COMMAND_LIST, NULL, NULL, NULL);
  free_cli_command(cmd);

  // Test USE command
  cmd = create_and_validate_command("%s %s", CLI_COMMAND_USE, "db1");
  validate_command_properties(cmd, CLI_COMMAND_USE, "db1", NULL, NULL);
  free_cli_command(cmd);

  // Test HELP command
  cmd = create_and_validate_command("%s", CLI_COMMAND_HELP);
  validate_command_properties(cmd, CLI_COMMAND_HELP, NULL, NULL, NULL);
  free_cli_command(cmd);

  // Test EXIT command
  cmd = create_and_validate_command("%s", CLI_COMMAND_EXIT);
  validate_command_properties(cmd, CLI_COMMAND_EXIT, NULL, NULL, NULL);
  free_cli_command(cmd);

  // Test PUT command
  cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  validate_command_properties(cmd, CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  free_cli_command(cmd);

  // Test GET command
  cmd = create_and_validate_command("%s %s", CLI_COMMAND_GET, "key1");
  validate_command_properties(cmd, CLI_COMMAND_GET, "key1", NULL, NULL);
  free_cli_command(cmd);

  // Test DELETE command
  cmd = create_and_validate_command("%s %s", CLI_COMMAND_DELETE, "key1");
  validate_command_properties(cmd, CLI_COMMAND_DELETE, "key1", NULL, NULL);
  free_cli_command(cmd);

  // Test PRINT command
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

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "invalid", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_3);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/test.db", "invalid", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("./test/test.db", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("invalid", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_3);

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

  // Load first database (LIST structure)
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);

  // Load second database (HASH structure)
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);

  free_db_list();
}

static void test_load_command_null_inputs() {
  logger(4, "*** test_load_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, load_command(NULL));
}

static void test_load_command_invalid_inputs() {
  logger(4, "*** test_load_command_invalid_inputs ***\n");

  cli_db_t *loaded_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];
 
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);
 
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);
 
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "", "", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "invalid", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(0, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_load_command_max_databases_reached() {
  logger(4, "*** test_load_command_max_databases_reached ***\n");
  cli_cmd_t *cmd;

  // Load first database - should succeed
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);

  // Load second database - should succeed
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_LIST, 2);
  
  // Attempt to load third database - should fail (max databases reached)
  cmd = create_and_validate_command("%s %s %s %s", CLI_COMMAND_LOAD, "./test/data/test_3.db", "db3", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(2, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_load_command_duplicate_id() {
  logger(4, "*** test_load_command_duplicate_id ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_2.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  free_cli_command(cmd);
  free_db_list();
}

static void test_load_command_duplicate_path() {
  logger(4, "*** test_load_command_duplicate_path ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db2", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  free_cli_command(cmd);
  free_db_list();
}

static void test_reload_command_valid_inputs() {
  logger(4, "*** test_reload_command_valid_inputs ***\n");
  cli_cmd_t *cmd;
  cli_db_t *reloaded_db;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

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

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "", KV_STORAGE_STRUCTURE_HASH);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "db1", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "invalid", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "db1", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, reload_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

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
  
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(2, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, list_command(cmd));
  TEST_ASSERT_EQUAL(2, get_db_count());
  free_cli_command(cmd);

  free_db_list();
}

static void test_list_command_null_inputs() {
  test_null_input_single_param((int64_t (*)(void*))list_command, "list_command");
}

static void test_use_command_null_inputs() {
  test_null_input_single_param((int64_t (*)(void*))use_command, "use_command");
}

static void test_use_command_invalid_inputs() {
  logger(4, "*** test_use_command_invalid_inputs ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);
  
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

  // Load first database (LIST structure) and test put operations
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "value1", STR_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key2", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key3", "3.14", FLOAT_TYPE_STR));

  // Load second database (HASH structure) and test put operations
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key1", "value1", STR_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key2", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "key3", "3.14", FLOAT_TYPE_STR));

  free_db_list();
}

static void test_put_command_null_inputs() {
  logger(4, "*** test_put_command_null_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
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

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "", "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "key1", "", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "key1", "value1", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s",
           CLI_COMMAND_PUT, "key1", "value1", "invalid");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  free_cli_command(cmd);

  free_db_list();
}

static void test_get_command_valid_inputs() {
  logger(4, "*** test_get_command_valid_inputs ***\n");
  cli_db_t *cli_db;

  // Test with LIST structure database
  load_and_validate_database("./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST, 1);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "testkey", "testvalue", STR_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, get_key_value(cli_db, "testkey"));

  // Test with HASH structure database
  load_and_validate_database("./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH, 2);
  cli_db = get_db_list()[get_db_count()-1];

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "testkey", "testvalue", STR_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, get_key_value(cli_db, "testkey"));

  free_db_list();
}

static void test_get_command_null_inputs() {
  logger(4, "*** test_get_command_null_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

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

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

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

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "deletekey", "deletevalue", STR_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, delete_key_value(cli_db, "deletekey"));

  free_db_list();
}

static void test_delete_command_null_inputs() {
  logger(4, "*** test_delete_command_null_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

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

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

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

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_key_value(cli_db, "printkey", "printvalue", STR_TYPE_STR));
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
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
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
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());
  free_cli_command(cmd);

  cli_db = get_db_list()[get_db_count()-1];

  free_cli_db(cli_db);
}

static void test_free_cli_db_null_input() {
  logger(4, "*** test_free_cli_db_null_input ***\n");
  free_cli_db(NULL);
}

extern void setUp(void) {
  // Reset global state before each test
  // reset_global_db_state();
}

extern void tearDown(void) {
  // Clean up after each test
  // reset_global_db_state();
}

extern int main() {
  UNITY_BEGIN();
  
  // create_cli_db tests
  RUN_TEST(test_create_cli_db_valid_inputs);
  RUN_TEST(test_create_cli_db_null_inputs);
  RUN_TEST(test_create_cli_db_invalid_inputs);

  // create_command tests
  RUN_TEST(test_create_command_valid_input);
  RUN_TEST(test_create_command_all_commands);
  RUN_TEST(test_create_command_null_inputs);
  RUN_TEST(test_create_command_invalid_inputs);
  
  // load_command tests
  RUN_TEST(test_load_command_valid_inputs);
  RUN_TEST(test_load_command_null_inputs);
  RUN_TEST(test_load_command_invalid_inputs);
  RUN_TEST(test_load_command_max_databases_reached);
  RUN_TEST(test_load_command_duplicate_id);
  RUN_TEST(test_load_command_duplicate_path);
  
  // reload_command tests
  RUN_TEST(test_reload_command_valid_inputs);
  RUN_TEST(test_reload_command_null_inputs);
  RUN_TEST(test_reload_command_invalid_inputs);
  
  // list_command tests
  RUN_TEST(test_list_command_valid_no_databases);
  RUN_TEST(test_list_command_valid_with_databases);
  RUN_TEST(test_list_command_null_inputs);
  
  // use_command tests
  RUN_TEST(test_use_command_null_inputs);
  RUN_TEST(test_use_command_invalid_inputs);
  
  // put_command tests
  RUN_TEST(test_put_command_valid_inputs);
  RUN_TEST(test_put_command_null_inputs);
  RUN_TEST(test_put_command_invalid_inputs);
  
  // get_command tests
  RUN_TEST(test_get_command_valid_inputs);
  RUN_TEST(test_get_command_null_inputs);
  RUN_TEST(test_get_command_invalid_inputs);
  
  // delete_command tests
  RUN_TEST(test_delete_command_valid_inputs);
  RUN_TEST(test_delete_command_null_inputs);
  RUN_TEST(test_delete_command_invalid_inputs);
  
  // print_command tests
  RUN_TEST(test_print_command_valid_inputs);
  RUN_TEST(test_print_command_null_inputs);
  
  // help command tests
  RUN_TEST(test_main_help_command_valid);
  RUN_TEST(test_use_help_command_valid);

  // free_cli_command tests
  RUN_TEST(test_free_cli_command_valid);
  RUN_TEST(test_free_cli_command_null_input);

  // free_cli_db tests
  RUN_TEST(test_free_cli_db_valid);
  RUN_TEST(test_free_cli_db_null_input);

  return UNITY_END();
}
