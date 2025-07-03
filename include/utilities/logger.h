#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "constants.h"

static uint64_t LOG_LEVEL = INITIAL_LOG_LEVEL;

extern void set_log_level(uint64_t log_level);
extern void logger(uint64_t log_level, uint8_t* message, ...);
