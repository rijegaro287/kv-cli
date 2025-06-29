# pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "kv_controller.h"

static db_t *db_list[KV_CLI_MAX_OPEN_DATABASES];
static uint64_t db_count = 0;

extern void start_cli();
extern int64_t open_db(uint8_t *db_path, uint8_t *storage_structure);
