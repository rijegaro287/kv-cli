#include "test_kv_cli.h"

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
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_LOAD, "./test/test.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("./test/test.db", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_3);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_RELOAD, "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_RELOAD, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, cmd->param_2);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_LIST, cmd->cmd);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_USE, "db1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_USE, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("db1", cmd->param_1);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_HELP);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_HELP, cmd->cmd);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_EXIT);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_EXIT, cmd->cmd);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_PUT, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("key1", cmd->param_1);
  TEST_ASSERT_EQUAL_STRING("value1", cmd->param_2);
  TEST_ASSERT_EQUAL_STRING(STR_TYPE_STR, cmd->param_3);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_GET, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("key1", cmd->param_1);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_DELETE, cmd->cmd);
  TEST_ASSERT_EQUAL_STRING("key1", cmd->param_1);
  free_cli_command(cmd);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_PRINT);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL_STRING(CLI_COMMAND_PRINT, cmd->cmd);
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
  cli_db_t *loaded_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];
 
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_GREATER_THAN(0, get_db_count());
  loaded_db = get_db_list()[get_db_count()-1];
  TEST_ASSERT_EQUAL_STRING("./test/data/test_1.db", loaded_db->path);
  TEST_ASSERT_EQUAL_STRING("db1", loaded_db->id);
  TEST_ASSERT_NOT_NULL(loaded_db->db);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, loaded_db->db->storage_type);
  TEST_ASSERT_NOT_NULL(loaded_db->db->storage);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_HASH);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_GREATER_THAN(0, get_db_count());
  loaded_db = get_db_list()[get_db_count()-1];
  TEST_ASSERT_EQUAL_STRING("./test/data/test_2.db", loaded_db->path);
  TEST_ASSERT_EQUAL_STRING("db2", loaded_db->id);
  TEST_ASSERT_NOT_NULL(loaded_db->db);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_HASH, loaded_db->db->storage_type);
  TEST_ASSERT_NOT_NULL(loaded_db->db->storage);

  free_cli_command(cmd);
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
}

