#pragma once

#include <stdio.h>
#include <stdlib.h>

#define CORE_ERROR(...)                 { printf(__VA_ARGS__); abort(); }
#define CORE_ASSERT(Expression, ...)    if (!(Expression)) CORE_ERROR(__VA_ARGS__)
