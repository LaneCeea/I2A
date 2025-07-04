#include "sheap.h"

static inline
size_t _parent(size_t i) {
    return (i == 0) ? 0 : (i - 1) / 2;
}

static inline
size_t _left(size_t i) {
    return i * 2 + 1;
}

static inline
size_t _right(size_t i) {
    return i * 2 + 2;
}

static inline
void _swap(sarray* this, size_t lhs, size_t rhs) {
    sarray_data_t temp = this->data[lhs];
    this->data[lhs] = this->data[rhs];
    this->data[rhs] = temp;
}

void max_heapify(sarray* this, size_t root, size_t heap_size) {
    for (;;) {
        size_t max = root;
        size_t l = _left(root);
        size_t r = _right(root);

        // find maximum among root, left, and right
        if (l < heap_size && this->data[max] < this->data[l]) {
            max = l;
        }
        if (r < heap_size && this->data[max] < this->data[r]) {
            max = r;
        }
        
        if (max == root) {
            return;
        }

        // float down
        _swap(this, root, max);
        root = max;
    }
}

void build_max_heap(sarray* this) {
    // for i = this->size / 2 - 1 downto 0
    size_t i = this->size / 2;
    while (i > 0) {
        --i;
        max_heapify(this, i, this->size);
    }
}

bool is_heap(const sarray* this) {
    for (size_t i = 1; i < this->size; ++i) {
        if (this->data[_parent(i)] < this->data[i]) {
            return false;
        }
    }
    return true;
}