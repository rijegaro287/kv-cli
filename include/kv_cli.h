/**
 * @file kv_cli.h
 * @brief Key-Value CLI Interface Header
 * @details This header file defines the interface for a command-line interface (CLI)
 *          that manages file-based key-value databases. It provides functionality to
 *          load, manage, and interact with multiple databases through a simple command
 *          interface.
 * @author rijegaro287
 * @date 2025
 * @version 1.0
 */

#pragma once

#include "kv_controller.h"

/**
 * @defgroup CLI_COMMANDS Command String Definitions
 * @brief String constants for CLI commands
 * @{
 */

/** @brief Command to load a new database */
#define CLI_COMMAND_LOAD "load"

/** @brief Command to reload an existing database */
#define CLI_COMMAND_RELOAD "reload"

/** @brief Command to close a database */
#define CLI_COMMAND_CLOSE "close"

/** @brief Command to list all loaded databases */
#define CLI_COMMAND_LIST "list"

/** @brief Command to switch to using a specific database */
#define CLI_COMMAND_USE "use"

/** @brief Command to show help information */
#define CLI_COMMAND_HELP "help"

/** @brief Command to exit the CLI */
#define CLI_COMMAND_EXIT "exit"

/** @brief Command to put a key-value pair */
#define CLI_COMMAND_PUT "put"

/** @brief Command to get a value by key */
#define CLI_COMMAND_GET "get"

/** @brief Command to delete a key-value pair */
#define CLI_COMMAND_DELETE "delete"

/** @brief Command to print all key-value pairs */
#define CLI_COMMAND_PRINT "print"

/** @} */

/**
 * @defgroup CLI_STRUCTURES Data Structures
 * @brief Core data structures for CLI database management
 * @{
 */

/**
 * @struct _cli_db_t
 * @brief CLI Database Structure
 * @details Represents a database instance in the CLI with path, identifier, and database handle
 */
typedef struct _cli_db_t {
  uint8_t path[BG_BUFFER_SIZE];  /**< File system path to the database */
  uint8_t id[SM_BUFFER_SIZE];    /**< Unique identifier for the database */
  db_t *db;                      /**< Pointer to the database structure */
} cli_db_t;

/**
 * @struct _cli_cmd_t
 * @brief CLI Command Structure
 * @details Represents a parsed CLI command with up to 3 parameters
 */
typedef struct _cli_cmd_t {
  uint8_t cmd[SM_BUFFER_SIZE];      /**< Main command string */
  uint8_t param_1[SM_BUFFER_SIZE];  /**< First parameter */
  uint8_t param_2[SM_BUFFER_SIZE];  /**< Second parameter */
  uint8_t param_3[SM_BUFFER_SIZE];  /**< Third parameter */
} cli_cmd_t;

/** @} */

/**
 * @defgroup CLI_GLOBALS Global Variables
 * @brief Global state variables for the CLI
 * @{
 */

/** @brief Array of pointers to loaded databases */
static cli_db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];

/** @brief Number of currently loaded databases */
static uint64_t db_count = 0;

/** @} */

/**
 * @defgroup CLI_GETTERS Database State Access Functions
 * @brief Functions for accessing global database state
 * @{
 */

/**
 * @brief Get pointer to the database list array
 * @details Provides access to the global array of loaded database pointers
 * @return Pointer to the array of cli_db_t pointers
 */
extern cli_db_t **get_db_list();

/**
 * @brief Get the current number of loaded databases
 * @details Returns the count of databases currently loaded in memory
 * @return Number of databases currently loaded
 */
extern uint64_t get_db_count();

/** @} */

/**
 * @defgroup CLI_CORE Core CLI Functions
 * @brief Core functionality for CLI operation and database management
 * @{
 */

/**
 * @brief Get command input from user
 * @details Prompts the user with a message and reads command input from stdin
 * @param msg Optional message to display to user (can be NULL)
 * @param dest Destination buffer to store the input command
 * @param max_len Maximum length of input to read
 * @return 0 on success, negative value on failure
 */
extern int64_t get_command_from_user(uint8_t *msg, uint8_t *dest, uint64_t max_len);

/**
 * @brief Create a new CLI database instance
 * @details Creates and initializes a new CLI database with specified parameters
 * @param path File system path where the database is stored
 * @param id Unique identifier for the database
 * @param storage_type Storage type for the database (e.g., "L" for linked list, "H" for hash table)
 * @return Pointer to created cli_db_t structure, NULL on failure
 */
extern cli_db_t *create_cli_db(uint8_t *path, uint8_t *id, uint8_t *storage_type);

/**
 * @brief Parse command string into command structure
 * @details Parses a command string and creates a cli_cmd_t structure with command and parameters
 * @param command String containing the command to parse
 * @return Pointer to created cli_cmd_t structure, NULL on failure
 */
