#include "dig-recog.h"
// 行列操作を記述

void show(Matrix *mat)
// 行列の表示
{
	for(int r=0; r<mat->row; r++){
		for(int c=0; c<mat->column; c++){
			fprintf(stderr, "%f ", mat->val[r*mat->column+c]);
		}
		fprintf(stderr, "\n");
	}
}

void show_image(Matrix *mat)
// 行列を画像風に表示
{
	for(int r=0; r<mat->row; r++){
		for(int c=0; c<mat->column; c++){
			if(mat->val[r*mat->column+c]>0.5){
				fprintf(stderr, "■ ");
			}else{
				fprintf(stderr, "□ ");
			}
		}
		fprintf(stderr, "\n");
	}
}

Matrix* mul(Matrix *A, Matrix *B, Matrix *C)
// A=BCを計算する
{
	assert(B->column == C->row);
	assert(A->row == B->row);
	assert(A->column == C->column);

	int bccr=B->column;
	int arow=A->row;
	int acol=A->column;

	for(int r=0; r<arow; r++){
		for(int c=0; c<acol; c++){
			A->val[r*acol+c] = 0;
			for(int ac=0; ac<bccr; ac++){
				A->val[r*acol+c] += B->val[r*bccr+ac]*C->val[ac*acol+c];
			}
		}
	}
	return A;
}

Matrix* add(Matrix *A, Matrix *C, Matrix *B)
// A=B+Cを計算する
{
	assert(B->column == C->column);
	assert(C->column == A->column);
	assert(B->row == C->row);
	assert(C->row == A->row);

	int row = B->row;
	int col = C->column;

	for(int r=0; r<row; r++){
		for(int c=0; c<col; c++){
			A->val[r*col+c] = B->val[r*col+c] + C->val[r*col+c];
		}
	}
	return A;
}

Matrix *scalar(Matrix *A, Matrix *B, double rt)
// Bのrt倍をする
{
	assert(A->column == B->column);
	assert(A->row == B->row);

	int row = A->row;
	int col = A->column;

	for(int r=0; r<row; r++){
		for(int c=0; c<row; c++){
			A->val[r*col+c] = rt*B->val[r*col+c];
		}
	}
	return A;
}

Matrix *init_matrix(Matrix **mat, int row, int column)
// row*column行列を作成
{
	*mat = (Matrix*)malloc(sizeof(Matrix));
	(*mat)->row = row;
	(*mat)->column = column;
	(*mat)->val = (float*)malloc(sizeof(float)*row*column);
	return *mat;
}

void free_matrix(Matrix *mat)
// 行列を削除
{
	free(mat->val);
	free(mat);
}

void rand_init(Matrix *mat)
// 乱数で行列を初期化する
{
	for(int r=0; r<mat->row; r++){
		for(int c=0; c<mat->column; c++){
			mat->val[r*mat->column+c] = (float)rand_normal(0.0, 1.0);
			// 平均0分散1のガウス分布
		}
	}
}

double rand_normal(double mu, double sigma)
// Box-Muller法によりガウス分布にしたがう乱数を生成
{
	double z=sqrt(-2.0*log(Uniform()))*sin(2.0*M_PI*Uniform());
	return mu + sigma*z;
}

double Uniform()
// (0, 1) の範囲の一様乱数
{
	return genrand_real3();
}

void shape(Matrix *A, int row, int column)
// 行列の縦横の長さを変える
{
	assert(A->row*A->column == row*column);
	A->row = row;
	A->column = column;
}
