#ifndef DEG_RECOG_HEADER
#define DEG_RECOG_HEADER

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#ifndef MT_HEADER
#include "MT.h"
#endif

typedef struct matrix Matrix;
typedef struct mnist_data Data;

// load.c
void load_mnist_data(const char *filename, Data **data);

// matrix.c
struct matrix{
	int row;
	int column;
	float *val;
};

struct mnist_data{
	int item_num;
	union{
		Matrix *images; // imageデータの配列
		unsigned char *labels; // labelデータの配列
	};
};

void show(Matrix *mat);
void show_image(Matrix *mat);
Matrix *mul(Matrix *A, Matrix *B, Matrix *C);
Matrix *plus(Matrix *A, Matrix *B, Matrix *C);
Matrix *scalar(Matrix *A, Matrix *B, double rt);
void rand_init(Matrix *mat);
double rand_normal(double mu, double sigma);
double Uniform();

// main.c
#endif
