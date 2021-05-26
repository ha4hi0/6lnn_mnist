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

#ifndef DEBUG_HEADER
#include "debug.h"
#endif

// ハイパーパラメータ
// 各層の出力のサイズ
#define INPUT_SIZE 28*28
#define FC1_SIZE 50
#define FC2_SIZE 100
#define FC3_SIZE 10
// 学習率
#define ETA 0.1

typedef struct matrix Matrix;
typedef struct mnist_data Data;
typedef struct parameter Parameter;

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
Matrix *add(Matrix *A, Matrix *B, Matrix *C);
Matrix *scalar(Matrix *A, Matrix *B, double rt);
Matrix *init_matrix(Matrix **mat, int row, int column);
void rand_init(Matrix *mat);
double rand_normal(double mu, double sigma);
double Uniform();
void shape(Matrix *A, int row, int column);

// nn.c

// 6層NNで使うパラメータ
struct parameter{
	// fc1
	Matrix *A1;
	Matrix *b1;
	// fc2
	Matrix *A2;
	Matrix *b2;
	// fc3
	Matrix *A3;
	Matrix *b3;
};

unsigned char inference(Matrix *y, Matrix *x, Parameter *par);
Matrix *fc(Matrix *y, Matrix *x, Matrix *A, Matrix *b);
Matrix *ReLU(Matrix *y, Matrix *x);
Matrix *softmax(Matrix *y, Matrix *x);
float cross_entropy_loss(Matrix *x, unsigned char t);
Parameter *init_params(Parameter **par, char rand);

// train.c

#endif
