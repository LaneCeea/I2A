#include "utils/timer.h"
#include "utils/pcg32.h"
#include "utils/sarray.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static
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

static
bool is_sorted(const sarray* arr) {
    for (size_t i = 1; i < arr->size; ++i) {
        if (arr->data[i - 1] > arr->data[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    pcg32_seed(666U, 999U);
    printf("insertion sort time measuring\n");
    sarray arr;
    uint32_t size = 1;
    for (int exp = 0; exp < 32; ++exp) {
        sarray_malloc(&arr, size);
        sarray_random(&arr, 0, size);
        
        TIMER_INIT(sort);
        TIMER_START(sort);
        insertion_sort(&arr);
        TIMER_STOP(sort);
        double time = TIMER_ELAPSED_MSEC(sort);
        printf("size 2^(%2u) - %13.6f ms\n", exp, time);

        if (!is_sorted(&arr)) {
            printf("not sorted!\n");
        }

        sarray_free(&arr);
        size <<= 1;
    }
    return 0;
}