static void test_load_command_max_databases_reached() {
  logger(4, "*** test_load_command_max_databases_reached ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_1.db", "db1", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(1, get_db_count());

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_2.db", "db2", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_command(cmd));
  TEST_ASSERT_EQUAL(2, get_db_count());
  
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
                    CLI_COMMAND_LOAD, "./test/data/test_3.db", "db3", KV_STORAGE_STRUCTURE_LIST);
  cmd = create_command(command);
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
  logger(4, "*** test_list_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, list_command(NULL));
}

static void test_use_command_null_inputs() {
  logger(4, "*** test_use_command_null_inputs ***\n");
  TEST_ASSERT_EQUAL(-1, use_command(NULL));
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

// put_command tests
static void test_put_command_valid_inputs() {
  logger(4, "*** test_put_command_valid_inputs ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_put_command_valid_inputs_without_type() {
  logger(4, "*** test_put_command_valid_inputs_without_type ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_put_command_null_cli_db() {
  logger(4, "*** test_put_command_null_cli_db ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(NULL, cmd));
  free_cli_command(cmd);
}

static void test_put_command_null_cmd_ptr() {
  logger(4, "*** test_put_command_null_cmd_ptr ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, NULL));
  free_cli_db(cli_db);
}

static void test_put_command_both_null() {
  logger(4, "*** test_put_command_both_null ***\n");
  TEST_ASSERT_EQUAL(-1, put_command(NULL, NULL));
}

static void test_put_command_missing_param_1() {
  logger(4, "*** test_put_command_missing_param_1 ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s", 
           CLI_COMMAND_PUT, "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_put_command_missing_param_2() {
  logger(4, "*** test_put_command_missing_param_2 ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_PUT, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_put_command_empty_params() {
  logger(4, "*** test_put_command_empty_params ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "", "", "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, put_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

// get_command tests
static void test_get_command_valid_input() {
  logger(4, "*** test_get_command_valid_input ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *put_cmd, *get_cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  // First put a value
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  put_cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(put_cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_command(cli_db, put_cmd));
  free_cli_command(put_cmd);

  // Now get it
  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "key1");
  get_cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(get_cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, get_command(cli_db, get_cmd));
  
  free_cli_command(get_cmd);
  free_cli_db(cli_db);
}

static void test_get_command_null_cli_db() {
  logger(4, "*** test_get_command_null_cli_db ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(NULL, cmd));
  free_cli_command(cmd);
}

static void test_get_command_null_cmd_ptr() {
  logger(4, "*** test_get_command_null_cmd_ptr ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, NULL));
  free_cli_db(cli_db);
}

static void test_get_command_both_null() {
  logger(4, "*** test_get_command_both_null ***\n");
  TEST_ASSERT_EQUAL(-1, get_command(NULL, NULL));
}

static void test_get_command_missing_param_1() {
  logger(4, "*** test_get_command_missing_param_1 ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_GET);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_get_command_empty_param_1() {
  logger(4, "*** test_get_command_empty_param_1 ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_get_command_nonexistent_key() {
  logger(4, "*** test_get_command_nonexistent_key ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_GET, "nonexistent");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, get_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

// delete_command tests
static void test_delete_command_valid_input() {
  logger(4, "*** test_delete_command_valid_input ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *put_cmd, *delete_cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  // First put a value
  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  put_cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(put_cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_command(cli_db, put_cmd));
  free_cli_command(put_cmd);

  // Now delete it
  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "key1");
  delete_cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(delete_cmd);
  TEST_ASSERT_GREATER_OR_EQUAL(0, delete_command(cli_db, delete_cmd));
  
  free_cli_command(delete_cmd);
  free_cli_db(cli_db);
}

static void test_delete_command_null_cli_db() {
  logger(4, "*** test_delete_command_null_cli_db ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "key1");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(NULL, cmd));
  free_cli_command(cmd);
}

static void test_delete_command_null_cmd_ptr() {
  logger(4, "*** test_delete_command_null_cmd_ptr ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, NULL));
  free_cli_db(cli_db);
}

static void test_delete_command_both_null() {
  logger(4, "*** test_delete_command_both_null ***\n");
  TEST_ASSERT_EQUAL(-1, delete_command(NULL, NULL));
}

static void test_delete_command_missing_param_1() {
  logger(4, "*** test_delete_command_missing_param_1 ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s", CLI_COMMAND_DELETE);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_delete_command_empty_param_1() {
  logger(4, "*** test_delete_command_empty_param_1 ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

static void test_delete_command_failed_delete() {
  logger(4, "*** test_delete_command_failed_delete ***\n");
  cli_db_t *cli_db;
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);

  // Try to delete a non-existent key
  snprintf(command, BG_BUFFER_SIZE, "%s %s", CLI_COMMAND_DELETE, "nonexistent");
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  TEST_ASSERT_EQUAL(-1, delete_command(cli_db, cmd));
  
  free_cli_command(cmd);
  free_cli_db(cli_db);
}

// print_command tests
static void test_print_command_valid_input() {
  logger(4, "*** test_print_command_valid_input ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);
  TEST_ASSERT_GREATER_OR_EQUAL(0, print_command(cli_db));
  free_cli_db(cli_db);
}

static void test_print_command_null_cli_db() {
  logger(4, "*** test_print_command_null_cli_db ***\n");
  TEST_ASSERT_EQUAL(-1, print_command(NULL));
}

// help command tests
static void test_main_help_command() {
  logger(4, "*** test_main_help_command ***\n");
  TEST_ASSERT_GREATER_OR_EQUAL(0, main_help_command());
}

static void test_use_help_command() {
  logger(4, "*** test_use_help_command ***\n");
  TEST_ASSERT_GREATER_OR_EQUAL(0, use_help_command());
}


// free_cli_command tests
static void test_free_cli_command_valid() {
  logger(4, "*** test_free_cli_command_valid ***\n");
  cli_cmd_t *cmd;
  uint8_t command[BG_BUFFER_SIZE];

  snprintf(command, BG_BUFFER_SIZE, "%s %s %s %s", 
           CLI_COMMAND_PUT, "key1", "value1", STR_TYPE_STR);
  cmd = create_command(command);
  TEST_ASSERT_NOT_NULL(cmd);
  
  // This should not crash
  free_cli_command(cmd);
}

static void test_free_cli_command_null() {
  logger(4, "*** test_free_cli_command_null ***\n");
  // This should not crash
  free_cli_command(NULL);
}

// free_cli_db tests
static void test_free_cli_db_valid() {
  logger(4, "*** test_free_cli_db_valid ***\n");
  cli_db_t *cli_db;

  cli_db = create_cli_db("test.db", "testdb", KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(cli_db);
  
  // This should not crash
  free_cli_db(cli_db);
}

static void test_free_cli_db_null() {
  logger(4, "*** test_free_cli_db_null ***\n");
  // This should not crash
  free_cli_db(NULL);
}

static void test_free_cli_db_null_db() {
  logger(4, "*** test_free_cli_db_null_db ***\n");
  cli_db_t *cli_db;

  cli_db = malloc(sizeof(cli_db_t));
  TEST_ASSERT_NOT_NULL(cli_db);
  cli_db->db = NULL;
  strcpy(cli_db->path, "test.db");
  strcpy(cli_db->id, "testdb");
  
  // This should not crash
  free_cli_db(cli_db);
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
  
  // // use_command tests
  RUN_TEST(test_use_command_null_inputs);
  RUN_TEST(test_use_command_invalid_inputs);
  
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
  
  // // help command tests
  // RUN_TEST(test_main_help_command);
  // RUN_TEST(test_use_help_command);

  // // free_cli_command tests
  // RUN_TEST(test_free_cli_command_valid);
  // RUN_TEST(test_free_cli_command_null);
  
  // // free_cli_db tests
  // RUN_TEST(test_free_cli_db_valid);
  // RUN_TEST(test_free_cli_db_null);
  // RUN_TEST(test_free_cli_db_null_db);
  
  return UNITY_END();
}
