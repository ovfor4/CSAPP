#pragma once

#define LOAD_ROW_4(src, base_i, base_j) \
do \
{ \
t0 = src[base_i][base_j + 0]; \
t1 = src[base_i][base_j + 1]; \
t2 = src[base_i][base_j + 2]; \
t3 = src[base_i][base_j + 3]; \
} while (0)

#define STORE_ROW_4(dst, base_i, base_j) \
do \
{ \
dst[base_i][base_j + 0] = t0; \
dst[base_i][base_j + 1] = t1; \
dst[base_i][base_j + 2] = t2; \
dst[base_i][base_j + 3] = t3; \
} while (0)
