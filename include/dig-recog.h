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

#define ETA 0.02 // 学習率
#define EPOC 20 // エポック数
#define BATCH 100 // バッチサイズ

typedef struct matrix Matrix;
typedef struct mnist_data Data;
typedef struct parameter Parameter;
typedef struct paramManager ParamManager;
typedef struct dataManager DataManager;

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
void free_matrix(Matrix *mat);
void zero_init(Matrix *mat);
void rand_init(Matrix *mat);
double rand_normal(double mu, double sigma);
double Uniform();
void shape(Matrix *A, int row, int column);
Matrix *transpose(Matrix *AT, Matrix *A);

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

// 推論
unsigned char inference(Matrix *y, Matrix *x, Parameter *par);
Matrix *fc(Matrix *y, Matrix *x, Matrix *A, Matrix *b);
Matrix *ReLU(Matrix *y, Matrix *x);
Matrix *softmax(Matrix *y, Matrix *x);
unsigned char calc_answer(Matrix *x);
float cross_entropy_loss(Matrix *x, unsigned char t);
// 学習
unsigned char inference_and_backpropagation(Matrix *y, Matrix *x, Parameter *par, Parameter *del_par, unsigned char t);
Matrix *del_celandsm(Matrix *delE_delx, Matrix *y, unsigned char t);
Matrix *del_fc(Matrix *delE_delx, Matrix *delE_dely, Matrix *x, Matrix *delE_delA, Matrix *delE_delb, Matrix *A);
Matrix *del_ReLU(Matrix *delE_dely, Matrix *delE_delx, Matrix *x);

Parameter *init_params(Parameter **par, char rand);

// train.c
struct paramManager{
	float eta;
	Parameter *param;
	Parameter *delparam_sum;
};

struct dataManager{
	int item_num;
	Data *image_data;
	Data *label_data;
	int *index;
};

void train(ParamManager *pm, DataManager *train_dm, DataManager *test_dm);
ParamManager *init_PM(ParamManager **pm);
void add_param(Parameter *param, Parameter *par);
void update_param(ParamManager *pm);
DataManager *init_DM(DataManager **dm, Data *image_data, Data *label_data);
void shuffle(int *index, int num);

#endif
