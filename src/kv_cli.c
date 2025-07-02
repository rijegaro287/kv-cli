#include "kv_cli.h"

extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type) {
  cli_db_t *cli_db = malloc(sizeof(cli_db_t));
  cli_db->db = create_db(storage_type);
  if (cli_db->db == NULL) {
    perror("Error: Failed to create new db\n");
    free_cli_db(cli_db);
    return NULL;
  }
  strcpy(cli_db->path, path);
  strcpy(cli_db->id, id);
  return cli_db;
}

extern void start_cli() {
  while (true) {
    printf("===============================================\n");
    cli_cmd_t *cmd = get_cmd("- Please enter a command (or help)\n");
    int64_t cmd_result;
    if (strcmp(cmd->cmd, CLI_COMMAND_LOAD) == 0) {
      cmd_result = load_command(cmd);
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_USE) == 0) {
      cmd_result = use_command(cmd);
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_LIST) == 0) {
      cmd_result = list_command(cmd);
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_HELP) == 0) {
      cmd_result = 0;
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_EXIT) == 0) {
      return;
    }
    else {
      fprintf(stderr, "Error: Invalid cmd entered %s\n", cmd->cmd);
      cmd_result = -1;
    }

    if (cmd_result < 0) {
      perror("Error: Failed to execute cmd\n");
    }
  }
}

static int64_t start_use(uint64_t db_idx) {
  cli_db_t *cli_db = db_list[db_idx];
  while (true) {
    printf("-----------------------------------------------\n");
    uint8_t msg[CLI_CMD_BUFFER_SIZE];
    sprintf(msg, "(%s) - Please enter a command (or help)\n", cli_db->id);
    cli_cmd_t *cmd = get_cmd(msg);

    int64_t cmd_result;
    if (strcmp(cmd->cmd, CLI_COMMAND_PUT) == 0) {
      cmd_result = put_command(cli_db, cmd);
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_GET) == 0) {
      cmd_result = -1;
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_DELETE) == 0) {
      cmd_result = -1;
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_PRINT) == 0) {
      cmd_result = print_command(cli_db);
    }
    else if (strcmp(cmd->cmd, CLI_COMMAND_EXIT) == 0) {
      /* *** stack smashing detected ***: terminated */
      return 0;
    }
    else {
      fprintf(stderr, "Error: Invalid cmd entered %s\n", cmd->cmd);
      cmd_result = -1;
    }

    if (cmd_result < 0) {
      perror("Error: Failed to execute cmd\n");
    }
  }
}

static cli_cmd_t *get_cmd(uint8_t *msg) {
  uint8_t input_cmd[LINE_BUFFER_SIZE];
  if (msg != NULL) {
    printf(msg);
  }
  printf("> ");
  fgets(input_cmd, LINE_BUFFER_SIZE, stdin);
  input_cmd[strcspn(input_cmd, "\n")] = '\0';
  
  cli_cmd_t *cmd_ptr = malloc(sizeof(cli_cmd_t));
  if(cmd_ptr == NULL) {
    perror("Error: Failed to allocate command memory\n");
    return NULL;
  }

  uint8_t *cmd, *param_1, *param_2, *param_3;
  cmd = strtok(input_cmd, " ");
  param_1 = strtok(NULL, " ");
  param_2 = strtok(NULL, " ");
  param_3 = strtok(NULL, "\n");

  if (cmd == NULL) {
    fprintf(stderr, "Error: Invalid command %s\n", input_cmd);
    free(cmd);
    return NULL;
  }
  else {
    strcpy(cmd_ptr->cmd, cmd);
  }

  if (param_1 != NULL) {
    strcpy(cmd_ptr->param_1, param_1);
  }

  if (param_2 != NULL) {
    strcpy(cmd_ptr->param_2, param_2);
  }

  if (param_3 != NULL) {
    strcpy(cmd_ptr->param_3, param_3);
  }
  
  return cmd_ptr;
}

static int64_t load_command(cli_cmd_t *cmd) {
  if (db_count >= KV_CLI_MAX_OPEN_DATABASES) {
    perror("Reached max number of open databases. Nothing done\n");
    return -1;
  }

  if (strlen(cmd->param_1) <= 0 ||
      strlen(cmd->param_2) <= 0 ||
      strlen(cmd->param_3) <= 0) {
    perror("\"load\" requires three parameters: load <db_path> <db_id> <storage_type>\n");
    return -1;
  }

  uint8_t *path = cmd->param_1;
  uint8_t *id = cmd->param_2;
  uint8_t *storage_type = cmd->param_3;

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    if (strcmp(cli_db->id, id) == 0) {
      fprintf(stderr, "Error: Database with id \"%s\" already exists\n", cli_db->id);
      return -1;
    }
  }

  printf("- Loading database from path: %s as \"%s\"\n ", path, id);

  cli_db_t *cli_db = create_cli_db(path, id, storage_type);
  if (cli_db == NULL) {
    perror("Error: Failed to create db instance\n");
    return -1;
  }

  if (load_db(cli_db->db, cli_db->path) < 0) {
    perror("Error: Failed to load db into memory\n");
    free_cli_db(cli_db);
    return -1;
  }
  
  db_list[db_count] = cli_db;
  db_count++;

  return 0;
}

static int64_t list_command(cli_cmd_t *cmd) {
  if (db_count == 0) {
    printf("- No databases loaded\n");
    return 0;
  }

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    printf("\t%s\t%s\n", cli_db->id, cli_db->path);
  }
  return 0;
}

static int64_t use_command(cli_cmd_t *cmd) {
  if (strlen(cmd->param_1) <= 0) {
    perror("\"use\" requires one parameter: use <db_id>\n");
    return -1;
  }

  int64_t db_idx = -1;
  for (int64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    if (strcmp(cli_db->id, cmd->param_1) == 0) {
      db_idx = i;
      break;
    }
  }

  if (db_idx < 0) {
    fprintf(stderr, "Error: database id %s does not exist\n", cmd->param_1);
    return -1;
  }

  return start_use(db_idx);
}

static int64_t put_command(cli_db_t *cli_db, cli_cmd_t *cmd) {
  if (strlen(cmd->param_1) <= 0 ||
      strlen(cmd->param_2) <= 0) {
    perror("\"put\" requires two or three parameters: use <key> <value> <data_type (optional)>\n");
    return -1;
  }

  uint8_t *key = cmd->param_1;
  uint8_t *value = cmd->param_2;
  uint8_t *type = cmd->param_3;

  if (put_entry(cli_db->db, key, value, type) < 0) {
    fprintf(stderr, "Error: Failed to put entry with key \"%s\"\n", key);
    return -1;
  }

  return 0;
}

extern void free_cli_db(cli_db_t* cli_db) {
  if (cli_db->db != NULL) {
    free_db(cli_db->db);
  }
  free(cli_db);
}

static int64_t print_command(cli_db_t *cli_db) {
  print_db(cli_db->db);
  return 0;
}
