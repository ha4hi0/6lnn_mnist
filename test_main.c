#include "dig-recog.h"
// 関数のテスト

int main(void){
	init_genrand(time(NULL));

	cerr("hoge");
	cerr("test libralies\n");
	cerr("mnist loader and show matrix test\n");
	Data *train_data;
	Data *train_label;
	load_mnist_data("qmnist_data/qmnist-train-images-idx3-ubyte", &train_data);
	load_mnist_data("qmnist_data/qmnist-train-labels-idx2-int", &train_label);
	cerr("\nlabel: %d\nmatrix:\n", train_label->labels[100]);
	show_image(&(train_data->images[100]));

	cerr("\ntest matrix calc and rand init\n");

	Matrix *C;
	C = init_matrix(&C, 5, 3);
	rand_init(C);

	Matrix *B;
	B = init_matrix(&B, 4, 5);
	rand_init(B);

	Matrix *A;
	A = init_matrix(&A, 4, 3);

	cerr("C:\n");
	show(C);

	cerr("\nB:\n");
	show(B);

	cerr("\nA=CB:\n");
	show(mul(A, B, C));

	Matrix *D;
	D = init_matrix(&D, 5, 3);

	rand_init(D);
	cerr("\nD:\n");
	show(D);

	Matrix *E;
	E = init_matrix(&E, 5, 3);

	cerr("\nE=C+D:\n");
	show(add(E, D, C));

	cerr("\nE=2.0*D:\n");
	show(scalar(E, D, 2.0));

	Parameter *par;
	par = init_params(&par, 2);
	Matrix *x = &(train_data->images[3]);
	cerr("\nx:\n");
	show_image(x);
	shape(x, INPUT_SIZE, 1);
	Matrix *y = init_matrix(&y, FC3_SIZE, 1);

	cerr("\ninference (with radom parameter):\n");
	cerr("%d\n", (int)inference(y, x, par));
}
