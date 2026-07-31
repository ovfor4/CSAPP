/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */

#define GENERAL_BLOCK_SIZE 8
#define CASE_32_32_BLOCK_SIZE 8
#define CASE_64_64_BLOCK_SIZE 4
#define CASE_64_64_MAGIC_NUMBER 2

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, i2, j2;
    int t0, t1, t2, t3, t4, t5, t6, t7;
    
    // FILE *fpA = fopen("matrices-a.csv", "w"), *fpB = fopen("matrices-b.csv", "w");
    // if (fpA == NULL || fpB == NULL) {
    //     perror("lol");
    //     return;
    // }
    // for (i = 0; i < N; i++)
    // {
    //     for (j = 0; j < M; j++)
    //     {    
    //         fprintf(fpA, "%d,", A[i][j]);
    //     }
    //     fprintf(fpA, "\n");
    // }
    // for (i = 0; i < M; i++)
    // {
    //     for (j = 0; j < N; j++)
    //     {    
    //         fprintf(fpB, "%d,", B[i][j]);
    //     }
    //     fprintf(fpB, "\n");
    // }
    // fclose(fpA); fclose(fpB);

    if (N == 32 && M == 32)
    {

        for (i = 0; i < N; i+=CASE_32_32_BLOCK_SIZE)
        {
            for (j = 0; j < M; j+=CASE_32_32_BLOCK_SIZE)
            {
                if (i == j && i != 24)
                {
                    for (i2 = 0; i2 < CASE_32_32_BLOCK_SIZE; i2++)
                    {
                        for (j2 = 0; j2 < CASE_32_32_BLOCK_SIZE; j2++)
                        {
                            t0 = A[i + i2][j + j2];
                            B[24 + j2][24 + i2] = t0;
                        }
                    }
                    for (i2 = 0; i2 < CASE_32_32_BLOCK_SIZE; i2++)
                    {
                        for (j2 = 0; j2 < CASE_32_32_BLOCK_SIZE; j2++)
                        {
                            t0 = B[24 + i2][24 + j2];
                            B[i + i2][j + j2] = t0;
                        }
                    }
                    continue;
                }


                for (i2 = 0; i2 < CASE_32_32_BLOCK_SIZE; i2++)
                {
                    t0 = A[i+i2][j+0];
                    t1 = A[i+i2][j+1];
                    t2 = A[i+i2][j+2];
                    t3 = A[i+i2][j+3];
                    t4 = A[i+i2][j+4];
                    t5 = A[i+i2][j+5];
                    t6 = A[i+i2][j+6];
                    t7 = A[i+i2][j+7];
                    B[j+0][i+i2] = t0;
                    B[j+1][i+i2] = t1;
                    B[j+2][i+i2] = t2;
                    B[j+3][i+i2] = t3;
                    B[j+4][i+i2] = t4;
                    B[j+5][i+i2] = t5;
                    B[j+6][i+i2] = t6;
                    B[j+7][i+i2] = t7;
                }
            }
        }
    }
    else if (N == 64 && M == 64) 
    {
        for (i = 0; i < N / (CASE_64_64_BLOCK_SIZE * CASE_64_64_MAGIC_NUMBER); i++)
        {
            for (j = 0; j < M / (CASE_64_64_BLOCK_SIZE * CASE_64_64_MAGIC_NUMBER); j++)
            {
                // idk but I think it's magic somehow
                // remove to increase miss by kinda 20
                if (i == 7 && j == 7)
                {
                    for (j2 = 0; j2 < CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE; j2 += CASE_64_64_BLOCK_SIZE)
                    {
                        for (t4 = 0; t4 < CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE; t4 += CASE_64_64_BLOCK_SIZE)
                        {
                            // move 4x4
                            for (i2 = 0; i2 < CASE_64_64_BLOCK_SIZE; i2++)
                            {
                                t0 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0];
                                t1 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1];
                                t2 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2];
                                t3 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3];

                                B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t0;
                                B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t1;
                                B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t2;
                                B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t3;
                            }
                        }
                    }
                    continue;
                }

                // A00 -> B00
                j2 = 0;
                t4 = 0;
                for (i2 = 0; i2 < CASE_64_64_BLOCK_SIZE; i2++)
                {
                    t0 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0];
                    t1 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1];
                    t2 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2];
                    t3 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3];
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t0;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t1;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t2;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t3;
                }

                // A01 -> temp B01
                j2 = 0;
                t4 = 4;
                for (i2 = 0; i2 < CASE_64_64_BLOCK_SIZE; i2++)
                {
                    t0 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0];
                    t1 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1];
                    t2 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2];
                    t3 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3];
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 0][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2] = t0;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 1][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2] = t1;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 2][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2] = t2;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 3][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2] = t3;
                }

                // B01 -> temp var, A10 -> B01, temp var -> B10
                j2 = 4, t4 = 0;
                for (i2 = 0; i2 < CASE_64_64_BLOCK_SIZE; i2++)
                {
                    t0 = B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                        [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 0];
                    t1 = B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                        [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 1];
                    t2 = B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                        [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 2];
                    t3 = B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                        [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 3];

                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 0] =
                        A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 0]
                        [j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2];

                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 1] =
                        A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 1]
                        [j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2];

                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 2] =
                        A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 2]
                        [j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2];

                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 3] =
                        A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + 3]
                        [j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + i2];

                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0] = t0;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1] = t1;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2] = t2;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2]
                    [i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3] = t3;
                }

                // A11 -> B11
                j2 = 4;
                t4 = 4;
                for (i2 = 0; i2 < CASE_64_64_BLOCK_SIZE; i2++)
                {
                    t0 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0];
                    t1 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1];
                    t2 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2];
                    t3 = A[i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2][j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3];
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 0][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t0;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 1][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t1;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 2][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t2;
                    B[j * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + t4 + 3][i * CASE_64_64_MAGIC_NUMBER * CASE_64_64_BLOCK_SIZE + j2 + i2] = t3;
                }
            }
        }
    }
    else
    {
        for (i = 0; i < N; i += GENERAL_BLOCK_SIZE)
        {
            for (j = 0; j < M; j += GENERAL_BLOCK_SIZE)
            {
                for (j2 = 0; j2 < GENERAL_BLOCK_SIZE && (j+j2) < M; j2++)
                {
                    for (i2 = 0; i2 < GENERAL_BLOCK_SIZE && (i+i2) < N; i2++)
                    {
                        t0 = A[i+i2][j+j2];
                        B[j+j2][i+i2] = t0;
                    }
                }
            }
        }
    }
}


/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
