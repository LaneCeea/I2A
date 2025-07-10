#include "utils/timer.h"
#include "utils/sarray.h"
#include "utils/pcg32.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static inline
void swap(sarray* arr, size_t lhs, size_t rhs) {
    sarray_data_t tmp = arr->data[lhs];
    arr->data[lhs] = arr->data[rhs];
    arr->data[rhs] = tmp;
}

static inline
bool sarray_is_sort(const sarray* arr) {
    for (size_t i = 1; i < arr->size; ++i) {
        if (arr->data[i - 1] > arr->data[i]) {
            return false;
        }
    }
    return true;
}

typedef size_t (*partition_fn_t)(sarray*, size_t, size_t);
static size_t partition_lomuto(sarray* arr, size_t first, size_t last);
static size_t partition_hoare(sarray* arr, size_t first, size_t last);
static void _quick_sort(sarray* arr, size_t first, size_t last, partition_fn_t partition);
static void quick_sort(sarray* arr, partition_fn_t partition);

typedef struct {
    size_t first;
    size_t last;
} three_way_t;

static void _sort_med3(sarray* arr, size_t i1, size_t i2, size_t i3);
static three_way_t partition_bentley_mcilroy(sarray* arr, size_t first, size_t pfirst, size_t last);
static void _quick_yeet(sarray* arr, size_t first, size_t last);
static void quick_yeet(sarray* arr);

////////////////////////////////////////////////////////////////////////////////
// definition
////////////////////////////////////////////////////////////////////////////////

static
size_t partition_lomuto(sarray* arr, size_t first, size_t last) {
    size_t tail = last - 1;
    int pivot = arr->data[tail];
    size_t i = first;
    for (size_t j = first; j < tail; ++j) {
        // data[first .. i) <= pivot < data[i .. j)
        if (arr->data[j] <= pivot) {
            swap(arr, i, j);
            ++i;
        }
    }
    swap(arr, i, tail);
    return i;
}

static
size_t partition_hoare(sarray* arr, size_t first, size_t last) {
    int pivot = arr->data[first];
    size_t i = first + 1;
    size_t j = last;
    for (;;) {
        // data[first .. i) <= pivot < data[j .. last)
        while (i < j && arr->data[i] <= pivot) {
            ++i;
        }
        while (i < j && pivot < arr->data[j - 1]) {
            --j;
        }
        if (i >= j) {
            break;
        }
        --j;
        swap(arr, i, j);
        ++i;
    }
    swap(arr, first, i - 1);
    return i - 1;
}

static
void _quick_sort(sarray* arr, size_t first, size_t last, partition_fn_t partition) {
    if (first < last) {
        size_t mid = partition(arr, first, last);
        _quick_sort(arr, first, mid, partition);
        _quick_sort(arr, mid + 1, last, partition);
    }
}

static
void quick_sort(sarray* arr, partition_fn_t partition) {
    _quick_sort(arr, 0, arr->size, partition);
}

static
void _sort_med3(sarray* arr, size_t i1, size_t i2, size_t i3) {
    // sort x, y, z into non-decreasing order
    if (arr->data[i1] > arr->data[i2]) {
        swap(arr, i1, i2);
    }
    // x <= y
    if (arr->data[i2] > arr->data[i3]) {
        swap(arr, i2, i3);
        // x <= z and y <= z
        if (arr->data[i1] > arr->data[i2]) {
            swap(arr, i1, i2);
        }
    }
}

