#include "utils/sarray.h"

#include <string.h>
#include <stdio.h>

void insertion_sort(sarray* arr) {
    for (sarray_size_t i = 1; i < arr->size; ++i) {
        sarray_data_t key = arr->data[i];
        sarray_size_t j = i;
        while (j > 0 && arr->data[j - 1] > key) {
            --j;
        }
        if (i != j) { // only modify when we have to move data[i]
            // move data[j .. (i-1)] to data[(j+1) .. i]
            memmove(arr->data + j + 1, arr->data + j, sizeof(sarray_data_t) * (i - j));
            arr->data[j] = key;
        }
    }
}

int main() {
    sarray arr;
    size_t size = 0;
    scanf("%zu", &size);
    sarray_malloc(&arr, size);
    sarray_input(&arr);

    insertion_sort(&arr);

    sarray_output(&arr);
    sarray_free(&arr);
    return 0;
}