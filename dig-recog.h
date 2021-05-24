#ifndef DIG_RECOG_HEADER_FILE
#define DIG_RECOG_HEADER_FILE

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

// main.c
#endif
