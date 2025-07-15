#include "kv_cli.h"

static void compact_db_list() {
  uint64_t target_idx = 0;
  for (uint64_t idx = 0; idx < KV_CLI_MAX_OPEN_DATABASES; idx++) {
    cli_db_t *tmp = db_list[idx];
    if (tmp != NULL) {
      db_list[idx] = db_list[target_idx];
      db_list[target_idx] = tmp;
      target_idx++;
    }
  }
}


extern cli_db_t **get_db_list() {
  return db_list;
}

extern uint64_t get_db_count() {
  return db_count;
}

extern int64_t get_command_from_user(uint8_t *msg, uint8_t *dest, uint64_t max_len) {
  uint8_t input_cmd[max_len];
  if (msg != NULL) {
    logger(4, msg);
  }
  logger(4, "> ");
  if (fgets(input_cmd, max_len, stdin) == NULL) {
    logger(3, "Error: Failed to get input from user\n");
    return -1;
  }
  input_cmd[strcspn(input_cmd, "\n")] = '\0';
  strncpy(dest, input_cmd, max_len);
  return 0;
}

extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type) {
  if (path == NULL || id == NULL || storage_type == NULL) {
    logger(3, "Error: NULL pointer passed to create_cli_db\n");
    return NULL;
  }
  
  if (strlen(path) == 0 || strlen(id) == 0 || strlen(storage_type) == 0) {
    logger(3, "Error: Empty string passed to create_cli_db\n");
    return NULL;
  }
  
  cli_db_t *cli_db = malloc(sizeof(cli_db_t));
  cli_db->db = create_db(storage_type);
  if (cli_db->db == NULL) {
    logger(3, "Error: Failed to create new db\n");
    free_cli_db(cli_db);
    return NULL;
  }
  strncpy(cli_db->path, path, BG_BUFFER_SIZE);
  strncpy(cli_db->id, id, SM_BUFFER_SIZE);
  return cli_db;
}

extern cli_cmd_t *create_command(uint8_t *command) {
  if (command == NULL) {
    logger(3, "Error: NULL pointer passed to create_command\n");
    return NULL;
  }
  
  cli_cmd_t *cmd_ptr = malloc(sizeof(cli_cmd_t));
  if (cmd_ptr == NULL) {
    logger(3, "Error: Failed to allocate command memory\n");
    return NULL;
  }
  memset(cmd_ptr, 0, sizeof(cli_cmd_t));

  uint8_t *cmd, *param_1, *param_2, *param_3;
  cmd = strtok(command, " ");
  param_1 = strtok(NULL, " ");
  param_2 = strtok(NULL, " ");
  param_3 = strtok(NULL, "\n");

  if (cmd == NULL) {
    logger(3, "Error: Invalid command %s\n", command);
    free_cli_command(cmd_ptr);
    return NULL;
  }
  
  if (strlen(cmd) > 0){
    strncpy(cmd_ptr->cmd, cmd, SM_BUFFER_SIZE);
    cmd_ptr->cmd[SM_BUFFER_SIZE - 1] = '\0';
  }

  if (param_1 != NULL && strlen(param_1) > 0) {
    strncpy(cmd_ptr->param_1, param_1, SM_BUFFER_SIZE);
    cmd_ptr->param_1[SM_BUFFER_SIZE - 1] = '\0';
  }

  if (param_2 != NULL && strlen(param_2) > 0) {
    strncpy(cmd_ptr->param_2, param_2, SM_BUFFER_SIZE);
    cmd_ptr->param_2[SM_BUFFER_SIZE - 1] = '\0';
  }

  if (param_3 != NULL && strlen(param_3) > 0) {
    strncpy(cmd_ptr->param_3, param_3, SM_BUFFER_SIZE);
    cmd_ptr->param_3[SM_BUFFER_SIZE - 1] = '\0';
  }
  
  return cmd_ptr;
}

