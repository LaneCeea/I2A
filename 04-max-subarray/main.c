#include "utils/timer.h"
#include "utils/sarray.h"
#include "utils/pcg32.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// subarray
////////////////////////////////////////////////////////////////////////////////////////////////////

struct subarray {
    ptrdiff_t first;
    ptrdiff_t last;
    int sum;
};

static inline
int subarray_is_equal(
    const struct subarray* lhs,
    const struct subarray* rhs
) {
    if (lhs->sum == rhs->sum) {
        if (lhs->first == rhs->first && lhs->last == rhs->last) {
            return 2;
        }
        return 1;
    }
    return 0;
}

static inline
bool subarray_is_less(
    const struct subarray* lhs,
    const struct subarray* rhs
) {
    if (lhs->sum == rhs->sum) {
        ptrdiff_t lhs_interval = lhs->last - lhs->first;
        ptrdiff_t rhs_interval = rhs->last - rhs->first;
        if (lhs_interval == rhs_interval) {
            return lhs->first > rhs->first;
        }
        return lhs_interval > rhs_interval;
    }
    return lhs->sum < rhs->sum;
}

static inline
const struct subarray* subarray_max(
    const struct subarray* r1,
    const struct subarray* r2,
    const struct subarray* r3
) {
    const struct subarray* max = r1;
    if (subarray_is_less(max, r2)) {
        max = r2;
    }
    if (subarray_is_less(max, r3)) {
        max = r3;
    }
    return max;
}

