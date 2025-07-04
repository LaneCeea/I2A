#pragma once

#include <stddef.h>

typedef int                     sarray_data_t;
typedef sarray_data_t*          sarray_data_ptr;
typedef const sarray_data_t*    sarray_data_cptr;
typedef size_t                  sarray_size_t;

struct sarray {
    sarray_data_ptr data;
    sarray_size_t size;
};

typedef struct sarray   sarray;

/**
 * Allocates memory for the array and initializes its size.
 * @param this Pointer to the sarray structure to initialize
 * @param size Number of elements to allocate space for
 * @note When size is 0, malloc() behavior is implementation-defined:
 *       may return NULL or a valid pointer that should not be dereferenced.
 *       Therefore, where data points to is implementation-defined.
 */
void sarray_malloc(sarray* this, sarray_size_t size);

/**
 * Frees the allocated memory and resets the array to an empty state.
 * Sets data pointer to NULL and size to 0.
 * @param this Pointer to the sarray structure to free
 * @note Safe to call multiple times on the same array since free(NULL) 
 *       is well-defined (does nothing) and data is set to NULL after freeing.
 *       Only call with pointers previously returned by sarray_malloc.
 */
void sarray_free(sarray* this);

/**
 * Reads input values from stdin to populate the array.
 * @param arr Pointer to the sarray structure to fill with input data
 * @note Expects the array to already be initialized with `sarray_malloc`.
 *       Even if the size of the array is 0, it is safe to call (accept no input).
 */
void sarray_input(sarray* arr);

/**
 * Prints the array contents to stdout in the format: [size] element1, element2, ...
 * @param arr Pointer to the sarray structure to output (const since it's not modified)
 * @note Expects the array to already be initialized with `sarray_malloc`.
 *       Even if the size of the array is 0, it is safe to call.
 */
void sarray_output(const sarray* arr);

void sarray_random(sarray* arr, sarray_data_t lower, sarray_data_t upper);
