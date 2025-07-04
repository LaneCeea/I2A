#pragma once

#include "sarray.h"

#include <stdbool.h>

// heap operation on sarray

void max_heapify(sarray* this, size_t index, size_t heap_size);
void build_max_heap(sarray* this);
bool is_heap(const sarray* this);