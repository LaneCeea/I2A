#pragma once

#include <stdint.h>

/* Seed the global PCG state.  Both values are user-supplied “non-secret” seeds.
 * Call once at program start (or whenever you want to reseed).                */
void pcg32_seed(uint64_t initstate, uint64_t initseq);

/* Return a discrete-uniform integer in the inclusive range [lower, upper].
 * If upper < lower, the function swaps the arguments for you.
 * Requires that (uint64_t)upper - (uint64_t)lower + 1 fits in 32 bits.         */
int32_t pcg32_uniform_lemire(int32_t lower, int32_t upper);