extern int64_t start_use(uint64_t db_idx) {
  cli_db_t *cli_db = db_list[db_idx];
  cli_cmd_t *cmd_ptr;
  uint8_t command[BG_BUFFER_SIZE];
  uint8_t msg[BG_BUFFER_SIZE];
  snprintf(msg, BG_BUFFER_SIZE, "(%s) - Please enter a command (or help)\n", cli_db->id);
  
  int64_t cmd_result = -1;
  while (true) {
    logger(4, "-----------------------------------------------\n");
    get_command_from_user(msg, command, BG_BUFFER_SIZE);
    cmd_ptr = create_command(command);
    if (cmd_ptr ==  NULL) {
      logger(3, "Failed to allocated memory for a command\n");
      cmd_result = -1;
      continue;
    }

    if (strcmp(cmd_ptr->cmd, CLI_COMMAND_PUT) == 0) {
      cmd_result = put_command(cli_db, cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_GET) == 0) {
      cmd_result = get_command(cli_db, cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_DELETE) == 0) {
      cmd_result = delete_command(cli_db, cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_HELP) == 0) {
      cmd_result = use_help_command();
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_PRINT) == 0) {
      cmd_result = print_command(cli_db);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_EXIT) == 0) {
      free_cli_command(cmd_ptr);
      return 0;
    }
    else {
      logger(4, "Error: Invalid command provided %s\n", cmd_ptr->cmd);
      cmd_result = -1;
    }

    if (cmd_result < 0) {
      logger(3, "Error: Failed to execute command\n");
    }
    else {
      save_db(cli_db->db, cli_db->path);
    }

    free_cli_command(cmd_ptr);
  }
}

extern int64_t load_command(cli_cmd_t *cmd_ptr) {
  if (cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to load_command\n");
    return -1;
  }
  
  if (db_count >= KV_CLI_MAX_OPEN_DATABASES) {
    logger(4, "Reached max number of open databases. Nothing done\n");
    return -1;
  }

  if (strlen(cmd_ptr->param_1) <= 0 ||
      strlen(cmd_ptr->param_2) <= 0 ||
      strlen(cmd_ptr->param_3) <= 0) {
    logger(4, "\"load\" requires three parameters: load <db_path> <db_id> <storage_type>\n");
    return -1;
  }

  uint8_t *path = cmd_ptr->param_1;
  uint8_t *id = cmd_ptr->param_2;
  uint8_t *storage_type = cmd_ptr->param_3;

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    if (strcmp(cli_db->id, id) == 0) {
      logger(4, "Error: Database with id \"%s\" already exists\n", cli_db->id);
      return -1;
    }
  
    if (strcmp(cli_db->path, path) == 0) {
      logger(4, "Error: Database with path \"%s\" already exists\n", cli_db->path);
      return -1;
    }
  }

  logger(4, "- Loading database from path: %s as \"%s\"\n ", path, id);

  cli_db_t *cli_db = create_cli_db(path, id, storage_type);
  if (cli_db == NULL) {
    logger(3, "Error: Failed to create db instance\n");
    return -1;
  }

  if (load_db(cli_db->db, cli_db->path) < 0) {
    logger(3, "Error: Failed to load db into memory\n");
    free_cli_db(cli_db);
    return -1;
  }
  
  db_list[db_count++] = cli_db;

  return 0;
}

extern int64_t reload_command(cli_cmd_t *cmd_ptr) {
  if (cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to reload_command\n");
    return -1;
  }
  
  if (strlen(cmd_ptr->param_1) <= 0 ||
      strlen(cmd_ptr->param_2) <= 0) {
    logger(4, "\"reload\" requires two parameters: reload <db_id> <storage_type>\n");
    return -1;
  }

  uint8_t *id = cmd_ptr->param_1;
  uint8_t *storage_type = cmd_ptr->param_2;

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    
    if (strcmp(cli_db->id, id) != 0) continue;
    
    logger(4, "- Reloading database from path: %s as \"%s\"\n ", cli_db->path, storage_type);

    cli_db_t *new_cli_db = create_cli_db(cli_db->path, id, storage_type);
    if (new_cli_db == NULL) {
      logger(3, "Error: Failed to create db instance\n");
      return -1;
    }

    if (load_db(new_cli_db->db, new_cli_db->path) < 0) {
      logger(3, "Error: Failed to load db into memory\n");
      free_cli_db(new_cli_db);
      return -1;
    }

    db_list[i] = new_cli_db;
    free_cli_db(cli_db);
    return 0;
  }
}

