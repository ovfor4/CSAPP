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
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, i2;
    int t0, t1, t2, t3, currentGroupI, currentGroupJ;
    const int blockSize = 4, larger = 2;

    for (i = 0; i < N / (blockSize * larger); i++)
    {
        for (j = 0; j < M / (blockSize * larger); j++)
        {
            // for (currentGroupI = 0; currentGroupI < larger*blockSize; currentGroupI+=blockSize)
            // {
            //     for (currentGroupJ = 0; currentGroupJ < larger*blockSize; currentGroupJ+=blockSize)
            //     {
            //         // move 4x4
            //         for (i2 = 0; i2 < blockSize; i2++)
            //         {
            //             t0 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 0];
            //             t1 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 1];
            //             t2 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 2];
            //             t3 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 3];

            //             B[j*larger*blockSize + currentGroupJ + 0][i*larger*blockSize + currentGroupI + i2] = t0;
            //             B[j*larger*blockSize + currentGroupJ + 1][i*larger*blockSize + currentGroupI + i2] = t1;
            //             B[j*larger*blockSize + currentGroupJ + 2][i*larger*blockSize + currentGroupI + i2] = t2;
            //             B[j*larger*blockSize + currentGroupJ + 3][i*larger*blockSize + currentGroupI + i2] = t3;
            //         }
            //     }
            // }

            // idk but I think it's magic somehow
            // remove to increase miss by kinda 20
            if (i == 7 && j == 7)
            {
                for (currentGroupI = 0; currentGroupI < larger * blockSize; currentGroupI += blockSize)
                {
                    for (currentGroupJ = 0; currentGroupJ < larger * blockSize; currentGroupJ += blockSize)
                    {
                        // move 4x4
                        for (i2 = 0; i2 < blockSize; i2++)
                        {
                            t0 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 0];
                            t1 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 1];
                            t2 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 2];
                            t3 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 3];

                            B[j * larger * blockSize + currentGroupJ + 0][i * larger * blockSize + currentGroupI + i2] = t0;
                            B[j * larger * blockSize + currentGroupJ + 1][i * larger * blockSize + currentGroupI + i2] = t1;
                            B[j * larger * blockSize + currentGroupJ + 2][i * larger * blockSize + currentGroupI + i2] = t2;
                            B[j * larger * blockSize + currentGroupJ + 3][i * larger * blockSize + currentGroupI + i2] = t3;
                        }
                    }
                }
                continue;
            }

            // A00 -> B00
            currentGroupI = 0;
            currentGroupJ = 0;
            for (i2 = 0; i2 < blockSize; i2++)
            {
                t0 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 0];
                t1 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 1];
                t2 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 2];
                t3 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 3];
                B[j * larger * blockSize + currentGroupJ + 0][i * larger * blockSize + currentGroupI + i2] = t0;
                B[j * larger * blockSize + currentGroupJ + 1][i * larger * blockSize + currentGroupI + i2] = t1;
                B[j * larger * blockSize + currentGroupJ + 2][i * larger * blockSize + currentGroupI + i2] = t2;
                B[j * larger * blockSize + currentGroupJ + 3][i * larger * blockSize + currentGroupI + i2] = t3;
            }

            // A01 -> temp B01
            currentGroupI = 0;
            currentGroupJ = 4;
            for (i2 = 0; i2 < blockSize; i2++)
            {
                t0 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 0];
                t1 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 1];
                t2 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 2];
                t3 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 3];
                B[j * larger * blockSize + currentGroupI + 0][i * larger * blockSize + currentGroupJ + i2] = t0;
                B[j * larger * blockSize + currentGroupI + 1][i * larger * blockSize + currentGroupJ + i2] = t1;
                B[j * larger * blockSize + currentGroupI + 2][i * larger * blockSize + currentGroupJ + i2] = t2;
                B[j * larger * blockSize + currentGroupI + 3][i * larger * blockSize + currentGroupJ + i2] = t3;
            }

            // // A10 -> B01
            // currentGroupI = 4; currentGroupJ = 0;
            // for (i2 = 0; i2 < blockSize; i2++) {
            //     t0 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 0];
            //     t1 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 1];
            //     t2 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 2];
            //     t3 = A[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 3];
            //     B[j*larger*blockSize + currentGroupJ + 0][i*larger*blockSize + currentGroupI + i2] = t0;
            //     B[j*larger*blockSize + currentGroupJ + 1][i*larger*blockSize + currentGroupI + i2] = t1;
            //     B[j*larger*blockSize + currentGroupJ + 2][i*larger*blockSize + currentGroupI + i2] = t2;
            //     B[j*larger*blockSize + currentGroupJ + 3][i*larger*blockSize + currentGroupI + i2] = t3;
            // }

            // // temp B01 -> B10
            // currentGroupI = 0; currentGroupJ = 4;
            // for (i2 = 0; i2 < blockSize; i2++) {
            //     t0 = B[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 0];
            //     t1 = B[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 1];
            //     t2 = B[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 2];
            //     t3 = B[i*larger*blockSize + currentGroupI + i2][j*larger*blockSize + currentGroupJ + 3];
            //     B[j*larger*blockSize + currentGroupJ + 0][i*larger*blockSize + currentGroupI + i2] = t0;
            //     B[j*larger*blockSize + currentGroupJ + 1][i*larger*blockSize + currentGroupI + i2] = t1;
            //     B[j*larger*blockSize + currentGroupJ + 2][i*larger*blockSize + currentGroupI + i2] = t2;
            //     B[j*larger*blockSize + currentGroupJ + 3][i*larger*blockSize + currentGroupI + i2] = t3;
            // }

            // B01 -> temp var, A10 -> B01, temp var -> B10
            currentGroupI = 4, currentGroupJ = 0;
            for (i2 = 0; i2 < blockSize; i2++)
            {
                t0 = B[j * larger * blockSize + currentGroupJ + i2]
                      [i * larger * blockSize + currentGroupI + 0];
                t1 = B[j * larger * blockSize + currentGroupJ + i2]
                      [i * larger * blockSize + currentGroupI + 1];
                t2 = B[j * larger * blockSize + currentGroupJ + i2]
                      [i * larger * blockSize + currentGroupI + 2];
                t3 = B[j * larger * blockSize + currentGroupJ + i2]
                      [i * larger * blockSize + currentGroupI + 3];

                B[j * larger * blockSize + currentGroupJ + i2]
                 [i * larger * blockSize + currentGroupI + 0] =
                     A[i * larger * blockSize + currentGroupI + 0]
                      [j * larger * blockSize + currentGroupJ + i2];

                B[j * larger * blockSize + currentGroupJ + i2]
                 [i * larger * blockSize + currentGroupI + 1] =
                     A[i * larger * blockSize + currentGroupI + 1]
                      [j * larger * blockSize + currentGroupJ + i2];

                B[j * larger * blockSize + currentGroupJ + i2]
                 [i * larger * blockSize + currentGroupI + 2] =
                     A[i * larger * blockSize + currentGroupI + 2]
                      [j * larger * blockSize + currentGroupJ + i2];

                B[j * larger * blockSize + currentGroupJ + i2]
                 [i * larger * blockSize + currentGroupI + 3] =
                     A[i * larger * blockSize + currentGroupI + 3]
                      [j * larger * blockSize + currentGroupJ + i2];

                B[j * larger * blockSize + currentGroupI + i2]
                 [i * larger * blockSize + currentGroupJ + 0] = t0;
                B[j * larger * blockSize + currentGroupI + i2]
                 [i * larger * blockSize + currentGroupJ + 1] = t1;
                B[j * larger * blockSize + currentGroupI + i2]
                 [i * larger * blockSize + currentGroupJ + 2] = t2;
                B[j * larger * blockSize + currentGroupI + i2]
                 [i * larger * blockSize + currentGroupJ + 3] = t3;
            }

            // A11 -> B11
            currentGroupI = 4;
            currentGroupJ = 4;
            for (i2 = 0; i2 < blockSize; i2++)
            {
                t0 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 0];
                t1 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 1];
                t2 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 2];
                t3 = A[i * larger * blockSize + currentGroupI + i2][j * larger * blockSize + currentGroupJ + 3];
                B[j * larger * blockSize + currentGroupJ + 0][i * larger * blockSize + currentGroupI + i2] = t0;
                B[j * larger * blockSize + currentGroupJ + 1][i * larger * blockSize + currentGroupI + i2] = t1;
                B[j * larger * blockSize + currentGroupJ + 2][i * larger * blockSize + currentGroupI + i2] = t2;
                B[j * larger * blockSize + currentGroupJ + 3][i * larger * blockSize + currentGroupI + i2] = t3;
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
