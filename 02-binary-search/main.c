#include "utils/sarray.h"

#include <stdio.h>

// Searches the sorted array A for the first element A[i] such that value <= A[i].
// If all elements are less than value, it returns a pointer to A[A.size] (not dereferenceable).
sarray_data_cptr lower_bound(const sarray* arr, sarray_data_t value) {
    // search [first, first + count) iteratively
    sarray_data_cptr first = arr->data;
    sarray_size_t count = arr->size;
    while (count > 0) {
        sarray_size_t count_half = count / 2;
        sarray_data_cptr mid = first + count_half;
        if (*mid < value) {  // search [mid + 1, first + count)
            first = mid + 1;
            count -= count_half + 1;
        } else {            // search [first, mid) 
            count = count_half;
        }
    }
    return first;
}

// Search the sorted array A for the element A[i] such that A[i] == value.
// If there are more than one matching elements, the result may be any of them.
// If value is not found, a null pointer is returned.
sarray_data_cptr binary_search(const sarray* arr, sarray_data_t value) {
    // search [first, first + count) iteratively
    sarray_data_cptr first = arr->data;
    sarray_size_t count = arr->size;
    while (count > 0) {
        sarray_size_t count_half = count / 2;
        sarray_data_cptr mid = first + count_half;
        if (*mid == value) {
            return mid;
        } else if (*mid < value) {  // search [mid + 1, first + count)
            first = mid + 1;
            count -= count_half + 1;
        } else {                    // search [first, mid) 
            count = count_half;
        }
    }
    return NULL;
}

int main() {
    sarray arr;
    size_t size = 0;
    scanf("%zu", &size);
    sarray_malloc(&arr, size);
    sarray_input(&arr);
    int value = 0;
    scanf("%d", &value);

    sarray_data_cptr p1 = lower_bound(&arr, value);
    sarray_data_cptr p2 = binary_search(&arr, value);

    if (p1 != arr.data + arr.size) {
        printf("p1 points to A[%zu] = %d\n", p1 - arr.data, *p1);
    } else {
        printf("p1 points to A[%zu]\n", p1 - arr.data);
    }
    
    if (p2) {
        printf("p2 points to A[%zu] = %d\n", p2 - arr.data, *p2);
    } else {
        printf("p2 points to 0.\n");
    }

    sarray_output(&arr);
    sarray_free(&arr);
    return 0;
}