extern int64_t close_command(cli_cmd_t *cmd_ptr) {
  if (cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to close_command\n");
    return -1;
  }
  
  if (strlen(cmd_ptr->param_1) <= 0) {
    logger(4, "\"close\" requires two parameters: close <db_id>\n");
    return -1;
  }

  uint8_t *id = cmd_ptr->param_1;

  for (uint64_t idx = 0; idx < db_count; idx++) {
    cli_db_t *current_db = db_list[idx];
    if (strcmp(current_db->id, id) == 0) {
      free_cli_db(current_db);
      db_list[idx] = NULL;
      db_count--;
      compact_db_list();
      return 0;
    }
  }
  
  logger(3, "Error: Failed to find a database with the given id\n");
  return -1;
}

extern int64_t list_command(cli_cmd_t *cmd_ptr) {
  if (cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to list_command\n");
    return -1;
  }
  
  if (db_count == 0) {
    logger(4, "- No databases loaded\n");
    return 0;
  }

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    logger(4, "\t%s\t%s\n", cli_db->id, cli_db->path);
  }
  return 0;
}

extern int64_t use_command(cli_cmd_t *cmd_ptr) {
  if (cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to use_command\n");
    return -1;
  }
  
  if (strlen(cmd_ptr->param_1) <= 0) {
    logger(4, "\"use\" requires one parameter: use <db_id>\n");
    return -1;
  }

  int64_t db_idx = -1;
  for (int64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    if (strcmp(cli_db->id, cmd_ptr->param_1) == 0) {
      db_idx = i;
      break;
    }
  }

  if (db_idx < 0) {
    logger(4, "Error: database id %s does not exist\n", cmd_ptr->param_1);
    return -1;
  }

  return start_use(db_idx);
}

extern int64_t put_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr) {
  if (cli_db == NULL || cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to put_command\n");
    return -1;
  }
  
  if (strlen(cmd_ptr->param_1) <= 0 ||
      strlen(cmd_ptr->param_2) <= 0) {
    logger(4, "\"put\" requires two or three parameters: use <key> <value> <data_type (optional)>\n");
    return -1;
  }

  uint8_t *key = cmd_ptr->param_1;
  uint8_t *value = cmd_ptr->param_2;
  uint8_t *type = cmd_ptr->param_3;

  if (put_entry(cli_db->db, key, value, type) < 0) {
    logger(4, "Error: Failed to put entry with key \"%s\"\n", key);
    return -1;
  }

  return 0;
}

extern int64_t get_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr) {
  if (cli_db == NULL || cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to get_command\n");
    return -1;
  }
  
  if (strlen(cmd_ptr->param_1) <= 0) {
    logger(4, "Error: \"get\" requires one parameter: get <key>\n");
    return -1;
  }

  uint8_t *key = cmd_ptr->param_1;
  db_entry_t *entry = get_entry(cli_db->db, key);
  if (entry ==  NULL) {
    logger(4, "Error: Failed to get entry with key: \"%s\"\n", key);
    return -1;
  }

  print_entry(entry);

  return 0;
}

extern int64_t delete_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr) {
  if (cli_db == NULL || cmd_ptr == NULL) {
    logger(3, "Error: NULL pointer passed to delete_command\n");
    return -1;
  }
  
  if (strlen(cmd_ptr->param_1) <= 0) {
    logger(4, "Error: \"delete\" requires one parameter: delete <key>\n");
    return -1;
  }

  uint8_t *key = cmd_ptr->param_1;
  if (delete_entry(cli_db->db, key) < 0) {
    logger(4, "Error: Failed to delete entry with key: \"%s\"\n", key);
    return -1;
  }

  return 0;
}

extern int64_t print_command(cli_db_t *cli_db) {
  if (cli_db == NULL) {
    logger(3, "Error: NULL pointer passed to print_command\n");
    return -1;
  }
  
  print_db(cli_db->db);
  return 0;
}

