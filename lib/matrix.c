// matrix.c
// 行列操作を記述

#include "dig-recog.h"

void show(Matrix *mat)
// 行列の表示
{
	for(int r=0; r<mat->row; r++){
		for(int c=0; c<mat->column; c++){
			fprintf(stderr, "%1.9f ", mat->val[r*mat->column+c]);
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

	for(int i=0; i<A->row*A->column; i++){
		A->val[i] = B->val[i] + C->val[i];
	}
	return A;
}

Matrix *scalar(Matrix *A, Matrix *B, double rt)
// Bのrt倍をする
{
	assert(A->column == B->column);
	assert(A->row == B->row);

	for(int i=0; i<A->row*A->column; i++){
		A->val[i] = rt*B->val[i];
	}
	return A;
}

Matrix *init_matrix(Matrix **mat, int row, int column)
// row*column行列を作成
{
	*mat = (Matrix*)malloc(sizeof(Matrix));
	if(*mat == NULL){
		cerr("malloc failed.\n");
		exit(1);
	}
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

void zero_init(Matrix *mat)
// 0で行列を初期化
{
	for(int i=0; i<mat->row*mat->column; i++){
		mat->val[i] = 0;
	}
}

void rand_init(Matrix *mat)
// 乱数で行列を初期化する
{
	for(int i=0; i<mat->row*mat->column; i++){
		mat->val[i] = (float)rand_normal(0.0, 1.0);
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

Matrix *transpose(Matrix *AT, Matrix *A)
// 転置
// AT=A^T
{
	assert(A->row==AT->column);
	assert(AT->row==A->column);
	for(int r=0; r<A->row; r++){
		for(int c=0; c<A->column; c++){
			AT->val[c*A->row+r] = A->val[r*A->column+c];
		}
	}
	return AT;
}
