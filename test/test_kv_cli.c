#include "test_kv_cli.h"

// Since the global variables are static in kv_cli.h, we need to declare 
// external functions to access them for testing purposes
// These would need to be added to kv_cli.c for testing

// // Helper functions
// static cli_cmd_t* create_test_command(const char* cmd, const char* param_1, const char* param_2, const char* param_3) {
//   cli_cmd_t *cmd_ptr = malloc(sizeof(cli_cmd_t));
//   if (cmd_ptr == NULL) return NULL;
  
//   memset(cmd_ptr, 0, sizeof(cli_cmd_t));
  
//   if (cmd != NULL) {
//     strncpy(cmd_ptr->cmd, cmd, SM_BUFFER_SIZE - 1);
//     cmd_ptr->cmd[SM_BUFFER_SIZE - 1] = '\0';
//   }
  
//   if (param_1 != NULL) {
//     strncpy(cmd_ptr->param_1, param_1, SM_BUFFER_SIZE - 1);
//     cmd_ptr->param_1[SM_BUFFER_SIZE - 1] = '\0';
//   }
  
//   if (param_2 != NULL) {
//     strncpy(cmd_ptr->param_2, param_2, SM_BUFFER_SIZE - 1);
//     cmd_ptr->param_2[SM_BUFFER_SIZE - 1] = '\0';
//   }
  
//   if (param_3 != NULL) {
//     strncpy(cmd_ptr->param_3, param_3, SM_BUFFER_SIZE - 1);
//     cmd_ptr->param_3[SM_BUFFER_SIZE - 1] = '\0';
//   }
  
//   return cmd_ptr;
// }

// static cli_db_t* create_test_cli_db(const char* path, const char* id, const char* storage_type) {
//   if (path == NULL || id == NULL || storage_type == NULL) return NULL;
  
//   cli_db_t *cli_db = malloc(sizeof(cli_db_t));
//   if (cli_db == NULL) return NULL;
  
//   cli_db->db = create_db((uint8_t*)storage_type);
//   if (cli_db->db == NULL) {
//     free(cli_db);
//     return NULL;
//   }
  
//   strncpy(cli_db->path, path, BG_BUFFER_SIZE - 1);
//   cli_db->path[BG_BUFFER_SIZE - 1] = '\0';
  
//   strncpy(cli_db->id, id, SM_BUFFER_SIZE - 1);
//   cli_db->id[SM_BUFFER_SIZE - 1] = '\0';
  
//   return cli_db;
// }

// static void reset_global_db_state() {
//   // Since we can't access the global state directly due to static declarations,
//   // we'll rely on the load/reload commands to manage state in our tests.
//   // In a real testing scenario, you might want to add getter/setter functions
//   // to kv_cli.c for testing purposes.
// }

static void test_create_cli_db_valid_inputs() {
  logger(4, "*** test_create_cli_db_valid_inputs ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", "L");
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_EQUAL_STRING("test.db", cli_db->path);
  TEST_ASSERT_EQUAL_STRING("testdb", cli_db->id);
  TEST_ASSERT_NOT_NULL(cli_db->db);
  free_cli_db(cli_db);

  cli_db = create_cli_db("test.db", "testdb", "H");
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

  cli_db = create_cli_db(NULL, "testdb", "L");
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", NULL, "L");
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "testdb", NULL);
  TEST_ASSERT_NULL(cli_db);
}

static void test_create_cli_db_invalid_inputs() {
  logger(4, "*** test_create_cli_db_invalid_inputs ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("", "testdb", "L");
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "", "L");
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "testdb", "");
  TEST_ASSERT_NULL(cli_db);

  cli_db = create_cli_db("test.db", "testdb", "INVALID");
  TEST_ASSERT_NULL(cli_db);
}

// // Test functions for load_command
// static void test_load_command_valid_inputs() {
//   logger(4, "*** test_load_command_valid_inputs ***\n");
  
//   cli_cmd_t *cmd = create_test_command("load", "test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = load_command(cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
//   free_cli_command(cmd);
// }

// static void test_load_command_null_cmd_ptr() {
//   logger(4, "*** test_load_command_null_cmd_ptr ***\n");
  
//   int64_t result = load_command(NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// static void test_load_command_missing_param_1() {
//   logger(4, "*** test_load_command_missing_param_1 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("load", "", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = load_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_load_command_missing_param_2() {
//   logger(4, "*** test_load_command_missing_param_2 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("load", "test.db", "", "L");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = load_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_load_command_missing_param_3() {
//   logger(4, "*** test_load_command_missing_param_3 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("load", "test.db", "testdb", "");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = load_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_load_command_empty_params() {
//   logger(4, "*** test_load_command_empty_params ***\n");
  
