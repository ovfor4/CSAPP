#define CP_4_BY_8(base_i, base_j, tmp) \
do \
{ \
for (tmp = 0; tmp < 4; tmp++) \
{ \
    t0 = A[base_i + tmp][base_j + 0]; \
    t1 = A[base_i + tmp][base_j + 1]; \
    t2 = A[base_i + tmp][base_j + 2]; \
    t3 = A[base_i + tmp][base_j + 3]; \
    t4 = A[base_i + tmp][base_j + 4]; \
    t5 = A[base_i + tmp][base_j + 5]; \
    t6 = A[base_i + tmp][base_j + 6]; \
    t7 = A[base_i + tmp][base_j + 7]; \
    B[base_i + tmp][base_j + 0] = t0; \
    B[base_i + tmp][base_j + 1] = t1; \
    B[base_i + tmp][base_j + 2] = t2; \
    B[base_i + tmp][base_j + 3] = t3; \
    B[base_i + tmp][base_j + 4] = t4; \
    B[base_i + tmp][base_j + 5] = t5; \
    B[base_i + tmp][base_j + 6] = t6; \
    B[base_i + tmp][base_j + 7] = t7; \
} \
} while (0)


#define SWAP(x, y, tmp) \
do \
{ \
tmp = x; x = y; y = tmp; \
} while (0)


#define B_SMALL_TRANSPOSE(base_i, base_j, tmp) \
do \
{ \
SWAP(B[base_i + 0][base_j + 1], B[base_i + 1][base_j + 0], tmp); \
SWAP(B[base_i + 0][base_j + 2], B[base_i + 2][base_j + 0], tmp); \
SWAP(B[base_i + 0][base_j + 3], B[base_i + 3][base_j + 0], tmp); \
 \
SWAP(B[base_i + 1][base_j + 2], B[base_i + 2][base_j + 1], tmp); \
SWAP(B[base_i + 1][base_j + 3], B[base_i + 3][base_j + 1], tmp); \
 \
SWAP(B[base_i + 2][base_j + 3], B[base_i + 3][base_j + 2], tmp); \
} while (0)


#define B_SWAP_4_BY_4(i1, j1, i2, j2, tmp) \
do \
{ \
for (tmp = 0; tmp < 4; tmp++) \
{ \
    t0 = B[i1 + tmp][j1 + 0]; \
    t1 = B[i1 + tmp][j1 + 1]; \
    t2 = B[i1 + tmp][j1 + 2]; \
    t3 = B[i1 + tmp][j1 + 3]; \
    \
    t4 = B[i2 + tmp][j2 + 0]; \
    t5 = B[i2 + tmp][j2 + 1]; \
    t6 = B[i2 + tmp][j2 + 2]; \
    t7 = B[i2 + tmp][j2 + 3]; \
    \
    B[i2 + tmp][j2 + 0] = t0; \
    B[i2 + tmp][j2 + 1] = t1; \
    B[i2 + tmp][j2 + 2] = t2; \
    B[i2 + tmp][j2 + 3] = t3; \
    \
    B[i1 + tmp][j1 + 0] = t4; \
    B[i1 + tmp][j1 + 1] = t5; \
    B[i1 + tmp][j1 + 2] = t6; \
    B[i1 + tmp][j1 + 3] = t7; \
} \
} while (0)
