#include "dig-recog.h"
// 6層ニューラルネットの学習及び推論で使う関数について記述


// 推論

unsigned char inference(Matrix *y, Matrix *x, Parameter *par)
// 6層NNで推論を行う
// x >> fc1 >> ReLU1 >> fc2 >> ReLU2 >> fc3 >> softmax >> y
{
	Matrix *y_fc1;
	y_fc1 = init_matrix(&y_fc1, FC1_SIZE, 1);
	fc(y_fc1, x, par->A1, par->b1);

	Matrix *y_relu1;
	y_relu1 = init_matrix(&y_relu1, FC1_SIZE, 1);
	ReLU(y_relu1, y_fc1);

	Matrix *y_fc2;
	y_fc2 = init_matrix(&y_fc2, FC2_SIZE, 1);
	fc(y_fc2, y_relu1, par->A2, par->b2);

	Matrix *y_relu2;
	y_relu2 = init_matrix(&y_relu2, FC2_SIZE, 1);
	ReLU(y_relu2, y_fc2);

	Matrix *y_fc3;
	y_fc3 = init_matrix(&y_fc3, FC3_SIZE, 1);
	fc(y_fc3, y_relu2, par->A3, par->b3);

	softmax(y, y_fc3);
	int max_idx = 0;
	for(int i=1; i<10; i++){
		if(y->val[max_idx]<y->val[i]){
			max_idx = i;
		}
	}
	free(y_fc1);
	free(y_relu1);
	free(y_fc2);
	free(y_relu2);
	free(y_fc3);
	return (unsigned char)max_idx;
}

Matrix *fc(Matrix *y, Matrix *x, Matrix *A, Matrix *b)
// 全結合層(fully-connected layer)
// A, b:パラメータ
// y = Ax + b
{
	return add(y, mul(y, A, x), b);
}

Matrix *ReLU(Matrix *y, Matrix *x)
// 活性化層(Rectified Linear Unit)
// 非負ならばその値を, 負なら0にする
{
	assert(x->row == y->row);
	assert(x->column == y->column);
	for(int i=0; i<x->row*x->column; i++){
		float xi = x->val[i];
		if(xi < 0){
			xi = 0;
		}
		y->val[i] = xi;
	}
	return y;
}

Matrix *softmax(Matrix *y, Matrix *x)
// 出力層
// exp(xi)/Sum_k(exp(xk))を返す
{
	// 要素10の列ベクトルのはず
	assert(x->row == 10);
	assert(x->column == 1);

	float x_max = x->val[0];
	for(int i=1; i<10; i++){
		if(x_max < x->val[i]){
			x_max = x->val[i];
		}
	}

	// exp(xi)では数が大きくなりすぎるためexp(xi-xmax)を用いる
	float exp_xsum;
	for(int i=0; i<10; i++){
		y->val[i] = exp(x->val[i] - x_max);
		exp_xsum += y->val[i];
	}
	for(int i=0; i<10; i++){
		y->val[i] /= exp_xsum;
	}
	// 正規化された出力
	// xiがデータがiである確率を示す
	return y;
}

float cross_entropy_loss(Matrix *x, unsigned char t)
// 損失関数(交差エントロピー誤差)
// t: データのラベル
// p = {pi| i=tのとき1, それ以外0}(データがiである確率)としたとき
// 損失E=-Sum_k(pk*Sum(xk))とする
{
	// softmaxと同様
	assert(x->row == 10);
	assert(x->column == 1);
	return -log(x->val[t]);
}

Parameter *init_params(Parameter **par, char rand)
// パラメータを初期化
// rand!=0のときrand_init
{
	*par = (Parameter*)malloc(sizeof(Parameter));
	(*par)->A1 = init_matrix(&((*par)->A1), FC1_SIZE, INPUT_SIZE);
	(*par)->b1 = init_matrix(&((*par)->b1), FC1_SIZE, 1);
	(*par)->A2 = init_matrix(&((*par)->A2), FC2_SIZE, FC1_SIZE);
	(*par)->b2 = init_matrix(&((*par)->b2), FC2_SIZE, 1);
	(*par)->A3 = init_matrix(&((*par)->A3), FC3_SIZE, FC2_SIZE);
	(*par)->b3 = init_matrix(&((*par)->b3), FC3_SIZE, 1);
	if(rand){
		rand_init((*par)->A1);
		rand_init((*par)->b1);
		rand_init((*par)->A2);
		rand_init((*par)->b2);
		rand_init((*par)->A3);
		rand_init((*par)->b3);
	}
	return *par;
}