//   cli_cmd_t *cmd = create_test_command("load", "", "", "");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = load_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_load_command_max_databases_reached() {
//   logger(4, "*** test_load_command_max_databases_reached ***\n");
  
//   // This test would require access to global state or a way to simulate
//   // max databases being reached. For now, we'll just test that the function
//   // handles the case gracefully.
  
//   cli_cmd_t *cmd = create_test_command("load", "new.db", "newdb", "L");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   // Note: This test cannot be fully validated without access to global state
//   // In a real scenario, you'd load KV_CLI_MAX_OPEN_DATABASES databases first
//   int64_t result = load_command(cmd);
//   // Result could be 0 (success) or -1 (failure), depending on current state
  
//   free_cli_command(cmd);
// }

// static void test_load_command_duplicate_id() {
//   logger(4, "*** test_load_command_duplicate_id ***\n");
  
//   // Load first database
//   cli_cmd_t *cmd1 = create_test_command("load", "test1.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cmd1);
  
//   int64_t result1 = load_command(cmd1);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result1);
  
//   // Try to load database with same ID
//   cli_cmd_t *cmd2 = create_test_command("load", "test2.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cmd2);
  
//   int64_t result2 = load_command(cmd2);
//   TEST_ASSERT_EQUAL(-1, result2);
  
//   free_cli_command(cmd1);
//   free_cli_command(cmd2);
// }

// static void test_load_command_duplicate_path() {
//   logger(4, "*** test_load_command_duplicate_path ***\n");
  
//   // Load first database
//   cli_cmd_t *cmd1 = create_test_command("load", "test.db", "testdb1", "L");
//   TEST_ASSERT_NOT_NULL(cmd1);
  
//   int64_t result1 = load_command(cmd1);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result1);
  
//   // Try to load database with same path
//   cli_cmd_t *cmd2 = create_test_command("load", "test.db", "testdb2", "L");
//   TEST_ASSERT_NOT_NULL(cmd2);
  
//   int64_t result2 = load_command(cmd2);
//   TEST_ASSERT_EQUAL(-1, result2);
  
//   free_cli_command(cmd1);
//   free_cli_command(cmd2);
// }

// static void test_load_command_failed_db_creation() {
//   logger(4, "*** test_load_command_failed_db_creation ***\n");
  
//   cli_cmd_t *cmd = create_test_command("load", "test.db", "testdb", "INVALID");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = load_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// // Test functions for reload_command
// static void test_reload_command_valid_inputs() {
//   logger(4, "*** test_reload_command_valid_inputs ***\n");
  
//   // First load a database
//   cli_cmd_t *load_cmd = create_test_command("load", "test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(load_cmd);
  
//   int64_t load_result = load_command(load_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, load_result);
  
//   // Now reload it
//   cli_cmd_t *reload_cmd = create_test_command("reload", "testdb", "H", NULL);
//   TEST_ASSERT_NOT_NULL(reload_cmd);
  
//   int64_t reload_result = reload_command(reload_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, reload_result);
  
//   free_cli_command(load_cmd);
//   free_cli_command(reload_cmd);
// }

// static void test_reload_command_null_cmd_ptr() {
//   logger(4, "*** test_reload_command_null_cmd_ptr ***\n");
  
//   int64_t result = reload_command(NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// static void test_reload_command_missing_param_1() {
//   logger(4, "*** test_reload_command_missing_param_1 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("reload", "", "L", NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = reload_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_reload_command_missing_param_2() {
//   logger(4, "*** test_reload_command_missing_param_2 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("reload", "testdb", "", NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = reload_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_reload_command_empty_params() {
//   logger(4, "*** test_reload_command_empty_params ***\n");
  
//   cli_cmd_t *cmd = create_test_command("reload", "", "", NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = reload_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_reload_command_nonexistent_id() {
//   logger(4, "*** test_reload_command_nonexistent_id ***\n");
  
//   cli_cmd_t *cmd = create_test_command("reload", "nonexistent", "L", NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = reload_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// // Test functions for list_command
// static void test_list_command_valid_no_databases() {
//   logger(4, "*** test_list_command_valid_no_databases ***\n");
  
//   cli_cmd_t *cmd = create_test_command("list", NULL, NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = list_command(cmd);
//   TEST_ASSERT_EQUAL(0, result);
  
//   free_cli_command(cmd);
// }

// static void test_list_command_valid_with_databases() {
//   logger(4, "*** test_list_command_valid_with_databases ***\n");
  
//   // Load a database first
//   cli_cmd_t *load_cmd = create_test_command("load", "test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(load_cmd);
  
