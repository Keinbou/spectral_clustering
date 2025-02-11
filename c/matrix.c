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
    if ( row_i==col_i )
        printf("Symmetrical: %i\n\n", isSymmetrical(row_i, A));
    p("");
}

void printList(size_t len, float *list, size_t len_print, const char* title)
// pass -1 or >row_i as n to read all rows
{
    p("%s", title);
    if (len_print==-1 || len_print>len) len_print=len;
    char * str = "";
    for(size_t i=0; i<len; ++i) {
        if( i==len_print )
        {
            break;
        }
        str = concat(str, printfStr("%i\t", i));
        str = concat(str, float2String(list[i]));
        str = concat(str, "\n");
    }
    printf("%s", str);
    p("%zu length list. %zu printed.", len, len_print);
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

void* trimMatrix(size_t row_i, size_t col_i, size_t new_col_i, float (*A)[row_i][col_i], size_t from, size_t to)
{
    float (*T)[row_i][new_col_i] = malloc(sizeof(*T));
    for(size_t i=0; i<row_i; ++i)
        for(size_t j=from, l=0; j<=to; ++j, ++l)
        {
            (*T)[i][l] = (*A)[i][j];
        }
    return T;
}

void distances(size_t row_i, size_t col_i, float (*S)[row_i][col_i], float (*N)[row_i][row_i])
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
            (*N)[i][j] = manhattan(col_i, arr_a, arr_b);
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

void knnGraph(size_t size, float (*N)[size][size], float (*W)[size][size], int k)
{
    for(size_t i=0; i<size; ++i)
    {
        dpair row[size];
        for(size_t j=0; j<size; ++j)
        {
            row[j].index = j;
            row[j].dist  = (*N)[i][j];
        }
        size_t s = sizeof(row)/sizeof(row[0]);
        qsort(row, s, sizeof(dpair), compareDist);
        for(size_t j=0, added=0; added<k && j<size; ++j)
        {
            size_t index = row[j].index;
            if (index==i) continue;
            (*W)[i][index] = 1;
            (*W)[index][i] = 1;
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

void degreeMatrix_quick(size_t size, float (*D)[size][size], int k) // and shit
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

void Givens(size_t size, int Nrun, float (*L)[size][size], float (*V)[size][size], double tol)
{
    int i,j,k,  p,q,r;
    float c,s,  t,tmax,tau,  alpha,  xp,xq, eps=1.0E-22;
    double sum;

    for (i=0; i<size; ++i)
    {
        for (j=0; j<size; ++j)
        {
            if (i==j) (*V)[i][j]=1.0; else (*V)[i][j]=0.0;
        }
    }

    for (k=0; k<Nrun; ++k)
    {
        tmax = -1;
        sum  = 0.0;
        for (i=0; i<size; ++i)
        {
            for (j=i+1; j<size; ++j)
            {
                t = _abs((*L)[i][j]);
                sum += (t*t);
                if ( t>tmax ) { tmax=t; p=i; q=j; }
            }
        }
        sum = sqrt(2.0*sum);
        if ( sum < tol ) return;
        alpha = ( (*L)[q][q]-(*L)[p][p] ) / 2.0 / (*L)[p][q];
        t = ( (alpha>eps)? 1.0/(alpha+sqrt(1.0+alpha*alpha)) :
              (alpha<eps)? 1.0/(alpha-sqrt(1.0+alpha*alpha)) : 1.0);
        
        c   = 1.0/sqrt(1.0+t*t);
        s   = c*t;
        tau = s/(1.0+c);

        for (r=0  ; r<p   ; ++r) (*L)[p][r] = c*(*L)[r][p] - s*(*L)[r][q];
        for (r=p+1; r<size; ++r) if (r!=q) (*L)[r][p] = c*(*L)[p][r] - s*(*L)[q][r];

        for (r=0  ; r<p   ; ++r) (*L)[q][r] = s*(*L)[r][p] + c*(*L)[r][q];
        for (r=p+1; r<q   ; ++r) (*L)[q][r] = s*(*L)[p][r] + c*(*L)[r][q];
        for (r=q+1; r<size; ++r) (*L)[r][q] = s*(*L)[p][r] + c*(*L)[q][r];

        (*L)[p][p] = (*L)[p][p] - t*(*L)[p][q];
        (*L)[q][q] = (*L)[q][q] + t*(*L)[p][q];
        (*L)[q][p] = 0.0;

        for (i=0  ; i<size; ++i)
        for (j=i+1; j<size; ++j)
            (*L)[i][j] = (*L)[j][i];

        for (i=0; i<size; ++i)
        {
            xp = (*V)[i][p];
            xq = (*V)[i][q];
            (*V)[i][p] = c*xp - s*xq;
            (*V)[i][q] = s*xp + c*xq;
        }
    }
    return;
}

float _random(int upper_bound)
{
    return (float)(((double)(rand()%upper_bound) - (double)(upper_bound/2))/upper_bound);
}

float* _kmeans(size_t row_i, size_t col_i, float (*V)[row_i][col_i], int k)
{
    float (*Centers)[k][col_i]  = malloc(sizeof(*Centers));
    float *clustered = malloc(sizeof(float)*row_i);
    int seed_inc = 0;
    for (int i=0; i<row_i; ++i) clustered[i] = -1;

    for (int i=0; i<k; ++i)
    {
        srand(time(NULL)+seed_inc);
        seed_inc += 1;
        float x = _random(UPPER_BOUND);
        (*Centers)[i][i] = x;
    }

    float *center = malloc(sizeof(float)*col_i);
    float *dot = malloc(sizeof(float)*col_i);
    float minDist;
    int cluster;
    bool changed;
    do
        {
        changed = false;
        for (int i=0; i<row_i; ++i)
        {
            for (int j=0; j<col_i; ++j) dot[j] = (*V)[i][j];
            minDist = MAXFLOAT;
            cluster = -1;
            for (int c=0; c<k; ++c)
                {
                for (int l=0; l<col_i; ++l) center[l] = (*Centers)[c][l];
                float distance = manhattan(col_i, center, dot);
                if (distance<minDist) { minDist=distance; cluster=c; } 
                }
            if (clustered[i] != cluster) changed = true;
            clustered[i] = cluster;
        }
        for (int c=0; c<k; ++c)
        {
            size_t count = 0;
            for (int j=0; j<col_i; ++j) (*Centers)[c][j] = 0.0;
            for (int j=0; j<row_i; ++j) {
                if (clustered[j]==c)
                    {
                        count += 1;
                        for (int l=0; l<k; ++l)
                            {
                                (*Centers)[c][l] += (*V)[j][l];
                            }
                    }
            }
            if (count == 0) 
                {
                for (int l=0; l<col_i; ++l)
                    {
                    srand(time(NULL)+seed_inc);
                    seed_inc += 1;
                    float x = _random(UPPER_BOUND);
                    (*Centers)[c][l] = x;
                    }
                }
            else
                {
                for (int l=0; l<col_i; ++l)
                    {
                        (*Centers)[c][l] /= count;
                    }
                }
        }
    } while (changed);
    free(Centers);
    return clustered;
}

bool isSymmetrical(size_t size, float (*A)[size][size])
{
    for (int i=0; i<size; ++i)
        for (int j=i; j<size; ++j)
        {
            if ((*A)[i][j]!=(*A)[j][i]) return false;
        }
    return true;
}