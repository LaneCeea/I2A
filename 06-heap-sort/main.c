#include "utils/timer.h"
#include "utils/sheap.h"
#include "utils/pcg32.h"

#include <stdio.h>

static inline
void _swap(sarray* arr, size_t lhs, size_t rhs) {
    sarray_data_t temp = arr->data[lhs];
    arr->data[lhs] = arr->data[rhs];
    arr->data[rhs] = temp;
}

static
void heap_sort(sarray* arr) {
    if (arr->size <= 1) {
        return;
    }
    build_max_heap(arr);
    for (size_t i = arr->size - 1; i > 0; --i) {
        _swap(arr, 0, i);
        max_heapify(arr, 0, i);
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
    printf("heap sort time measuring\n");
    sarray arr;
    uint32_t size = 1;
    for (int exp = 0; exp < 32; ++exp) {
        sarray_malloc(&arr, size);
        sarray_random(&arr, 0, size);
        
        TIMER_INIT(sort);
        TIMER_START(sort);
        heap_sort(&arr);
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