extern cli_cmd_t *create_command(uint8_t *command);

/**
 * @brief Start interactive mode for a specific database
 * @details Enters interactive mode where user can perform operations on the specified database
 * @param db_idx Index of the database in the db_list array
 * @return 0 when user exits interactive mode, negative value on error
 */
extern int64_t start_use(uint64_t db_idx);

/** @} */

/**
 * @defgroup CLI_DB_COMMANDS Database Management Commands
 * @brief Functions implementing database management operations
 * @{
 */

/**
 * @brief Load a database from file
 * @details Loads a database from specified path with given identifier and storage type
 * @param cmd_ptr Pointer to command structure containing path, id, and storage type parameters
 * @return 0 on success, negative value on failure
 */
extern int64_t load_command(cli_cmd_t *cmd_ptr);

/**
 * @brief Reload an existing database with different storage type
 * @details Reloads a database that's already loaded with a new storage type
 * @param cmd_ptr Pointer to command structure containing database id and new storage type
 * @return 0 on success, negative value on failure
 */
extern int64_t reload_command(cli_cmd_t *cmd_ptr);

/**
 * @brief Close and remove a database from memory
 * @details Closes a loaded database and removes it from the active database list
 * @param cmd_ptr Pointer to command structure containing database id to close
 * @return 0 on success, negative value on failure
 */
extern int64_t close_command(cli_cmd_t *cmd_ptr);

/**
 * @brief List all currently loaded databases
 * @details Displays information about all databases currently loaded in memory
 * @param cmd_ptr Pointer to command structure (unused but required for consistency)
 * @return 0 on success, negative value on failure
 */
extern int64_t list_command(cli_cmd_t *cmd_ptr);

/**
 * @brief Switch to using a specific database
 * @details Enters interactive mode for the specified database
 * @param cmd_ptr Pointer to command structure containing database id to use
 * @return 0 when user exits interactive mode, negative value on error
 */
extern int64_t use_command(cli_cmd_t *cmd_ptr);

/** @} */
/**
 * @defgroup CLI_KV_COMMANDS Key-Value Operations
 * @brief Functions implementing key-value database operations
 * @{
 */

/**
 * @brief Put a key-value pair into the database
 * @param cli_db Pointer to the CLI database instance
 * @param cmd_ptr Pointer to command structure containing key, value, and type
 * @return 0 on success, negative value on failure
 */
extern int64_t put_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);

/**
 * @brief Get a value by key from the database
 * @param cli_db Pointer to the CLI database instance
 * @param cmd_ptr Pointer to command structure containing the key
 * @return 0 on success, negative value on failure
 */
extern int64_t get_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);

/**
 * @brief Delete a key-value pair from the database
 * @param cli_db Pointer to the CLI database instance
 * @param cmd_ptr Pointer to command structure containing the key
 * @return 0 on success, negative value on failure
 */
extern int64_t delete_command(cli_db_t *cli_db, cli_cmd_t *cmd_ptr);

/**
 * @brief Print all key-value pairs in the database
 * @param cli_db Pointer to the CLI database instance
 * @return 0 on success, negative value on failure
 */
extern int64_t print_command(cli_db_t *cli_db);

/** @} */

/**
 * @defgroup CLI_HELP Help Functions
 * @brief Functions providing help information
 * @{
 */

/**
 * @brief Display main help information
 * @details Shows help information for main CLI commands available at the top level
 * @return 0 on success
 */
extern int64_t main_help_command();

/**
 * @brief Display help information for database operations
 * @details Shows help information for commands available when using a specific database
 * @return 0 on success
 */
extern int64_t use_help_command();

/** @} */

/**
 * @defgroup CLI_MAIN Main CLI Functions
 * @brief Main CLI startup and lifecycle functions
 * @{
 */

/**
 * @brief Start the main CLI loop
 * @details Initializes and runs the main command loop for the CLI application
 */
extern void start_cli();

/** @} */

/**
 * @defgroup CLI_MEMORY Memory Management Functions
 * @brief Functions for memory cleanup and resource management
 * @{
 */

/**
 * @brief Free CLI database structure
 * @details Frees memory allocated for a CLI database instance and its associated database
 * @param cli_db Pointer to the CLI database structure to free
 */
extern void free_cli_db(cli_db_t *cli_db);

/**
 * @brief Free CLI command structure
 * @details Frees memory allocated for a CLI command structure
 * @param cmd_ptr Pointer to the CLI command structure to free
 */
extern void free_cli_command(cli_cmd_t *cmd_ptr);

/**
 * @brief Free all databases in the database list
 * @details Frees all loaded databases and resets the database count
 */
extern void free_db_list();

/** @} */
