#include "kv_cli.h"

static cli_cmd_t *get_cmd() {
  uint8_t input_cmd[LINE_BUFFER_SIZE];
  printf("Please enter a command (or help)\n");
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
    free(cmd_ptr);
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

static int64_t load_command(cli_cmd_t *cmd_ptr) {
  if (db_count >= KV_CLI_MAX_OPEN_DATABASES) {
    perror("Reached max number of open databases. Nothing done\n");
    return -1;
  }

  if (strlen(cmd_ptr->param_1) <= 0 ||
      strlen(cmd_ptr->param_2) <= 0 ||
      strlen(cmd_ptr->param_3) <= 0) {
    perror("\"load\" requires three parameters: load <db_path> <db_alias> <storage_type>\n");
    return -1;
  }

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    if (strcmp(cli_db->id, cmd_ptr->param_2) == 0) {
      fprintf(stderr, "Error: Database with alias \"%s\" already exists\n", cli_db->id);
      return -1;
    }
  }

  printf("- Loading database from path: %s as \"%s\"\n ", cmd_ptr->param_1, 
                                                          cmd_ptr->param_2);

  cli_db_t *cli_db = malloc(sizeof(cli_db_t));
  cli_db->db = create_db(cmd_ptr->param_3);
  if (cli_db->db == NULL) {
    perror("Error: Failed to create new db\n");
    return -1;
  }
  strcpy(cli_db->path, cmd_ptr->param_1);
  strcpy(cli_db->id, cmd_ptr->param_2);
  cli_db->db->id = db_count;

  if (load_db(cli_db->db, cmd_ptr->param_1) < 0) {
    perror("Error: Failed to load db into memory\n");
    free(cli_db);
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
    perror("\"use\" requires one parameter: use <db_alias>\n");
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
    fprintf(stderr, "Error: database alias %s does not exist\n", cmd->param_1);
    return -1;
  }

  return start_use(db_idx);
}

static int64_t start_use(uint64_t db_idx) {
  cli_db_t *cli_db = db_list[db_idx];
  while (true) {
    printf("-----------------------------------------------\n");
    cli_cmd_t *cmd_ptr = get_cmd();
  }
}

extern void start_cli() {
  while (true) {
    printf("===============================================\n");
    cli_cmd_t *cmd_ptr = get_cmd();
    int64_t cmd_result;
    if (strcmp(cmd_ptr->cmd, CLI_STR_COMMAND_LOAD) == 0) {
      cmd_result = load_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_STR_COMMAND_USE) == 0) {
      cmd_result = use_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_STR_COMMAND_LIST) == 0) {
      cmd_result = list_command(cmd_ptr);
    }
    else if (strcmp(cmd_ptr->cmd, CLI_STR_COMMAND_HELP) == 0) {
      cmd_result = 0;
    }
    else if (strcmp(cmd_ptr->cmd, CLI_STR_COMMAND_EXIT) == 0) {
      return;
    }
    else {
      fprintf(stderr, "Error: Invalid cmd entered %s\n", cmd_ptr->cmd);
      int64_t cmd_result = -1;
    }

    if (cmd_result < 0) {
      perror("Error: Failed to execute cmd\n");
    }
  }
}
