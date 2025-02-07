#pragma once
#include "matrix.c"

// Create .so file to import to Python
// gcc -c funcs.c
// gcc *.o -dynamiclib -o c_code.so

float** spectral(int data_len, int read_n_rows, char *path, int knn){

    size_t PRINT_N = 5;
    // len of x    - arg 1
    // len of rows - arg 2
    // path        - arg 3
    const char* PATH= path;
    size_t TRU_DATA_LEN = dataWidth(PATH);
    size_t REQ_DATA_LEN = data_len==-1 ? TRU_DATA_LEN : data_len; //atoi() : char* to int
    size_t TRU_N_ROWS   = dataDepth(PATH);
    size_t REQ_N_ROWS   = read_n_rows==-1 ? TRU_N_ROWS: read_n_rows; //atoi() : char* to int

    p("");
    p("REQ_DATA_LEN: %zu \tTRU_DATA_LEN: %zu", REQ_DATA_LEN, TRU_DATA_LEN);
    p("REQ_N_ROWS  : %zu \tTRU_N_ROWS  : %zu+1", REQ_N_ROWS, TRU_N_ROWS);
    p("");
    //return 0;

    if ( REQ_DATA_LEN < 2 )
    {
        errmsg("The width of data must be at least 2: 1 for x, 1 for y.");
    } else if ( REQ_DATA_LEN > TRU_DATA_LEN ){
        errmsg("The width of the data in the .csv file is less than requested.");
    }
    if ( REQ_N_ROWS < 0 )
    {
        errmsg("Cannot read negative quantity of rows.");
    } else if ( REQ_N_ROWS > TRU_N_ROWS ){
        errmsg("The length of the data in the .csv file is less than requested.");
    }
    size_t Y_LEN    = 1;
    size_t X_LEN    = REQ_DATA_LEN - Y_LEN;
    size_t DATA_LEN = Y_LEN + X_LEN;

    float (*A)[REQ_N_ROWS][DATA_LEN] = malloc(sizeof(*A));

    size_t rowi = REQ_N_ROWS;
    size_t coli = DATA_LEN;

    int RETURNED_N_OF_ROWS = read_csv_vla(path, rowi, coli, A);
    int k = k_clusters(rowi, coli, A);
    if(RETURNED_N_OF_ROWS){}
    else
    {
        p("WARNING : read 0 rows from .csv file.");
    }
    printMatrix(rowi, coli, A, PRINT_N, -1, "ORIGINAL DATASET: (x,y,target)");
    p("k of clusters: %i", k);
    p(""); 

    float (*S)[rowi][X_LEN] = trimLastCol(rowi, coli, A);
    printMatrix(rowi, X_LEN, S, PRINT_N, -1, "TRIMMED DATASET: (x,y)");

    float (*W)[rowi][rowi] = malloc(sizeof(*W));

    distances(rowi, X_LEN, S, W);
    printMatrix(rowi, rowi, W, PRINT_N, PRINT_N, "DISTANCES MATRIX:");

    knnGraph(rowi, W, knn);
    printMatrix(rowi, rowi, W, 1, -1, "WEIGHTS MATRIX: (one row of)");

    float **out = malloc(rowi*sizeof(float*));
    for (int i=0; i<rowi; ++i)
    {
        out[i] = malloc(rowi*sizeof(*out[i]));
    }

    toOut(rowi, W, out);

    float (*D)[rowi][rowi] = malloc(sizeof(*D));
    fill(rowi, rowi, D, 0);
    degreeMatrix_quick(rowi, D, knn);
    printMatrix(rowi, rowi, D, PRINT_N, PRINT_N, "DEGREE MATRIX:");

    float (*L)[rowi][rowi] = malloc(sizeof(*L));
    substractMatrices(rowi, L, D, W);
    printMatrix(rowi, rowi, D, PRINT_N, PRINT_N, "GRAPH LAPLACIAN MATRIX:");

    // perform spectral clustering on list

    free(A);
    free(S);
    free(W);
    return out;
}

void free_floatPP(float **out)
{
    free(out);
}