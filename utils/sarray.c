#include "sarray.h"

#include "pcg32.h"
#include "log.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define SCNF_DATA SCNd32
#define PRIF_DATA PRId32

void sarray_malloc(sarray* this, sarray_size_t size) {
    this->data = (sarray_data_ptr)malloc(sizeof(sarray_data_t) * size);
    CORE_ASSERT(this->data, "sarray_malloc: Failed to malloc\n");
    this->size = size;
}

void sarray_free(sarray* this) {
    free(this->data);
    this->data = NULL;
    this->size = 0;
}

void sarray_input(sarray* arr) {
    sarray_data_ptr begin = arr->data;
    sarray_data_ptr end = begin + arr->size;
    for (; begin < end; ++begin) {
        scanf("%" SCNF_DATA, begin);
    }
}

void sarray_output(const sarray* arr) {
    printf("[%zu] ", arr->size);
    if (arr->size) {
        sarray_data_ptr begin = arr->data;
        sarray_data_ptr end = begin + arr->size;
        printf("%" PRIF_DATA, *begin);
        ++begin;
        for (; begin < end; ++begin) {
            printf(", %" PRIF_DATA, *begin);
        }
    }
    printf("\n");
}

void sarray_random(sarray* arr, sarray_data_t lower, sarray_data_t upper) {
    for (size_t i = 0; i < arr->size; ++i) {
        arr->data[i] = pcg32_uniform_lemire(lower, upper);
    }
}

