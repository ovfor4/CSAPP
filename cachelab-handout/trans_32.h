#pragma once

#ifndef CASE_32_32_BLOCK_SIZE
#define CASE_32_32_BLOCK_SIZE 8
#endif

#define B_32_8_BY_8_TRANSPOSE(base_i, base_j, tmp) \
do \
{ \
for (tmp = 0; tmp < CASE_32_32_BLOCK_SIZE; tmp++) \
{ \
    t0 = A[base_i + tmp][base_j + 0]; \
    t1 = A[base_i + tmp][base_j + 1]; \
    t2 = A[base_i + tmp][base_j + 2]; \
    t3 = A[base_i + tmp][base_j + 3]; \
    t4 = A[base_i + tmp][base_j + 4]; \
    t5 = A[base_i + tmp][base_j + 5]; \
    t6 = A[base_i + tmp][base_j + 6]; \
    t7 = A[base_i + tmp][base_j + 7]; \
    B[base_j + 0][base_i + tmp] = t0; \
    B[base_j + 1][base_i + tmp] = t1; \
    B[base_j + 2][base_i + tmp] = t2; \
    B[base_j + 3][base_i + tmp] = t3; \
    B[base_j + 4][base_i + tmp] = t4; \
    B[base_j + 5][base_i + tmp] = t5; \
    B[base_j + 6][base_i + tmp] = t6; \
    B[base_j + 7][base_i + tmp] = t7; \
} \
} while (0)
