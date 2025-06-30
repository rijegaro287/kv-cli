#include "kv_cli.h"

extern int64_t main_menu() {
  uint8_t input_command[LINE_BUFFER_SIZE];
  printf("Please enter a command (enter help)\n");
  printf("> ");
  fgets(input_command, LINE_BUFFER_SIZE, stdin);
  input_command[strcspn(input_command, "\n")] = '\0';

  printf("Entered Value: %s\n", input_command);
  
  cli_command_t command;
  command.cmd = strtok(input_command, " ");
  command.first_param = strtok(NULL, " ");
  command.second_param = strtok(NULL, "\n");

  if (command.cmd == NULL) {
    perror("Error: Failed to read a command\n");
    return -1;
  }

  if (strcmp(command.cmd, CLI_STR_COMMAND_LOAD) == 0) return CLI_COMMAND_LOAD;
  else if (strcmp(command.cmd, CLI_STR_COMMAND_HELP) == 0) return CLI_COMMAND_HELP;
  else if (strcmp(command.cmd, CLI_STR_COMMAND_EXIT) == 0) return CLI_COMMAND_EXIT;
  else {
    fprintf(stderr, "Error: Invalid command entered %s\n", command.cmd);
    return -1;
  }
}

extern void start_cli() {
  while (true) {
    printf("===============================================\n");
    switch (main_menu()) {
    case CLI_COMMAND_LOAD:
      printf("LOAD COMMAND ENTERED\n");
      continue;
    case CLI_COMMAND_HELP:
      printf("HELP COMMAND ENTERED\n");
      continue;
    case CLI_COMMAND_EXIT:
      printf("EXIT COMMAND ENTERED\n");
      return;
    default:
      perror("Error: Invalid command\n");
    }

  // printf("Starting KV Storage CLI...\n");
  // uint8_t database_path[LINE_BUFFER_SIZE];
  // uint8_t database_storage[LINE_BUFFER_SIZE];
  // uint8_t database_id[LINE_BUFFER_SIZE];

  //   printf("- Enter a .db file path to load a database:\n");
  //   fgets(database_path, LINE_BUFFER_SIZE, stdin);
  //   database_path[strcspn(database_path, "\n")] = '\0';

  //   printf("- Enter a type of storage to load the database:\n");
  //   printf("\t- L for Linked List\n");
  //   printf("\t- H for Hash Table\n");
  //   fgets(database_storage, LINE_BUFFER_SIZE, stdin);
  //   database_storage[strcspn(database_storage, "\n")] = '\0';

  //   printf("- Give an alias to the database:\n");
  //   fgets(database_id, LINE_BUFFER_SIZE, stdin);
  //   database_id[strcspn(database_id, "\n")] = '\0';

  //   printf("- Loading database \"%s\" from path: %s\n", database_id,
  //                                                       database_path,
  //                                                       database_storage);
    
  //   if(open_db(database_path, database_storage, database_id) < 0) {
  //     perror("Error: Failed to open database");
  //     continue;
  //   }
  }
}

extern int64_t open_db(uint8_t *db_path, uint8_t *db_storage, uint8_t *db_id) {
  if (db_count >= KV_CLI_MAX_OPEN_DATABASES) {
    perror("Reached max number of open databases. Nothing done");
    return -1;
  }

  db_t *new_db = create_db(db_storage);
  if (new_db == NULL) {
    perror("Error: Failed to create new db\n");
    return -1;
  }
  new_db->id = db_count;
  
  if (load_db(new_db, db_path, db_storage) < 0) {
    perror("Error: Failed to load db into memory\n");
    return -1;
  }
  
  db_list[db_count] = new_db;
  strcpy(db_ids[db_count], db_id);
  db_count++;

  return 0;
}
