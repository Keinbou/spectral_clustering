#include "matrix.c"
#include "spectral.c"

int main(){
    
    size_t s = 5;
    int max_sweeps = 3;
    int Nrun = max_sweeps*(s*(s-1))/2;
    double tol = 1.0E-22;

    int matrixType = 2;
    LOG(2, "matrixType:", int2String(matrixType));
    if (matrixType == 0 || matrixType == 1 || matrixType == 2)
    {
        s = 5;
    } else if (matrixType == 3 || matrixType == 4) {
        s = 2;
    }

    float (*L)[s][s] = malloc(sizeof(*L));
    float (*V)[s][s] = malloc(sizeof(*V));

    for ( int i=0; i<s; ++i )
    {
        for ( int j=0; j<s; ++j )
        {
            if (i==j) {(*L)[i][j] = 1.0; continue;}
            (*L)[i][j] = 0.5;
        }
    }
    if (matrixType == 0) // Toeplitz Matrix / Tridiagonal Matrix
    {
        read_csv_vla_f("matrices/toeplitz.csv", s, s, L);
    } else if (matrixType == 1) // Real symmetric Matrix with Zer0s
    {
        read_csv_vla_f("matrices/real_symmetric_5x5.csv", s, s, L);
    } else if (matrixType == 2) // Example of a Laplacian graph Matrix
    {
        read_csv_vla_f("matrices/graph_Laplacian.csv", s, s, L);
    } else if (matrixType == 3) // 2x2 real symmetric Matrix
    {
        read_csv_vla_f("matrices/real_symmetric_2x2.csv", s, s, L);
    } else if (matrixType == 4) // 2x2 diagonal Matrix
    {
        read_csv_vla_f("matrices/real_diagonal_2x2.csv", s, s, L);
    }
    printMatrix(s, s, L, s, s, "Original matrix:");
    Givens(s, Nrun, L, V, tol);
    printMatrix(s, s, L, s, s, "Eigenvalues:");
    printMatrix(s, s, V, s, s, "Eigenvectors:");/*
    float **res = spectral(-1, 300, "../datasets/three_clouds.csv", 10, 3);
    for (int i=0; i<300; ++i)
    {
        for (int j=0; j<3; ++j)
        {
            printf("%f ", res[i][j]);
        }
        p("");
    }*/
    return 0;
}

/*
EXPECTED OUTPUT:
//////////separator//////////
matrixType: 0

Original matrix:
0       1.000000 0.500000 0.250000 0.125000 0.062500 
1       0.500000 1.000000 0.500000 0.250000 0.125000 
2       0.250000 0.500000 1.000000 0.500000 0.250000 
3       0.125000 0.250000 0.500000 1.000000 0.500000 
4       0.062500 0.125000 0.250000 0.500000 1.000000 
5x5 matrix. 5x5 printed.

Eigenvalues:
0       2.261928 0.000000 -0.000000 -0.000000 0.000000 
1       0.000000 0.457209 -0.000000 -0.000000 0.000000 
2       -0.000000 -0.000000 0.690343 0.000000 0.000000 
3       -0.000000 -0.000000 0.000000 1.230291 0.000000 
4       0.000000 0.000000 0.000000 0.000000 0.360229 
5x5 matrix. 5x5 printed.

Eigenvectors:
0       0.360537 -0.435162 -0.561122 -0.557345 0.234850 
1       0.481984 0.557346 0.096981 -0.435162 -0.508219 
2       0.524794 -0.000000 0.592851 0.000000 0.610834 
3       0.481984 -0.557345 0.096981 0.435162 -0.508219 
4       0.360537 0.435162 -0.561123 0.557345 0.234850 
5x5 matrix. 5x5 printed.
//////////separator//////////
matrixType: 1
Original matrix:
0       4.000000 1.000000 0.000000 0.000000 0.000000 
1       1.000000 4.000000 1.000000 0.000000 0.000000 
2       0.000000 1.000000 4.000000 1.000000 0.000000 
3       0.000000 0.000000 1.000000 4.000000 1.000000 
4       0.000000 0.000000 0.000000 1.000000 4.000000 
5x5 matrix. 5x5 printed.

Eigenvalues:
0       5.000000 0.000000 -0.000000 0.000000 0.000000 
1       0.000000 3.000000 -0.000000 0.000000 -0.000000 
2       -0.000000 -0.000000 5.732050 -0.000000 0.000000 
3       0.000000 0.000000 -0.000000 2.267949 -0.000000 
4       0.000000 -0.000000 0.000000 -0.000000 4.000000 
5x5 matrix. 5x5 printed.

Eigenvectors:
0       0.500000 -0.500000 0.288675 -0.288675 0.577350 
1       0.500000 0.500000 0.500000 0.500000 -0.000000 
2       -0.000000 -0.000000 0.577350 -0.577350 -0.577350 
3       -0.500000 -0.500000 0.500000 0.500000 -0.000000 
4       -0.500000 0.500000 0.288675 -0.288675 0.577350 
5x5 matrix. 5x5 printed.
//////////separator//////////
matrixType: 2
Original matrix:
0       2.000000 -1.000000 -1.000000 0.000000 0.000000 
1       -1.000000 2.000000 0.000000 -1.000000 0.000000 
2       -1.000000 0.000000 2.000000 0.000000 -1.000000 
3       0.000000 -1.000000 0.000000 2.000000 -1.000000 
4       0.000000 0.000000 -1.000000 -1.000000 2.000000 
5x5 matrix. 5x5 printed.

Eigenvalues:
0       -0.000000 0.000000 0.000000 -0.000000 0.000000 
1       0.000000 3.618034 -0.000000 0.000000 -0.000000 
2       0.000000 -0.000000 1.381966 -0.000000 0.000000 
3       -0.000000 0.000000 -0.000000 1.381966 0.000000 
4       0.000000 -0.000000 0.000000 0.000000 3.618034 
5x5 matrix. 5x5 printed.

Eigenvectors:
0       0.447214 -0.272811 -0.222544 -0.592009 0.570591 
1       0.447214 0.556094 -0.631803 0.028711 -0.301264 
2       0.447214 -0.114676 0.494264 -0.394593 -0.621972 
3       0.447214 -0.626968 -0.167932 0.609753 -0.083136 
4       0.447214 0.458361 0.528016 0.348137 0.435781 
5x5 matrix. 5x5 printed.
//////////separator//////////
matrixType: 3
Original matrix:
0       2.000000 0.500000 
1       0.500000 6.000000 
2x2 matrix. 2x2 printed.

Eigenvalues:
0       1.938447 0.000000 
1       0.000000 6.061553 
2x2 matrix. 2x2 printed.

Eigenvectors:
0       0.992508 0.122183 
1       -0.122183 0.992508 
2x2 matrix. 2x2 printed.
//////////separator//////////
matrixType: 4
Original matrix:
0       2.000000 0.000000 
1       0.000000 6.000000 
2x2 matrix. 2x2 printed.

Eigenvalues:
0       2.000000 0.000000 
1       0.000000 6.000000 
2x2 matrix. 2x2 printed.

Eigenvectors:
0       1.000000 0.000000 
1       0.000000 1.000000 
2x2 matrix. 2x2 printed.
*/