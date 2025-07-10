#include "kv_cli.h"

extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type) {
  cli_db_t *cli_db = malloc(sizeof(cli_db_t));
  cli_db->db = create_db(storage_type);
  if (cli_db->db == NULL) {
    logger(3, "Error: Failed to create new db\n");
    free_cli_db(cli_db);
    return NULL;
  }
  strncpy(cli_db->path, path, LINE_BUFFER_SIZE);
  strncpy(cli_db->id, id, CLI_CMD_BUFFER_SIZE);
  return cli_db;
}

extern void start_cli() {
  int64_t cmd_result = -1;
  while (true) {
    logger(4, "===============================================\n");
    cli_cmd_t *cmd_ptr = get_cmd("- Please enter a command (or help)\n");
    if (cmd_ptr ==  NULL) {
      logger(3, "Failed to allocated memory for a command\n");
      cmd_result = -1;
      continue;
    }

    if (strcmp(cmd_ptr->cmd, CLI_COMMAND_LOAD) == 0) {
      cmd_result = load_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_LIST) == 0) {
      cmd_result = list_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_USE) == 0) {
      cmd_result = use_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_COMMAND_HELP) == 0) {
      cmd_result = 0;
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

static int64_t start_use(uint64_t db_idx) {
  int64_t cmd_result = -1;
  cli_db_t *cli_db = db_list[db_idx];
  uint8_t msg[LINE_BUFFER_SIZE];
  while (true) {
    logger(4, "-----------------------------------------------\n");
    logger(4, "(%s) - Please enter a command (or help)\n", cli_db->id);
    cli_cmd_t *cmd_ptr = get_cmd(msg);
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

    free_cli_command(cmd_ptr);
  }
}

static cli_cmd_t* get_cmd(uint8_t *msg) {
  uint8_t input_cmd[LINE_BUFFER_SIZE];
  if (msg != NULL) {
    logger(4, msg);
  }
  logger(4, "> ");
  if (fgets(input_cmd, LINE_BUFFER_SIZE, stdin) == NULL) {
    logger(3, "Error: Failed to get input from user\n");
    return NULL;
  }

  input_cmd[strcspn(input_cmd, "\n")] = '\0';
  
  cli_cmd_t *cmd_ptr = malloc(sizeof(cli_cmd_t));
  if (cmd_ptr == NULL) {
    logger(3, "Error: Failed to allocate command memory\n");
    return NULL;
  }
    memset(cmd_ptr, 0, sizeof(cli_cmd_t));

  uint8_t *cmd, *param_1, *param_2, *param_3;
  cmd = strtok(input_cmd, " ");
  param_1 = strtok(NULL, " ");
  param_2 = strtok(NULL, " ");
  param_3 = strtok(NULL, "\n");

  if (cmd == NULL) {
    logger(3, "Error: Invalid command %s\n", input_cmd);
    free_cli_command(cmd_ptr);
    return NULL;
  }
  else {
    strncpy(cmd_ptr->cmd, cmd, CLI_CMD_BUFFER_SIZE);
    cmd_ptr->cmd[CLI_CMD_BUFFER_SIZE - 1] = '\0';
  }

  if (param_1 != NULL) {
    strncpy(cmd_ptr->param_1, param_1, CLI_CMD_BUFFER_SIZE);
    cmd_ptr->param_1[CLI_CMD_BUFFER_SIZE - 1] = '\0';
  }

  if (param_2 != NULL) {
    strncpy(cmd_ptr->param_2, param_2, CLI_CMD_BUFFER_SIZE);
    cmd_ptr->param_2[CLI_CMD_BUFFER_SIZE - 1] = '\0';
  }

  if (param_3 != NULL) {
    strncpy(cmd_ptr->param_3, param_3, CLI_CMD_BUFFER_SIZE);
    cmd_ptr->param_3[CLI_CMD_BUFFER_SIZE - 1] = '\0';
  }
  
  return cmd_ptr;
}

static int64_t load_command(cli_cmd_t *cmd_ptr) {
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
  
  db_list[db_count] = cli_db;
  db_count++;

  return 0;
}

static int64_t list_command(cli_cmd_t *cmd_ptr) {
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

static int64_t use_command(cli_cmd_t *cmd_ptr) {
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

static int64_t put_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr) {
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

static int64_t get_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr) {
  if (strlen(cmd_ptr->param_1) <= 0) {
    logger(4, "Error: \"get\" requires one parameter: get <key>\n");
    return -1;
  }

  uint8_t *key = cmd_ptr->param_1;
  db_entry_t *entry = get_entry_by_key(cli_db->db, key);
  if (entry ==  NULL) {
    logger(4, "Error: Failed to get entry with key: \"%s\"\n", key);
    return -1;
  }

  print_entry(entry);

  return 0;
}

static int64_t delete_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr) {
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

static int64_t print_command(cli_db_t *cli_db) {
  print_db(cli_db->db);
  return 0;
}
