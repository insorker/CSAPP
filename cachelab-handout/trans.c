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
void trans61x67(int A[67][61], int B[61][67]);

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
			trans61x67(A, B);
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
#define demo0 0
#define demo1 0
#define demo2 0
#define elegant 0
#define fastest32 1
	
#if demo0
	int i, j;

	for (i = 0; i < 8; i ++ )
		for (j = 0; j < 8; j ++ )
			*(pb + j * n + i) = *(pa + i * n + j);
#endif

#if demo1
	int i;
	int x0, x1, x2, x3, x4, x5, x6, x7;

	for (i = 0; i < s; i ++ ) {
		x0 = *(pa + i * n + 0);
		x1 = *(pa + i * n + 1);
		x2 = *(pa + i * n + 2);
		x3 = *(pa + i * n + 3);
		x4 = *(pa + i * n + 4);
		x5 = *(pa + i * n + 5);
		x6 = *(pa + i * n + 6);
		x7 = *(pa + i * n + 7);

		*(pb + 0 * n + i) = x0;
		*(pb + 1 * n + i) = x1;
		*(pb + 2 * n + i) = x2;
		*(pb + 3 * n + i) = x3;
		*(pb + 4 * n + i) = x4;
		*(pb + 5 * n + i) = x5;
		*(pb + 6 * n + i) = x6;
		*(pb + 7 * n + i) = x7;
	}
#endif

#if demo2
	int i, j;
	int x[8];

	for (i = 0; i < s; i ++ ) {
		for (j = 0; j < s; j ++ )
			x[j] = *(pa + i * n + j);

		for (j = 0; j < s; j ++ )
			*(pb + j * n + i) = x[j];
	}
#endif

#if elegant
	int i, j, tmp;
	int x[8];

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
#endif

#if fastest32
	int i, j, tmp;
	int x0, x1, x2, x3, x4, x5, x6, x7;

	for (i = 0; i < s; i ++ ) {
		x0 = *(pa + i * n + 0);
		x1 = *(pa + i * n + 1);
		x2 = *(pa + i * n + 2);
		x3 = *(pa + i * n + 3);
		x4 = *(pa + i * n + 4);
		x5 = *(pa + i * n + 5);
		x6 = *(pa + i * n + 6);
		x7 = *(pa + i * n + 7);

		*(pb + i * n + 0) = x0;
		*(pb + i * n + 1) = x1;
		*(pb + i * n + 2) = x2;
		*(pb + i * n + 3) = x3;
		*(pb + i * n + 4) = x4;
		*(pb + i * n + 5) = x5;
		*(pb + i * n + 6) = x6;
		*(pb + i * n + 7) = x7;

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
#endif
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
	int i, j, k, tmp;
	int x0, x1, x2, x3, x4, x5, x6, x7;

	for (i = 0; i < 64; i += 8)
		for (j = 0; j < 64; j += 8) {
			for (k = 0; k < 4; k ++ ) {
				/* A left top */
				x0 = A[i + k][j + 0];
				x1 = A[i + k][j + 1];
				x2 = A[i + k][j + 2];
				x3 = A[i + k][j + 3];
				/* A right top */
				x4 = A[i + k][j + 4];
				x5 = A[i + k][j + 5];
				x6 = A[i + k][j + 6];
				x7 = A[i + k][j + 7];

				/* B left top */
				B[j + 0][i + k] = x0;
				B[j + 1][i + k] = x1;
				B[j + 2][i + k] = x2;
				B[j + 3][i + k] = x3;
				/* B rjght top */
				B[j + 0][i + k + 4] = x4;
				B[j + 1][i + k + 4] = x5;
				B[j + 2][i + k + 4] = x6;
				B[j + 3][i + k + 4] = x7;
			}

			for (k = 0; k < 4; k ++ ) {
				/* A left bottom */
				x0 = A[i + 4][j + k];
				x1 = A[i + 5][j + k];
				x2 = A[i + 6][j + k];
				x3 = A[i + 7][j + k];
				/* A right bottom */
				x4 = A[i + 4][j + k + 4];
				x5 = A[i + 5][j + k + 4];
				x6 = A[i + 6][j + k + 4];
				x7 = A[i + 7][j + k + 4];

				/* B left top */
				tmp = B[j + k][i + 4]; B[j + k][i + 4] = x0; x0 = tmp;
				tmp = B[j + k][i + 5]; B[j + k][i + 5] = x1; x1 = tmp;
				tmp = B[j + k][i + 6]; B[j + k][i + 6] = x2; x2 = tmp;
				tmp = B[j + k][i + 7]; B[j + k][i + 7] = x3; x3 = tmp;
				/* B left top */
				B[j + k + 4][i + 0] = x0; B[j + k + 4][i + 4 + 0] = x4;
				B[j + k + 4][i + 1] = x1; B[j + k + 4][i + 4 + 1] = x5;
				B[j + k + 4][i + 2] = x2; B[j + k + 4][i + 4 + 2] = x6;
				B[j + k + 4][i + 3] = x3; B[j + k + 4][i + 4 + 3] = x7;
			}
		}
}

char trans61x67_desc[] = "61x67";
void trans61x67(int A[67][61], int B[61][67]) {
	int i, j, k, h, s = 17;

	for (i = 0; i < 67; i += s)
		for (j = 0; j < 61; j += s) {
			for (k = i; k < i + s && k < 67; k ++ )
				for (h = j; h < j + s && h < 61; h ++ )
					B[h][k] = A[k][h];
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

