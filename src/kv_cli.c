#include "kv_cli.h"

static int64_t main_menu(cli_command_t **cmd_pptr) {
  uint8_t input_command[LINE_BUFFER_SIZE];
  printf("Please enter a command (enter help)\n");
  printf("> ");
  fgets(input_command, LINE_BUFFER_SIZE, stdin);
  input_command[strcspn(input_command, "\n")] = '\0';

  printf("Entered Value: %s\n", input_command);
  
  *cmd_pptr = (cli_command_t*)malloc(sizeof(cli_command_t));
  if(*cmd_pptr == NULL) {
    perror("Error: Failed to allocate command memory");
    return -1;
  }

  (*cmd_pptr)->cmd = strtok(input_command, " ");
  (*cmd_pptr)->param_1 = strtok(NULL, " ");
  (*cmd_pptr)->param_2 = strtok(NULL, " ");
  (*cmd_pptr)->param_3 = strtok(NULL, "\n");

  if ((*cmd_pptr)->cmd == NULL) {
    perror("Error: Failed to read a command\n");
    return -1;
  }

  if (strcmp((*cmd_pptr)->cmd, CLI_STR_COMMAND_LOAD) == 0) {
    return CLI_COMMAND_LOAD;
  }
  else if (strcmp((*cmd_pptr)->cmd, CLI_STR_COMMAND_HELP) == 0) {
    return CLI_COMMAND_HELP;
  }
  else if (strcmp((*cmd_pptr)->cmd, CLI_STR_COMMAND_EXIT) == 0) {
    return CLI_COMMAND_EXIT;
  }
  else {
    fprintf(stderr, "Error: Invalid command entered %s\n", (*cmd_pptr)->cmd);
    return -1;
  }
}

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

  /* Check for the uniqueness of the db_ids */

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
  
  db_list[db_count] = new_db;
  strcpy(db_ids[db_count], cmd_ptr->param_2);
  db_count++;

  return 0;
}

extern void start_cli() {
  while (true) {
    printf("===============================================\n");
    int64_t cmd_result;
    cli_command_t *command_ptr;
    switch (main_menu(&command_ptr)) {
    case CLI_COMMAND_LOAD:
      cmd_result = load_command(command_ptr);
      break;
    case CLI_COMMAND_HELP:
      printf("HELP COMMAND ENTERED\n");
      break;
    case CLI_COMMAND_EXIT:
      printf("EXIT COMMAND ENTERED\n");
      return;
    default:
      perror("Error: Invalid command\n");
      cmd_result = -1;
    }

    if (cmd_result < 0) {
      perror("Error: Failed to execute command\n");
    }
  }
}