static
three_way_t partition_bentley_mcilroy(sarray* arr, size_t first, size_t pfirst, size_t last) {
    // goal: partition into three regions
    // A1 = A[first .. pfirst),
    // A2 = A[pfirst .. plast),
    // A3 = A[plast .. last),
    // where A1 < A2 = pivot < A3

    const int pivot = arr->data[pfirst];
    size_t plast = pfirst + 1;

    while (first < pfirst && arr->data[pfirst - 1] == pivot) {
        --pfirst;
    }
    while (plast < last && arr->data[plast] == pivot) {
        ++plast;
    }
    // A2 = pivot

    size_t gfirst = plast;
    size_t glast = pfirst;
    
    for (;;) {
        // A[glast .. pfirst) < A2 < A[plast .. gfirst]
        for (; gfirst < last; ++gfirst) {
            // A2 < A[plast .. gfirst)
            if (pivot < arr->data[gfirst]) {
                continue;
            } else if (arr->data[gfirst] < pivot) {
                break;
            }
            // found A[gfirst] = pivot in A3, swap to A2
            swap(arr, plast, gfirst);
            ++plast;
        }
        // found A[gfirst] < pivot in A3, need to swap to A1
        // or gfirst = last
        
        for (; first < glast; --glast) {
            // A[glast .. pfirst) < A2
            const size_t glast_prev = glast - 1;
            if (arr->data[glast_prev] < pivot) {
                continue;
            } else if (pivot < arr->data[glast_prev]) {
                break;
            }
            // found pivot = A[glast] in A1, swap to A2
            --pfirst;
            swap(arr, pfirst, glast_prev);
        }
        // found pivot < A[glast - 1] in A1, need to swap to A3
        // or first = glast

        if (first == glast && gfirst == last) {
            // achieve A1 < A2 < A3
            return (three_way_t){ pfirst, plast };
        }

        if (first == glast) {
            // no element in A1 need to swap to A3,
            // but A[gfirst] < pivot in A3 need to swap down,
            // rotate pivot upward:
            //
            // |------------A2-------------|
            // | pfirst   | .............. | plast | ................ | gfirst   |
            //     b                           c                          a
            //
            //            |------------A2------------|
            // | pfirst-1 | pfirst | ..... | plast-1 | plast | ...... | gfirst-1 | gfirst |
            //     a                           b                          c
            //
            if (plast != gfirst) {
                swap(arr, pfirst, plast);
            }
            swap(arr, pfirst, gfirst);
            ++pfirst;
            ++plast;
            ++gfirst;
        } else if (gfirst == last) {
            // no element in A3 need to swap to A1,
            // but pivot < A[glast - 1] in A1 need to swap up,
            // rotate pivot downward:
            //
            //                                   |------------------A2-----------------|
            // | glast-1 | .......... | pfirst-1 | pfirst   | .............. | plast-1 | plast |
            //     c                       a         b                           b
            //
            //                        |------------------A2------------------|
            // | glast   | .......... | pfirst   | pfirst+1 | .............. | plast   |
            //     a                       b         b                           c
            //
            --glast;
            --pfirst;
            --plast;
            if (glast != pfirst) {
                swap(arr, pfirst, plast);
            }
            swap(arr, glast, plast);
        } else {
            --glast;
            swap(arr, gfirst, glast);
            ++gfirst;
        }
    }
}

static
void _quick_yeet(sarray* arr, size_t first, size_t last) {
    if (first < last) {
        size_t mid = first + (last - first) / 2;
        _sort_med3(arr, first, mid, last);
        three_way_t pivot = partition_bentley_mcilroy(arr, first, mid, last);
        _quick_yeet(arr, first, pivot.first);
        _quick_yeet(arr, pivot.last, last);
    }
}

static
void quick_yeet(sarray* arr) {
    _quick_yeet(arr, 0, arr->size);
}

void sarray_random_nearly_sort(sarray* arr) {
    for (size_t i = 0; i < arr->size; ++i) {
        arr->data[i] = pcg32_uniform_lemire(i, i + 15);
    }
}

int main() {
    pcg32_seed(666U, 999U);
    printf("quick sort time measuring\n");
    printf("nearly sorted array:\n");
    printf("for each valid index, i <= A[i] < i + 16\n");
    sarray arr;
    uint32_t size = 1;
    for (int exp = 0; exp < 32; ++exp) {
        sarray_malloc(&arr, size);
        sarray_random_nearly_sort(&arr);
        
        TIMER_INIT(sort);
        TIMER_START(sort);
        quick_sort(&arr, partition_hoare);
        TIMER_STOP(sort);
        double time = TIMER_ELAPSED_MSEC(sort);
        printf("size 2^(%2u) - %13.6f ms\n", exp, time);

        if (!sarray_is_sort(&arr)) {
            printf("not sorted!\n");
        }

        sarray_free(&arr);
        size <<= 1;
    }
    return 0;
}
