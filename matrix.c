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