static inline
void subarray_print(const struct subarray* sub) {
    printf("\t[%zd, %zd) = %d\n", sub->first, sub->last, sub->sum);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// brute force
////////////////////////////////////////////////////////////////////////////////////////////////////

// check every possible subarray [first, last)

static
struct subarray _brute_force(const sarray* arr, ptrdiff_t first, ptrdiff_t last) {
    struct subarray ret = {
        .first = 0,
        .last = 0,
        .sum = 0
    };

    struct subarray current = { 0 };

    for (current.first = first; current.first < last; ++current.first) {
        current.sum = 0;
        for (current.last = current.first + 1; current.last <= last; ++current.last) {
            current.sum += arr->data[current.last - 1];
            if (subarray_is_less(&ret, &current)) {
                ret = current;
            }
        }
    }

    return ret;
}

static
struct subarray brute_force(const sarray* arr) {
    return _brute_force(arr, 0, arr->size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// divide and conquer
////////////////////////////////////////////////////////////////////////////////////////////////////

// find the maximum subarray of arr[first : last) which must includes arr[mid-1 : mid]
// first <= i < mid < j <= last
// arr[i : mid) + arr[mid : j)
static
struct subarray _max_cross_mid(const sarray* arr, ptrdiff_t first, ptrdiff_t mid, ptrdiff_t last);

// find the maximum subarray of arr[first : last)
static
struct subarray _divide_and_conquer(const sarray* arr, ptrdiff_t first, ptrdiff_t last);

static
struct subarray divide_and_conquer(const sarray* arr);

static
struct subarray _max_cross_mid(const sarray* arr, ptrdiff_t first, ptrdiff_t mid, ptrdiff_t last) {
    if (mid - 1 < first || mid >= last) {
        // valid cross subarray does not exist
        struct subarray empty = { 0 };
        return empty;
    }

    struct subarray ret = {
        .first = mid - 1,
        .last = mid + 1,
        .sum = arr->data[mid - 1] + arr->data[mid]
    };

    int tmp_sum = ret.sum;
    for (ptrdiff_t i = ret.first - 1; i >= first; --i) {
        tmp_sum += arr->data[i];
        if (ret.sum < tmp_sum) {
            ret.sum = tmp_sum;
            ret.first = i;
        }
    }

    tmp_sum = ret.sum;
    for (ptrdiff_t j = ret.last; j < last; ++j) {
        tmp_sum += arr->data[j];
        if (ret.sum < tmp_sum) {
            ret.sum = tmp_sum;
            ret.last = j + 1;
        }
    }

    return ret;
}

static
struct subarray _divide_and_conquer(const sarray* arr, ptrdiff_t first, ptrdiff_t last) {
    size_t size = last - first;
    if (size <= 1) {
        if (size == 0 || arr->data[first] <= 0) {
            // choose empty set
            struct subarray ret = { 0 };
            return ret;
        }
        // choose this element
        struct subarray ret = {
            .first = first,
            .last = last,
            .sum = arr->data[first]
        };
        return ret;
    }

    ptrdiff_t mid = first + size / 2;

    struct subarray left  = _divide_and_conquer(arr, first, mid);
    struct subarray right = _divide_and_conquer(arr, mid, last);
    struct subarray cross = _max_cross_mid(arr, first, mid, last);

    // return the maximum from the three
    const struct subarray* max = subarray_max(&left, &cross, &right);
    return *max;
}

static
struct subarray divide_and_conquer(const sarray* arr) {
    if (arr->size == 0) {
        struct subarray zero = { 0 };
        return zero;
    }
    return _divide_and_conquer(arr, 0, arr->size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// hybrid
////////////////////////////////////////////////////////////////////////////////////////////////////

static
struct subarray _hybrid(const sarray* arr, ptrdiff_t first, ptrdiff_t last) {
    size_t size = last - first;
    if (size < 32) {
        return _brute_force(arr, first, last);
    }

    ptrdiff_t mid = first + size / 2;

    struct subarray left  = _hybrid(arr, first, mid);
    struct subarray right = _hybrid(arr, mid, last);
    struct subarray cross = _max_cross_mid(arr, first, mid, last);

    const struct subarray* max = subarray_max(&left, &cross, &right);
    return *max;
}

static
struct subarray hybrid(const sarray* arr) {
    if (arr->size == 0) {
        struct subarray zero = { 0 };
        return zero;
    }
    return _hybrid(arr, 0, arr->size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Kadane
////////////////////////////////////////////////////////////////////////////////////////////////////

static struct subarray kadane(const sarray* arr) {
    struct subarray ret = { 0 };
    struct subarray max_ending = { 0 };

    for (size_t i = 0; i < arr->size; ++i) {
        if (max_ending.sum > 0) {
            max_ending.last  = i + 1;
            max_ending.sum  += arr->data[i];
        } else {
            max_ending.first = i;
            max_ending.last  = i + 1;
            max_ending.sum   = arr->data[i];
        }

        if (subarray_is_less(&ret, &max_ending)) {
            ret = max_ending;
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    sarray arr;
    size_t size = 0;
    int lower = 0;
    int upper = 0;
    int n_trial = 0;

    scanf("%zu%d%d%d", &size, &lower, &upper, &n_trial);
    sarray_malloc(&arr, size);
    pcg32_seed(666u, 999u);

    for (int trial = 0; trial < n_trial; ++trial) {
        sarray_random(&arr, lower, upper);

        struct subarray ret1, ret2;
        double time1, time2;

        TIMER_INIT(test1);
        TIMER_START(test1);
        ret1 = hybrid(&arr);
        TIMER_STOP(test1);
        time1 = TIMER_ELAPSED_USEC(test1);

        TIMER_INIT(test2);
        TIMER_START(test2);
        ret2 = divide_and_conquer(&arr);
        TIMER_STOP(test2);
        time2 = TIMER_ELAPSED_USEC(test2);

        int result = subarray_is_equal(&ret1, &ret2);
        if (result == 2) {
            printf("test %d pass\n", trial);
        } else if (result == 1) {
            printf("test %d failed - different range\n", trial);
            subarray_print(&ret1);
            subarray_print(&ret2);
        } else {
            printf("test %d failed - different sum\n", trial);
            subarray_print(&ret1);
            subarray_print(&ret2);
        }
        printf("\ttime - %.6f : %.6f\n", time1, time2);
    }

    sarray_free(&arr);
    return 0;
}
