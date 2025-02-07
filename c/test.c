#include "matrix.c"
#include "funcs.c"

int main(){
    float **res = spectral(-1, -1, "../datasets/file_1.csv", 10);
    return 0;
}