extern int64_t main_help_command() {
  logger(4, "List of available commands:\n");

  logger(4, "\t* load <db_path> <db_id> <storage_type>: Loads a database into memory\n");
  logger(4, "\t\t<db_path>: path to file containing database\n");
  logger(4, "\t\t<db_id>: alias for the database\n");
  logger(4, "\t\t<storage_type>: data structure to use for storage\n");
  logger(4, "\t\t\tL: linked list\n");
  logger(4, "\t\t\tH: Hash Table\n");

  logger(4, "\t* reload <db_id> <storage_type>: Reloads a database with a different storage type\n");
  logger(4, "\t\t<db_id>: alias of the database to reload\n");
  logger(4, "\t\t<storage_type>: new data structure to use for storage\n");
  logger(4, "\t\t\tL: linked list\n");
  logger(4, "\t\t\tH: Hash Table\n");

  logger(4, "\t* close <db_id>: Closes a database and removes it from memory\n");
  logger(4, "\t\t<db_id>: alias of the database to close\n");

  logger(4, "\t* use <db_id>: Selects a loaded database to apply operations to\n");
  logger(4, "\t\t<db_id>: alias of the loaded database\n");

  logger(4, "\t* list: Lists loaded databases\n");
  
  logger(4, "\t* exit: Exists the application. Closes all databases\n");

  return 0;
}

extern int64_t use_help_command() {
  logger(4, "List of available commands:\n");

  logger(4, "\t* put <key> <value> <type>: Creates or updates an entry in the database\n");
  logger(4, "\t\t<key>: Key that identifies the entry\n");
  logger(4, "\t\t<value>: Value held by the entry\n");
  logger(4, "\t\t<type>: Type of the value held by the entry\n");
  logger(4, "\t\t\tint8\n");
  logger(4, "\t\t\tint16\n");
  logger(4, "\t\t\tint32\n");
  logger(4, "\t\t\tint64\n");
  logger(4, "\t\t\tfloat\n");
  logger(4, "\t\t\tdouble\n");
  logger(4, "\t\t\tbool\n");

  logger(4, "\t* get <key>: Prints the value of the entry with the given key\n");
  logger(4, "\t\t<key>: Key that identifies the entry\n");
  
  logger(4, "\t* delete <key>: Deletes an entry with the given key\n");
  logger(4, "\t\t<key>: Key that identifies the entry\n");
  
  logger(4, "\t* print: Prints all entries in the current database\n");
  
  logger(4, "\t* exit: Exists to the main menu. Stops operating on the current database\n");

  return 0;
}

extern void start_cli() {
  int64_t cmd_result = -1;
  while (true) {
    logger(4, "===============================================\n");
    cli_cmd_t *cmd_ptr;
    uint8_t command[BG_BUFFER_SIZE];

    get_command_from_user("- Please enter a command (or help)\n", command, BG_BUFFER_SIZE);
    cmd_ptr = create_command(command);
    if (cmd_ptr ==  NULL) {
      logger(3, "Failed to allocated memory for a command\n");
      cmd_result = -1;
      continue;
    }

    if (strcmp(cmd_ptr->cmd, CLI_COMMAND_LOAD) == 0) {
      cmd_result = load_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_RELOAD) == 0) {
      cmd_result = reload_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_CLOSE) == 0) {
      cmd_result = close_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_LIST) == 0) {
      cmd_result = list_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_USE) == 0) {
      cmd_result = use_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_HELP) == 0) {
      cmd_result = main_help_command();
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_EXIT) == 0) {
      free_cli_command(cmd_ptr);
      return;
    }
    else {
      logger(4, "Error: Invalided command provided %s\n", cmd_ptr->cmd);
      cmd_result = -1;
    }

    if (cmd_result < 0) {
      logger(3, "Error: Failed to execute command\n");
    }
  
    free_cli_command(cmd_ptr);
  }
}

extern void free_cli_command(cli_cmd_t *cmd_ptr) {
  if (cmd_ptr == NULL) return;
  free(cmd_ptr);
}

extern void free_cli_db(cli_db_t* cli_db) {
  if (cli_db == NULL) return;

  if (cli_db->db != NULL) {
    free_db(cli_db->db);
  }
  free(cli_db);
}

extern void free_db_list() {
  while(db_count > 0) {
    cli_db_t *cli_db = db_list[db_count-1];
    if (cli_db != NULL) {
      free_cli_db(cli_db);
    }
    db_count--;
  }
}