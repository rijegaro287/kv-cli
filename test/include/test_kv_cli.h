#pragma once

#include <stdio.h>
#include <stdint.h>

#include "unity.h"
#include "kv_cli.h"

// Helper functions
// static cli_cmd_t* create_test_command(const char* cmd, const char* param_1, const char* param_2, const char* param_3);
// static cli_db_t* create_test_cli_db(const char* path, const char* id, const char* storage_type);
// static void reset_global_db_state();

extern void setUp(void);
extern void tearDown(void);
extern int main();
