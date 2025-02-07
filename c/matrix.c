#pragma once
#include "matrix.h"

float _abs (float num)
{
    return num<0 ? -num : num;
}

float _pow2 (float num)
{
    return num*num;
}

float manhattan (size_t len, float *a, float *b)
{
    float dist = 0;
    for( int i=0;i<len;++i )
    {
        dist+=_abs(a[i]-b[i]);
    }
    return dist;
}

void printMatrix(size_t row_i, size_t col_i, float (*A)[row_i][col_i], size_t r, size_t c, const char* title)
// pass -1 or >row_i as n to read all rows
{
    p("%s", title);
    if (r==-1 || r>row_i) r=row_i;
    if (c==-1 || c>col_i) c=col_i;
    char * str = "";
    for(size_t i=0; i<row_i; ++i) {
        if( i==r )
        {
            break;
        }
        str = concat(str, printfStr("%i\t", i));
        for(size_t j=0; j<col_i; ++j)
        {
            if( j==c )
            {
                break;
            }
            str = concat(str, float2String((*A)[i][j]));
            str = concat(str, " ");
        }
        str = concat(str, "\n");
    }
    printf("%s", str);
    p("%zux%zu matrix. %zux%zu printed.", row_i, col_i, r, c);
    p("");
}

int read_csv_vla(const char* filename, size_t N, size_t D, float (*A)[N][D])
{
    FILE* f=fopen(filename, "r");
    if(f==NULL){
        errmsg("There is no file to be opened.");
    }
    size_t dur=0;
    char* line=NULL;
    ssize_t len;

    int flag = 0;
    size_t i = 0;
    while( (len=getline(&line, &dur, f))!=-1 ){
        if(!flag){ //skip the first line
            flag=1;
            continue;
        }
        if(i>=N){
            return N;
        }
        char *str, *token;
        str=strdup(line);
        size_t j=0;
        while( (token=strsep(&str, ",")) ){
            if(strcmp(token, "")==0) continue;
            if ( j<(D-1) )
            {
                (*A)[i][j] = atof(token);
                ++j;
            } else
            {
                (*A)[i][D-1] = atoi(token);
                break;
            }
        }
        ++i;
    }
    if(i<N){
        printf("There are less rows in the dataset then requested.\n");
        return i;
    }
    return N;
}

int k_clusters(size_t N, size_t D, float (*A)[N][D])
{
    int k=-1; // cluster numeration start from 0, which can be the max value
    for(size_t i=0; i<N; ++i) {
        float t = (*A)[i][D-1];
        if(t>k)
        {
            k = t;
        }
    }
    return k+1; // cluster numeration starts from 0
}

void fill(size_t row_i, size_t col_i, float (*A)[row_i][col_i], float filling)
{
    for(size_t i=0; i<row_i;++i)
        for(size_t j=0; j<col_i; ++j)
        {
            (*A)[i][j] = filling;
        }
}

void* trimLastCol(size_t row_i, size_t col_i, float (*A)[row_i][col_i])
{
    size_t x_len = col_i-1;
    float (*S)[row_i][x_len] = malloc(sizeof(*S));
    for(size_t i=0; i<row_i; ++i)
        for(size_t j=0; j<x_len; ++j)
        {
            (*S)[i][j] = (*A)[i][j];
        }
    return S;
}

void distances(size_t row_i, size_t col_i, float (*S)[row_i][col_i], float (*W)[row_i][row_i])
{
    for(size_t i=0; i<row_i; ++i) {
        float *arr_a = (float*)malloc(sizeof(float)*col_i);
        for(size_t d=0; d<col_i; ++d)
        {
            arr_a[d] = (*S)[i][d];
        }
        for(size_t j=0; j<row_i; ++j)
        {
            float *arr_b = (float*)malloc(sizeof(float)*col_i);
            for(size_t d=0; d<col_i; ++d)
            {
            arr_b[d] = (*S)[j][d];
            }
            (*W)[i][j] = manhattan(col_i, arr_a, arr_b);
            free(arr_b);
        }
        free(arr_a);
    }
}

int compareDist(const void* a, const void* b)
{
    float af = ((dpair*)a)->dist;
    float bf = ((dpair*)b)->dist;
    return (af>bf)-(af<bf);
}

void knnGraph(size_t size, float (*W)[size][size], int k)
{
    for(size_t i=0; i<size; ++i)
    {
        dpair row[size];
        for(size_t j=0; j<size; ++j)
        {
            row[j].index = j;
            row[j].dist  = (*W)[i][j];
        }
        size_t s = sizeof(row)/sizeof(row[0]);
        qsort(row, s, sizeof(dpair), compareDist);
        for(size_t j=0; j<size; ++j)
        {
            (*W)[i][j] = 0;
        }
        for(size_t j=0, added=0; added<k && j<size; ++j)
        {
            size_t index = row[j].index;
            if (index==i) continue;
            (*W)[i][index] = 1;
            ++added;
        }
    }
}

void toOut(size_t size, float (*W)[size][size], float **out)
{
    for(size_t i=0; i<size; ++i)
    for(size_t j=0; j<size; ++j)
    {
        out[i][j] = (*W)[i][j];
    }
}

void degreeMatrix(size_t size, float (*D)[size][size], float (*W)[size][size])
{
    for(int i=0; i<size; ++i)
    {
        float sum = 0;
        for(int j=0; j<size; ++j)
        {
            sum += (*W)[i][j];
        }
        (*D)[i][i] = sum;
    }
}

void degreeMatrix_quick(size_t size, float (*D)[size][size], int k)
{
    for(int i=0; i<size; ++i)
    {
        (*D)[i][i] = k;
    }
}

void substractMatrices(size_t size, float (*L)[size][size],
                                    float (*D)[size][size],
                                    float (*W)[size][size])
{
    for(int i=0; i<size; ++i)
    for(int j=0; j<size; ++j)
    {
        (*L)[i][j] = (*D)[i][j] - (*W)[i][j];
    }
}