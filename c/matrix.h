#pragma once
#include "file.c"
#include <time.h>
#include <math.h>

int UPPER_BOUND = 20;
size_t PRINT_N  =  8;
int MAX_SWEEPS  =  2;

// sorting
struct Dist_I_Pair
{
    float dist;
    int index;
};
typedef struct Dist_I_Pair dpair;
int compareDist(const void* a, const void* b);

// math corner
float _abs (float num);
float _pow2 (float num);
float _manhattan (size_t len, float *a, float *b);
float _random(int upper_bound);

// printing
void printMatrix(size_t row_i, size_t col_i, float (*A)[row_i][col_i], size_t r, size_t c, const char* title);
void printList(size_t len, float *list, size_t len_print, const char* title);

// read matrix
int read_csv_vla(const char* filename, size_t N, size_t D, float (*A)[N][D]);
// determine number of clusters (by pre-definded target values)
int k_clusters(size_t N, size_t D, float (*A)[N][D]);
// fill matrix with provided filling
void fill(size_t row_i, size_t col_i, float (*A)[row_i][col_i], float filling);
// trimming
void* trimLastCol(size_t row_i, size_t col_i, float (*A)[row_i][col_i]);
void* trimMatrix(size_t row_i, size_t col_i, size_t new_col_i, float (*A)[row_i][col_i], size_t from, size_t to);
// compute Weights matrix
void distances(size_t row_i, size_t col_i, float (*S)[row_i][col_i], float (*W)[row_i][row_i]);
void knnGraph(size_t size, float (*N)[size][size], float (*W)[size][size], int k);
// compute Weights matrix
void degreeMatrix(size_t size, float (*D)[size][size], float (*W)[size][size]);
void degreeMatrix_quick(size_t size, float (*D)[size][size], int k);
// compute Graph Laplacian (by substracting Degree from Weights)
void substractMatrices(size_t size, float (*L)[size][size],
                                    float (*D)[size][size],
                                    float (*W)[size][size]);
// determines, if the matrix is symmetrical
bool isSymmetrical(size_t size, float (*A)[size][size]);
// sorts eigenvectors by decreasing order of eigenvalues : return float* list of sorted eigenvalues
float* _sortEigenvectors(size_t size, float (*L)[size][size], float (*V)[size][size], int k);

// CONVERTING BETWEEN FLOAT** and VLA back and forth
float** VLA2floatPP(size_t row_i, size_t col_i, float (*A)[row_i][col_i]);
void floatPP2VLA(float **pp, size_t row_i, size_t col_i, float (*A)[row_i][col_i]);

// computing eigenvalues and eigenvectors of a REAL SYMMETRIC matrix. Purposed for Graph Laplacian.
void Givens(size_t size, int Nrun, float (*L)[size][size], float (*V)[size][size], double tol);

// KMeans clustering algorithm. Return a list of assigned clusters to each dot respectfully.
float* _kmeans(size_t row_i, size_t col_i, float (*V)[row_i][col_i], int k);

// testing
int read_csv_vla_f(const char* filename, size_t N, size_t D, float (*A)[N][D]);