//   int64_t load_result = load_command(load_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, load_result);
  
//   // Now list databases
//   cli_cmd_t *list_cmd = create_test_command("list", NULL, NULL, NULL);
//   TEST_ASSERT_NOT_NULL(list_cmd);
  
//   int64_t list_result = list_command(list_cmd);
//   TEST_ASSERT_EQUAL(0, list_result);
  
//   free_cli_command(load_cmd);
//   free_cli_command(list_cmd);
// }

// static void test_list_command_null_cmd_ptr() {
//   logger(4, "*** test_list_command_null_cmd_ptr ***\n");
  
//   int64_t result = list_command(NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// // Test functions for use_command
// static void test_use_command_valid_input() {
//   logger(4, "*** test_use_command_valid_input ***\n");
  
//   // First load a database
//   cli_cmd_t *load_cmd = create_test_command("load", "test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(load_cmd);
  
//   int64_t load_result = load_command(load_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, load_result);
  
//   // Note: We can't fully test use_command as it enters an interactive loop
//   // We can only test parameter validation. The actual use_command call would
//   // start an interactive session which is not suitable for unit testing.
  
//   cli_cmd_t *use_cmd = create_test_command("use", "testdb", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(use_cmd);
  
//   // We'll just verify the command was created properly since use_command
//   // would start an interactive loop
//   TEST_ASSERT_EQUAL_STRING("use", use_cmd->cmd);
//   TEST_ASSERT_EQUAL_STRING("testdb", use_cmd->param_1);
  
//   free_cli_command(load_cmd);
//   free_cli_command(use_cmd);
// }

// static void test_use_command_null_cmd_ptr() {
//   logger(4, "*** test_use_command_null_cmd_ptr ***\n");
  
//   int64_t result = use_command(NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// static void test_use_command_missing_param_1() {
//   logger(4, "*** test_use_command_missing_param_1 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("use", "", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = use_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_use_command_empty_param_1() {
//   logger(4, "*** test_use_command_empty_param_1 ***\n");
  
//   cli_cmd_t *cmd = create_test_command("use", "", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = use_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_use_command_nonexistent_id() {
//   logger(4, "*** test_use_command_nonexistent_id ***\n");
  
//   cli_cmd_t *cmd = create_test_command("use", "nonexistent", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = use_command(cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// // Test functions for help commands
// static void test_main_help_command() {
//   logger(4, "*** test_main_help_command ***\n");
  
//   int64_t result = main_help_command();
//   TEST_ASSERT_EQUAL(0, result);
// }

// static void test_use_help_command() {
//   logger(4, "*** test_use_help_command ***\n");
  
//   int64_t result = use_help_command();
//   TEST_ASSERT_EQUAL(0, result);
// }

// // Test functions for put_command
// static void test_put_command_valid_inputs() {
//   logger(4, "*** test_put_command_valid_inputs ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("put", "key1", "value1", "string");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = put_command(cli_db, cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_put_command_valid_inputs_without_type() {
//   logger(4, "*** test_put_command_valid_inputs_without_type ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("put", "key1", "value1", "");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = put_command(cli_db, cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_put_command_null_cli_db() {
//   logger(4, "*** test_put_command_null_cli_db ***\n");
  
//   cli_cmd_t *cmd = create_test_command("put", "key1", "value1", "string");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = put_command(NULL, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_put_command_null_cmd_ptr() {
//   logger(4, "*** test_put_command_null_cmd_ptr ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   int64_t result = put_command(cli_db, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
// }

// static void test_put_command_both_null() {
//   logger(4, "*** test_put_command_both_null ***\n");
  
//   int64_t result = put_command(NULL, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// static void test_put_command_missing_param_1() {
//   logger(4, "*** test_put_command_missing_param_1 ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("put", "", "value1", "string");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = put_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_put_command_missing_param_2() {
//   logger(4, "*** test_put_command_missing_param_2 ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("put", "key1", "", "string");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = put_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_put_command_empty_params() {
//   logger(4, "*** test_put_command_empty_params ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("put", "", "", "");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = put_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// // Test functions for get_command
// static void test_get_command_valid_input() {
//   logger(4, "*** test_get_command_valid_input ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   // First put an entry
//   cli_cmd_t *put_cmd = create_test_command("put", "key1", "value1", "string");
//   TEST_ASSERT_NOT_NULL(put_cmd);
  
//   int64_t put_result = put_command(cli_db, put_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_result);
  
//   // Now get the entry
//   cli_cmd_t *get_cmd = create_test_command("get", "key1", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(get_cmd);
  
