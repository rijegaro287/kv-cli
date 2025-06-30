#include "kv_cli.h"

static int64_t load_command(cli_command_t *cmd_ptr) {
  if (db_count >= KV_CLI_MAX_OPEN_DATABASES) {
    perror("Reached max number of open databases. Nothing done\n");
    return -1;
  }

  if (cmd_ptr->param_1 == NULL ||
      cmd_ptr->param_2 == NULL ||
      cmd_ptr->param_3 == NULL) {
    perror("\"load\" requires three parameters as arguments: load <db_path> <db_alias> <storage_type>\n");
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

  db_t *new_db = create_db(cmd_ptr->param_3);
  if (new_db == NULL) {
    perror("Error: Failed to create new db\n");
    return -1;
  }
  new_db->id = db_count;
  
  if (load_db(new_db, cmd_ptr->param_1, cmd_ptr->param_3) < 0) {
    perror("Error: Failed to load db into memory\n");
    return -1;
  }
  
  cli_db_t *cli_db = malloc(sizeof(cli_db_t));
  cli_db->db = new_db;
  strcpy(cli_db->path, cmd_ptr->param_1);
  strcpy(cli_db->id, cmd_ptr->param_2);
  
  db_list[db_count] = cli_db;
  db_count++;

  return 0;
}

static int64_t list_command(cli_command_t *command) {
  if (db_count == 0) {
    printf("- No databases loaded\n");
    return 0;
  }

  for (uint64_t i = 0; i < db_count; i++) {
    cli_db_t *cli_db = db_list[i];
    printf("\t%s\t%s\n", cli_db->path, cli_db->id);
  }
  return 0;
}

extern void start_cli() {
  while (true) {
    printf("===============================================\n");    
    uint8_t input_cmd[LINE_BUFFER_SIZE];
    printf("Please enter a command (enter help)\n");
    printf("> ");
    fgets(input_cmd, LINE_BUFFER_SIZE, stdin);
    input_cmd[strcspn(input_cmd, "\n")] = '\0';
    
    printf("Entered Value: %s\n", input_cmd);
    
    cli_command_t *cmd_ptr;
    cmd_ptr = malloc(sizeof(cli_command_t));
    if(cmd_ptr == NULL) {
      perror("Error: Failed to allocate command memory");
      continue;
    }

    cmd_ptr->cmd = strtok(input_cmd, " ");
    cmd_ptr->param_1 = strtok(NULL, " ");
    cmd_ptr->param_2 = strtok(NULL, " ");
    cmd_ptr->param_3 = strtok(NULL, "\n");

    if (cmd_ptr->cmd == NULL) {
      perror("Error: Failed to read a command\n");
      continue;
    }

    int64_t cmd_result;
    if (strcmp(cmd_ptr->cmd, CLI_STR_COMMAND_LOAD) == 0) {
      cmd_result = load_command(cmd_ptr);
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
      fprintf(stderr, "Error: Invalid command entered %s\n", cmd_ptr->cmd);
      int64_t cmd_result = -1;
    }

    if (cmd_result < 0) {
      perror("Error: Failed to execute command\n");
    }
  }
}
