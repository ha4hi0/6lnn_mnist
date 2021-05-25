#include "dig-recog.h"

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
//CxB=Aを計算する
{
	assert(C->column == B->row);
	assert(A->row == C->row);
	assert(A->column == B->column);

	int abcr=C->column;
	int arow=C->row;
	int bcol=B->column;

	for(int r=0; r<arow; r++){
		for(int c=0; c<bcol; c++){
			A->val[r*bcol+c] = 0;
			for(int ac=0; ac<abcr; ac++){
				A->val[r*bcol+c] += C->val[r*abcr+ac]*B->val[ac*bcol+c];
			}
		}
	}
	return A;
}

Matrix* plus(Matrix *A, Matrix *B, Matrix *C)
//C+B=Aを計算する
{
	assert(C->column == B->column);
	assert(B->column == A->column);
	assert(C->row == B->row);
	assert(B->row == A->row);

	int row = C->row;
	int col = B->column;

	for(int r=0; r<row; r++){
		for(int c=0; c<col; c++){
			A->val[r*col+c] = C->val[r*col+c] + B->val[r*col+c];
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
			A->val[r*col+c] = rt*B->val[r&col+c];
		}
	}
	return A;
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