//   int64_t get_result = get_command(cli_db, get_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, get_result);
  
//   free_cli_db(cli_db);
//   free_cli_command(put_cmd);
//   free_cli_command(get_cmd);
// }

// static void test_get_command_null_cli_db() {
//   logger(4, "*** test_get_command_null_cli_db ***\n");
  
//   cli_cmd_t *cmd = create_test_command("get", "key1", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = get_command(NULL, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_get_command_null_cmd_ptr() {
//   logger(4, "*** test_get_command_null_cmd_ptr ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   int64_t result = get_command(cli_db, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
// }

// static void test_get_command_both_null() {
//   logger(4, "*** test_get_command_both_null ***\n");
  
//   int64_t result = get_command(NULL, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// static void test_get_command_missing_param_1() {
//   logger(4, "*** test_get_command_missing_param_1 ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("get", "", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = get_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_get_command_empty_param_1() {
//   logger(4, "*** test_get_command_empty_param_1 ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("get", "", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = get_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_get_command_nonexistent_key() {
//   logger(4, "*** test_get_command_nonexistent_key ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("get", "nonexistent", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = get_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// // Test functions for delete_command
// static void test_delete_command_valid_input() {
//   logger(4, "*** test_delete_command_valid_input ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   // First put an entry
//   cli_cmd_t *put_cmd = create_test_command("put", "key1", "value1", "string");
//   TEST_ASSERT_NOT_NULL(put_cmd);
  
//   int64_t put_result = put_command(cli_db, put_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_result);
  
//   // Now delete the entry
//   cli_cmd_t *delete_cmd = create_test_command("delete", "key1", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(delete_cmd);
  
//   int64_t delete_result = delete_command(cli_db, delete_cmd);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, delete_result);
  
//   free_cli_db(cli_db);
//   free_cli_command(put_cmd);
//   free_cli_command(delete_cmd);
// }

// static void test_delete_command_null_cli_db() {
//   logger(4, "*** test_delete_command_null_cli_db ***\n");
  
//   cli_cmd_t *cmd = create_test_command("delete", "key1", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = delete_command(NULL, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_command(cmd);
// }

// static void test_delete_command_null_cmd_ptr() {
//   logger(4, "*** test_delete_command_null_cmd_ptr ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   int64_t result = delete_command(cli_db, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
// }

// static void test_delete_command_both_null() {
//   logger(4, "*** test_delete_command_both_null ***\n");
  
//   int64_t result = delete_command(NULL, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// static void test_delete_command_missing_param_1() {
//   logger(4, "*** test_delete_command_missing_param_1 ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("delete", "", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = delete_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_delete_command_empty_param_1() {
//   logger(4, "*** test_delete_command_empty_param_1 ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("delete", "", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = delete_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// static void test_delete_command_failed_delete() {
//   logger(4, "*** test_delete_command_failed_delete ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_cmd_t *cmd = create_test_command("delete", "nonexistent", NULL, NULL);
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   int64_t result = delete_command(cli_db, cmd);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_cli_db(cli_db);
//   free_cli_command(cmd);
// }

// // Test functions for print_command
// static void test_print_command_valid_input() {
//   logger(4, "*** test_print_command_valid_input ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   int64_t result = print_command(cli_db);
//   TEST_ASSERT_EQUAL(0, result);
  
//   free_cli_db(cli_db);
// }

// static void test_print_command_null_cli_db() {
//   logger(4, "*** test_print_command_null_cli_db ***\n");
  
//   int64_t result = print_command(NULL);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// // Test functions for free_cli_command
// static void test_free_cli_command_valid() {
//   logger(4, "*** test_free_cli_command_valid ***\n");
  
//   cli_cmd_t *cmd = create_test_command("test", "param1", "param2", "param3");
//   TEST_ASSERT_NOT_NULL(cmd);
  
//   // This should not crash
//   free_cli_command(cmd);
//   TEST_ASSERT_EQUAL(1, 1); // If we reach here, the function didn't crash
// }

// static void test_free_cli_command_null() {
//   logger(4, "*** test_free_cli_command_null ***\n");
  
//   // This should not crash
//   free_cli_command(NULL);
//   TEST_ASSERT_EQUAL(1, 1); // If we reach here, the function didn't crash
// }

// // Test functions for free_cli_db
// static void test_free_cli_db_valid() {
//   logger(4, "*** test_free_cli_db_valid ***\n");
  
//   cli_db_t *cli_db = create_test_cli_db("test.db", "testdb", "L");
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   // This should not crash
//   free_cli_db(cli_db);
//   TEST_ASSERT_EQUAL(1, 1); // If we reach here, the function didn't crash
// }

