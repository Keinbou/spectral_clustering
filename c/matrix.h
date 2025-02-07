#pragma once
#include "fileFunc.c"

struct Dist_I_Pair
{
    float dist;
    int index;
};

typedef struct Dist_I_Pair dpair;

// MATH CORNER
float _abs (float num);
float _pow2 (float num);
float manhattan (size_t len, float *a, float *b);

// MATRICES
void printMatrix(size_t row_i, size_t col_i, float (*A)[row_i][col_i], size_t r, size_t c, const char* title);
int read_csv_vla(const char* filename, size_t N, size_t D, float (*A)[N][D]);
int k_clusters(size_t N, size_t D, float (*A)[N][D]);
void fill(size_t row_i, size_t col_i, float (*A)[row_i][col_i], float filling);
void* trimLastCol(size_t row_i, size_t col_i, float (*A)[row_i][col_i]);
void distances(size_t row_i, size_t col_i, float (*S)[row_i][col_i], float (*W)[row_i][row_i]);
int compareDist(const void* a, const void* b);
void knnGraph(size_t size, float (*W)[size][size], int k);
void degreeMatrix(size_t size, float (*D)[size][size], float (*W)[size][size]);
void degreeMatrix_quick(size_t size, float (*D)[size][size], int k);
void substractMatrices(size_t size, float (*L)[size][size],
                                    float (*D)[size][size],
                                    float (*W)[size][size]);

// FOR TESTING
void toOut(size_t size, float (*W)[size][size], float **out);