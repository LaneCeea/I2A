#pragma once

#include <stddef.h>

typedef int             sarray_data_t;
typedef sarray_data_t*  sarray_data_ptr;
typedef size_t          sarray_size_t;

struct sarray {
    sarray_data_ptr data;
    sarray_size_t size;
};

typedef struct sarray   sarray;

void sarray_malloc(sarray* this, sarray_size_t size);
void sarray_free(sarray* this);
void sarray_input(sarray* arr);
void sarray_output(const sarray* arr);