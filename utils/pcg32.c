#include "pcg32.h"
#include <assert.h>

/* ─────────── 1. hidden global state ─────────── */
static uint64_t pcg32_state = 0u;
static uint64_t pcg32_inc   = 0u;   /* must stay odd */

/* ─────────── 2. core PCG-XSH-RR step ─────────── */
static inline uint32_t pcg32_random_internal(void)
{
    const uint64_t multiplier = 6364136223846793005ULL;
    uint64_t oldstate = pcg32_state;
    pcg32_state = oldstate * multiplier + pcg32_inc;

    uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot        = (uint32_t)(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

/* ─────────── 3. public seeding API ─────────── */
void pcg32_seed(uint64_t initstate, uint64_t initseq)
{
    pcg32_state = 0u;
    pcg32_inc   = (initseq << 1u) | 1u;      /* force odd */
    pcg32_random_internal();                 /* advance once */
    pcg32_state += initstate;
    pcg32_random_internal();                 /* advance again */
}

/* ─────────── 4. Lemire multiply-high sampler ─────────── */
int32_t pcg32_uniform_lemire(int32_t lower, int32_t upper)
{
    /* Make the range canonical: lower ≤ upper */
    if (upper < lower) {
        int32_t tmp = lower;
        lower = upper;
        upper = tmp;
    }

    /* Calculate span = (upper - lower + 1) as unsigned 32-bit. 
       The caller must ensure the span fits (i.e. range ≤ 2^32-1).            */
    uint32_t span = (uint32_t)((uint64_t)upper - (uint64_t)lower + 1u);
    assert(span != 0);                       /* guards against full 2^32 span */

    while (1) {
        uint32_t r        = pcg32_random_internal();
        uint64_t product  = (uint64_t)r * span;
        uint32_t hi       = (uint32_t)(product >> 32);

        /* Fast path:  ~100 % of the time we exit here */
        if ((uint32_t)product >= span)
            return (int32_t)(hi + lower);

        /* Rare bias-correction branch */
        uint32_t thresh = -span % span;
        if ((uint32_t)product >= thresh)
            return (int32_t)(hi + lower);
        /* else: generate a new r and try again */
    }
}
