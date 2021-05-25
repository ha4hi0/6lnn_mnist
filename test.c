#include "dig-recog.h"

int main(void){
	init_genrand(time(NULL));

	fprintf(stderr, "test libralies\n");
	fprintf(stderr, "mnist loader and show matrix test\n");
	Data *train_data;
	Data *train_label;
	load_mnist_data("qmnist-train-images-idx3-ubyte", &train_data);
	load_mnist_data("qmnist-train-labels-idx2-int", &train_label);
	fprintf(stderr, "\nlabel: %d\nmatrix:\n", train_label->labels[100]);
	show_image(&(train_data->images[100]));

	fprintf(stderr, "\ntest matrix calc and rand init\n");

	Matrix C;
	C.row = 3;
	C.column = 5;
	C.val = (float*)malloc(sizeof(float)*C.row*C.column);
	rand_init(&C);

	Matrix B;
	B.row = 5;
	B.column = 4;
	B.val = (float*)malloc(sizeof(float)*B.row*B.column);
	rand_init(&B);

	Matrix A;
	A.row = 3;
	A.column = 4;
	A.val = (float*)malloc(sizeof(float)*A.row*A.column);

	fprintf(stderr, "C:\n");
	show(&C);

	fprintf(stderr, "\nB:\n");
	show(&B);

	fprintf(stderr, "\nA=CB:\n");
	show(mul(&A, &B, &C));

	Matrix D;
	D.row = 3;
	D.column = 5;
	D.val = (float*)malloc(sizeof(float)*D.row*D.column);

	rand_init(&D);
	fprintf(stderr, "\nD:\n");
	show(&D);

	Matrix E;
	E.row = 3;
	E.column = 5;
	E.val = (float*)malloc(sizeof(float)*E.row*E.column);

	fprintf(stderr, "\nE=C+D:\n");
	show(plus(&E, &D, &C));

	fprintf(stderr, "\nE=2.0*D:\n");
	show(scalar(&E, &D, 2.0));
}