// static void test_free_cli_db_null() {
//   logger(4, "*** test_free_cli_db_null ***\n");
  
//   // This should not crash
//   free_cli_db(NULL);
//   TEST_ASSERT_EQUAL(1, 1); // If we reach here, the function didn't crash
// }

// static void test_free_cli_db_null_db() {
//   logger(4, "*** test_free_cli_db_null_db ***\n");
  
//   cli_db_t *cli_db = malloc(sizeof(cli_db_t));
//   TEST_ASSERT_NOT_NULL(cli_db);
  
//   cli_db->db = NULL;
//   strncpy(cli_db->path, "test.db", BG_BUFFER_SIZE - 1);
//   strncpy(cli_db->id, "testdb", SM_BUFFER_SIZE - 1);
  
//   // This should not crash
//   free_cli_db(cli_db);
//   TEST_ASSERT_EQUAL(1, 1); // If we reach here, the function didn't crash
// }

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
  
  // // load_command tests
  // RUN_TEST(test_load_command_valid_inputs);
  // RUN_TEST(test_load_command_null_cmd_ptr);
  // RUN_TEST(test_load_command_missing_param_1);
  // RUN_TEST(test_load_command_missing_param_2);
  // RUN_TEST(test_load_command_missing_param_3);
  // RUN_TEST(test_load_command_empty_params);
  // RUN_TEST(test_load_command_max_databases_reached);
  // RUN_TEST(test_load_command_duplicate_id);
  // RUN_TEST(test_load_command_duplicate_path);
  // RUN_TEST(test_load_command_failed_db_creation);
  
  // // reload_command tests
  // RUN_TEST(test_reload_command_valid_inputs);
  // RUN_TEST(test_reload_command_null_cmd_ptr);
  // RUN_TEST(test_reload_command_missing_param_1);
  // RUN_TEST(test_reload_command_missing_param_2);
  // RUN_TEST(test_reload_command_empty_params);
  // RUN_TEST(test_reload_command_nonexistent_id);
  
  // // list_command tests
  // RUN_TEST(test_list_command_valid_no_databases);
  // RUN_TEST(test_list_command_valid_with_databases);
  // RUN_TEST(test_list_command_null_cmd_ptr);
  
  // // use_command tests
  // RUN_TEST(test_use_command_valid_input);
  // RUN_TEST(test_use_command_null_cmd_ptr);
  // RUN_TEST(test_use_command_missing_param_1);
  // RUN_TEST(test_use_command_empty_param_1);
  // RUN_TEST(test_use_command_nonexistent_id);
  
  // // help command tests
  // RUN_TEST(test_main_help_command);
  // RUN_TEST(test_use_help_command);
  
  // // put_command tests
  // RUN_TEST(test_put_command_valid_inputs);
  // RUN_TEST(test_put_command_valid_inputs_without_type);
  // RUN_TEST(test_put_command_null_cli_db);
  // RUN_TEST(test_put_command_null_cmd_ptr);
  // RUN_TEST(test_put_command_both_null);
  // RUN_TEST(test_put_command_missing_param_1);
  // RUN_TEST(test_put_command_missing_param_2);
  // RUN_TEST(test_put_command_empty_params);
  
  // // get_command tests
  // RUN_TEST(test_get_command_valid_input);
  // RUN_TEST(test_get_command_null_cli_db);
  // RUN_TEST(test_get_command_null_cmd_ptr);
  // RUN_TEST(test_get_command_both_null);
  // RUN_TEST(test_get_command_missing_param_1);
  // RUN_TEST(test_get_command_empty_param_1);
  // RUN_TEST(test_get_command_nonexistent_key);
  
  // // delete_command tests
  // RUN_TEST(test_delete_command_valid_input);
  // RUN_TEST(test_delete_command_null_cli_db);
  // RUN_TEST(test_delete_command_null_cmd_ptr);
  // RUN_TEST(test_delete_command_both_null);
  // RUN_TEST(test_delete_command_missing_param_1);
  // RUN_TEST(test_delete_command_empty_param_1);
  // RUN_TEST(test_delete_command_failed_delete);
  
  // // print_command tests
  // RUN_TEST(test_print_command_valid_input);
  // RUN_TEST(test_print_command_null_cli_db);
  
  // // free_cli_command tests
  // RUN_TEST(test_free_cli_command_valid);
  // RUN_TEST(test_free_cli_command_null);
  
  // // free_cli_db tests
  // RUN_TEST(test_free_cli_db_valid);
  // RUN_TEST(test_free_cli_db_null);
  // RUN_TEST(test_free_cli_db_null_db);
  
  return UNITY_END();
}
