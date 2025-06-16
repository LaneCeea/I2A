#include "utils/sarray.h"

#include <string.h>
#include <stdio.h>

// the range [l, m) and [m, r) is sorted, merge them into tmp, and store them back to arr
void _merge(sarray* arr, sarray* tmp, sarray_size_t l, sarray_size_t m, sarray_size_t r) {
    sarray_size_t p = l;
    sarray_size_t q = m;
    
    for (sarray_size_t i = l; i < r; ++i) {
        // Only one of the below special terminating conditions is possible,
        // since if [l, m) and [m, r) are all in result, then i equal size,
        // which will never happen inside this loop.
        if (p >= m) {
            // [l, m) is all in result, append [q, r)
            memcpy(tmp->data + i, arr->data + q, sizeof(sarray_data_t) * (r - q));
            break;
        } else if (q >= r) {
            // [m, r) is all in result, append [p, m)
            memcpy(tmp->data + i, arr->data + p, sizeof(sarray_data_t) * (m - p));
            break;
        }

        // find smallest
        if (arr->data[p] <= arr->data[q]) {
            tmp->data[i] = arr->data[p];
            ++p;
        } else {
            tmp->data[i] = arr->data[q];
            ++q;
        }
    }

    // store the result back to arr
    memcpy(arr->data + l, tmp->data + l, sizeof(sarray_data_t) * (r - l));
}

// sort the range [l, r)
void _merge_sort(sarray* arr, sarray* tmp, sarray_size_t l, sarray_size_t r) {
    if (l + 1 >= r) { // only one element or less
        return;
    }
    sarray_size_t m = l + (r - l) / 2; // equivalent to (l + r) / 2 but avoids overflow
    _merge_sort(arr, tmp, l, m);
    _merge_sort(arr, tmp, m, r);
    _merge(arr, tmp, l, m, r);
}

void merge_sort(sarray* arr) {
    sarray tmp;
    sarray_malloc(&tmp, arr->size);
    _merge_sort(arr, &tmp, 0, arr->size);
    sarray_free(&tmp);
}

int main() {
    sarray arr;
    size_t size = 0;
    scanf("%zu", &size);
    sarray_malloc(&arr, size);
    sarray_input(&arr);

    merge_sort(&arr);

    sarray_output(&arr);
    sarray_free(&arr);
    return 0;
}