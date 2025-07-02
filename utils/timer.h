#pragma once

/*
 * USAGE:
 * 
 * This header provides cross-platform high-resolution timing macros for 
 * performance measurement. The macros work on Windows (using QueryPerformanceCounter)
 * and Linux (using POSIX clock_gettime with CLOCK_MONOTONIC).
 * 
 * BASIC USAGE:
 * 
 *   #include "timer.h"  // Must be first include on Linux!
 *   
 *   int main() {
 *       TIMER_INIT(my_timer);           // Declare timer variables
 *       
 *       TIMER_START(my_timer);          // Start timing
 *       // ... code to measure ...
 *       TIMER_STOP(my_timer);           // Stop timing
 *       
 *       double seconds = TIMER_ELAPSED_SEC(my_timer);
 *       double millis  = TIMER_ELAPSED_MSEC(my_timer);
 *       double micros  = TIMER_ELAPSED_USEC(my_timer);
 *       
 *       printf("Elapsed: %.6f seconds\n", seconds);
 *       return 0;
 *   }
 * 
 * MULTIPLE TIMERS:
 * 
 *   TIMER_INIT(parse_timer);
 *   TIMER_INIT(sort_timer);
 *   
 *   TIMER_START(parse_timer);
 *   parse_data();
 *   TIMER_STOP(parse_timer);
 *   
 *   TIMER_START(sort_timer);
 *   sort_data();
 *   TIMER_STOP(sort_timer);
 *   
 *   printf("Parse: %.3f ms\n", TIMER_ELAPSED_MSEC(parse_timer));
 *   printf("Sort:  %.3f ms\n", TIMER_ELAPSED_MSEC(sort_timer));
 * 
 * NESTED TIMING:
 * 
 *   TIMER_INIT(total);
 *   TIMER_INIT(subtask);
 *   
 *   TIMER_START(total);
 *   
 *   TIMER_START(subtask);
 *   expensive_operation();
 *   TIMER_STOP(subtask);
 *   
 *   other_work();
 *   TIMER_STOP(total);
 * 
 * IMPORTANT NOTES:
 * 
 * - On Linux: This header MUST be included before any standard headers
 *   (like <time.h>, <stdio.h>, etc.) to ensure _POSIX_C_SOURCE is defined early
 * 
 * - Timer names become part of variable names, so use valid C identifiers
 *   (no spaces, no special characters except underscore)
 * 
 * - TIMER_INIT() must be called in the same scope where you use the timer
 * 
 * - Timers use monotonic clocks where available, so they're not affected 
 *   by system clock adjustments
 * 
 * - On unsupported platforms, all macros are no-ops that return 0.0
 * 
 * - Resolution: Windows provides sub-microsecond precision, Linux provides
 *   nanosecond precision (actual accuracy depends on hardware)
 */

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#define TIMER_INIT(name)                                    \
    LARGE_INTEGER name##_start, name##_end, name##_freq;    \
    QueryPerformanceFrequency(&name##_freq)

#define TIMER_START(name)           QueryPerformanceCounter(&name##_start)
#define TIMER_STOP(name)            QueryPerformanceCounter(&name##_end)
#define TIMER_ELAPSED_SEC(name)     \
    ((double)(name##_end.QuadPart - name##_start.QuadPart) / name##_freq.QuadPart)
#define TIMER_ELAPSED_MSEC(name)    \
    ((double)(name##_end.QuadPart - name##_start.QuadPart) * 1e3 / name##_freq.QuadPart)
#define TIMER_ELAPSED_USEC(name)    \
    ((double)(name##_end.QuadPart - name##_start.QuadPart) * 1e6 / name##_freq.QuadPart)

#elif defined(__linux__) // ^^^ defined(_WIN32) || defined(_WIN64) ^^^

#define _POSIX_C_SOURCE 199309L
#include <time.h>

#define TIMER_INIT(name)    struct timespec name##_start, name##_end
#define TIMER_START(name)   clock_gettime(CLOCK_MONOTONIC, &name##_start)
#define TIMER_STOP(name)    clock_gettime(CLOCK_MONOTONIC, &name##_end)

#define TIMER_ELAPSED_SEC(name)                                 \
    ((double)(name##_end.tv_sec - name##_start.tv_sec) +        \
     (double)(name##_end.tv_nsec - name##_start.tv_nsec) / 1e9)
#define TIMER_ELAPSED_MSEC(name)                                \
    ((double)(name##_end.tv_sec - name##_start.tv_sec) * 1e3 +  \
     (double)(name##_end.tv_nsec - name##_start.tv_nsec) / 1e6)
#define TIMER_ELAPSED_USEC(name)                                \
    ((double)(name##_end.tv_sec - name##_start.tv_sec) * 1e6 +  \
     (double)(name##_end.tv_nsec - name##_start.tv_nsec) / 1e3)

#else // ^^^ defined(__linux__) ^^^

#define TIMER_INIT(name)            do {} while (0)
#define TIMER_START(name)           do {} while (0)
#define TIMER_STOP(name)            do {} while (0)
#define TIMER_ELAPSED_SEC(name)     0.0
#define TIMER_ELAPSED_MSEC(name)    0.0
#define TIMER_ELAPSED_USEC(name)    0.0

#endif