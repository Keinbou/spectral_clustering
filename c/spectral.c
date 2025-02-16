#include "matrix.c"

// Create .so file to import to Python
// gcc -c spectral.c
// gcc *.o -dynamiclib -o c_code.so

float* assign_clusters(size_t row_i, size_t col_i, float** spectral_embedding, int k)
{
    float (*A)[row_i][col_i] = malloc(sizeof(*A));
    floatPP2VLA(spectral_embedding, row_i, col_i, A);
    float *clustered_list = _kmeans(row_i, col_i, A, k);
    printList(row_i, clustered_list, PRINT_N, "ASSIGNED CLUSTERS LIST:");

    free(A);
    return clustered_list;
}

float** spectral_embedding(int data_len, int read_n_rows, char *path, int knn, int k)
{
    int    NRUN = MAX_SWEEPS * ((read_n_rows * (read_n_rows-1))/2);
    double TOL  = 1.0E-22;
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

    if ( REQ_DATA_LEN < 2 )
        errmsg("The width of data must be at least 2: 1 for x, 1 for y.");
    else if ( REQ_DATA_LEN > TRU_DATA_LEN )
        errmsg("The width of the data in the .csv file is less than requested.");
    if ( REQ_N_ROWS < 0 )
        errmsg("Cannot read negative quantity of rows.");
    else if ( REQ_N_ROWS > TRU_N_ROWS )
        errmsg("The length of the data in the .csv file is less than requested.");
    if ( k>=REQ_N_ROWS ) errmsg("The number of clusters must be less than the number of dots.");

    size_t Y_LEN    = 1;
    size_t X_LEN    = REQ_DATA_LEN - Y_LEN;
    size_t DATA_LEN = Y_LEN + X_LEN;

    float (*A)[REQ_N_ROWS][DATA_LEN] = malloc(sizeof(*A));

    size_t row_i = REQ_N_ROWS;
    size_t col_i = DATA_LEN;

    int RETURNED_N_OF_ROWS = read_csv_vla(path, row_i, col_i, A);
    if ( k==-1 ) k = k_clusters(row_i, col_i, A);
    if(RETURNED_N_OF_ROWS){}
    else
    {
        p("WARNING : read 0 rows from .csv file.");
    }
    printMatrix(row_i, col_i, A, PRINT_N, -1, "ORIGINAL DATASET: (x,y,target)");
    p("k of clusters: %i", k);
    p(""); 

    float (*S)[row_i][X_LEN] = trimLastCol(row_i, col_i, A);
    printMatrix(row_i, X_LEN, S, PRINT_N, -1, "TRIMMED DATASET: (x,y)");

    float (*N)[row_i][row_i] = malloc(sizeof(*N));

    size_t sqr_size = row_i;
    distances(row_i, X_LEN, S, N);
    printMatrix(sqr_size, sqr_size, N, PRINT_N, PRINT_N, "DISTANCES MATRIX:");

    float (*W)[sqr_size][sqr_size] = malloc(sizeof(*W));

    knnGraph(sqr_size, N, W, knn);
    printMatrix(sqr_size, sqr_size, W, 1, -1, "WEIGHTS MATRIX: (one row of)");

    float (*D)[sqr_size][sqr_size] = malloc(sizeof(*D));
    degreeMatrix(sqr_size, D, W);
    printMatrix(sqr_size, sqr_size, D, PRINT_N, PRINT_N, "DEGREE MATRIX:");

    float (*L)[sqr_size][sqr_size] = malloc(sizeof(*L));
    substractMatrices(sqr_size, L, D, W);
    printMatrix(sqr_size, sqr_size, L, PRINT_N, PRINT_N, "GRAPH LAPLACIAN MATRIX:");

    float (*V)[sqr_size][sqr_size] = malloc(sizeof(*V));
    Givens(sqr_size, NRUN, L, V, TOL);
    printMatrix(sqr_size, sqr_size, L, PRINT_N, PRINT_N, "EIGENVALUES:");
    printMatrix(sqr_size, sqr_size, V, PRINT_N, PRINT_N, "EIGENVECTORS MATRIX: (in the columns)");

    float *evalues = _sortEigenvectors(sqr_size, L, V, k);
    printList(sqr_size, evalues, PRINT_N, "SORTED EIGENVALUES:");
    printMatrix(sqr_size, sqr_size, V, PRINT_N, PRINT_N, "SORTED EIGENVECTORS MATRIX:");

    // Rayleigh principle: the solution is [2;k+1] eigenvectors.
    // So the first eigenvector is trimmed.
    size_t from = 1, to = k+1, k_col_i = k;
    float (*T)[sqr_size][k] = trimMatrix(sqr_size, sqr_size, k_col_i, V, from, to);
    printMatrix(sqr_size, k, T, PRINT_N, -1, "TRIMMED EIGENVECTORS [2:k] MATRIX:");

    float **spectral_embedding = VLA2floatPP(sqr_size, k_col_i, T);

    free(A);free(S);free(N);free(W);free(D);free(L);free(V);free(T);

    return spectral_embedding;
}

float *kmeans(int data_len, int read_n_rows, char *path, size_t k)
{
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

    if ( REQ_DATA_LEN < 2 )
        errmsg("The width of data must be at least 2: 1 for x, 1 for y.");
    else if ( REQ_DATA_LEN > TRU_DATA_LEN )
        errmsg("The width of the data in the .csv file is less than requested.");
    if ( REQ_N_ROWS < 0 )
        errmsg("Cannot read negative quantity of rows.");
    else if ( REQ_N_ROWS > TRU_N_ROWS )
        errmsg("The length of the data in the .csv file is less than requested.");
    if ( k>=REQ_N_ROWS ) errmsg("The number of clusters must be less than the number of dots.");

    size_t Y_LEN    = 1;
    size_t X_LEN    = REQ_DATA_LEN - Y_LEN;
    size_t DATA_LEN = Y_LEN + X_LEN;

    float (*A)[REQ_N_ROWS][DATA_LEN] = malloc(sizeof(*A));

    size_t rowi = REQ_N_ROWS;
    size_t coli = DATA_LEN;

    int RETURNED_N_OF_ROWS = read_csv_vla(path, rowi, coli, A);
    if ( k==-1 ) k = k_clusters(rowi, coli, A);
    if(RETURNED_N_OF_ROWS){}
    else
    {
        p("WARNING : read 0 rows from .csv file.");
    }
    printMatrix(rowi, coli, A, PRINT_N, -1, "ORIGINAL DATASET: (x,y,target)");
    p("k of clusters: %i", k);
    p(""); 

    float (*V)[rowi][X_LEN] = trimLastCol(rowi, coli, A);
    printMatrix(rowi, X_LEN, V, PRINT_N, -1, "TRIMMED DATASET: (x,y)");
    return _kmeans(read_n_rows, X_LEN, V, k);
}

void free_floatP(float *out)
{
    free(out);
}

void free_floatPP(float **out)
{
    free(out);
}