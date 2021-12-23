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
void transsxs(int s, int n, int* pa, int* pb);
void trans32x32(int A[32][32], int B[32][32]);
void trans64x64(int A[64][64], int B[64][64]);

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
    switch (M) {
		case 8:
			transsxs(8, 8, *A, *B);
			break;
		case 32:
			trans32x32(A, B);
			break;
		case 64:
			trans64x64(A, B);
			break;
		case 61:
			break;
	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
char transsxs_desc[] = "Specify s N into 8";
void transsxs(int s, int n, int* pa, int* pb)
{
	int i, j, tmp;
	int x[8];
	
	/* for (i = 0; i < 8; i ++ ) */
	/*     for (j = 0; j < 8; j ++ ) */
	/*         *(pb + j * n + i) = *(pa + i * n + j); */

	/* for (i = 0; i < s; i ++ ) { */
	/*     for (j = 0; j < s; j ++ ) */
	/*         x[j] = *(pa + i * n + j); */
    /*  */
	/*     for (j = 0; j < s; j ++ ) */
	/*         *(pb + j * n + i) = x[j]; */
	/* } */
	for (i = 0; i < s; i ++ ) {
		for (j = 0; j < s; j ++ )
			x[j] = *(pa + i * n + j);

		for (j = 0; j < s; j ++ )
			*(pb + i * n + j) = x[j];

		for (j = 0; 2 * j < i; j ++ ) {
			tmp = *(pb + j * n + i - j);
			*(pb + j * n + i - j) = *(pb + (i - j) * n + j);
			*(pb + (i - j) * n + j) = tmp;
		}
	}
	for (i = s; i < 2 * s; i ++ ) {
		for (j = i - s + 1; 2 * j < i; j ++ ) {
			tmp = *(pb + j * n + i - j);
			*(pb + j * n + i - j) = *(pb + (i - j) * n + j);
			*(pb + (i - j) * n + j) = tmp;
		}
	}
}

char trans32x32_desc[] = "32x32";
void trans32x32(int A[32][32], int B[32][32]) {
	int i, j, s = 8;
	int* pa, * pb;

	for (i = 0; i < 32; i += s)
		for (j = 0; j < 32; j += s) {
			pa = *(A + i) + j;
			pb = *(B + j) + i;
			transsxs(s, 32, pa, pb);
		}
}

char trans64x64_desc[] = "64x64";
void trans64x64(int A[64][64], int B[64][64]) {
	int i, j, s = 4;
	int* pa, * pb;

	for (i = 0; i < 64; i += s)
		for (j = 0; j < 64; j += s) {
			pa = *(A + i) + j;
			pb = *(B + j) + i;
			transsxs(s, 64, pa, pb);
		}
}
/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
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
	/* registerTransFunction(trans, trans_desc); */

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

