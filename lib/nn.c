// nn.c
// 6層ニューラルネットの学習及び推論で使う関数について記述

#include "dig-recog.h"

// 推論

unsigned char inference(Matrix *y, Matrix *x, Parameter *par)
// 6層NNで推論を行う
// x >> fc1 >> ReLU1 >> fc2 >> ReLU2 >> fc3 >> softmax >> y
{
	Matrix *y_fc1;
	Matrix *y_relu1;
	Matrix *y_fc2;
	Matrix *y_relu2;
	Matrix *y_fc3;

	y_fc1 = init_matrix(&y_fc1, FC1_SIZE, 1);
	y_relu1 = init_matrix(&y_relu1, FC1_SIZE, 1);
	y_fc2 = init_matrix(&y_fc2, FC2_SIZE, 1);
	y_relu2 = init_matrix(&y_relu2, FC2_SIZE, 1);
	y_fc3 = init_matrix(&y_fc3, FC3_SIZE, 1);

	fc(y_fc1, x, par->A1, par->b1);
	ReLU(y_relu1, y_fc1);
	fc(y_fc2, y_relu1, par->A2, par->b2);
	ReLU(y_relu2, y_fc2);
	fc(y_fc3, y_relu2, par->A3, par->b3);
	softmax(y, y_fc3);

	free_matrix(y_fc1);
	free_matrix(y_relu1);
	free_matrix(y_fc2);
	free_matrix(y_relu2);
	free_matrix(y_fc3);
	return calc_answer(y);
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
	// 要素FC3_SIZEの列ベクトルのはず
	assert(x->row == FC3_SIZE);
	assert(x->column == 1);

	float x_max = x->val[0];
	for(int i=1; i<FC3_SIZE; i++){
		if(x_max < x->val[i]){
			x_max = x->val[i];
		}
	}

	// exp(xi)では数が大きくなりすぎるためexp(xi-xmax)を用いる
	float exp_xsum;
	for(int i=0; i<FC3_SIZE; i++){
		y->val[i] = exp(x->val[i] - x_max);
		exp_xsum += y->val[i];
	}
	for(int i=0; i<FC3_SIZE; i++){
		y->val[i] /= exp_xsum;
	}
	// 正規化された出力
	// xiがデータがiである確率を示す
	return y;
}

unsigned char calc_answer(Matrix *x)
// 推論結果
{
	unsigned char answer=0;
	for(unsigned char i=0; i<FC3_SIZE; i++){
		if(x->val[answer]<x->val[i]){
			answer = i;
		}
	}
	return answer;
}

float cross_entropy_loss(Matrix *x, unsigned char t)
// 損失関数(交差エントロピー誤差)
// t: データのラベル
// p = {pi| i=tのとき1, それ以外0}(データがiである確率)としたとき
// 損失E=-Sum_k(pk*log(xk))とする
{
	// softmaxと同様
	assert(x->row == FC3_SIZE);
	assert(x->column == 1);
	return fmax(-log(x->val[t]+1e-11), 0);
}

// 学習
// 下流からdelE/delyを受け取り上流にdelE/delxを流す(微分の連鎖律)

unsigned char inference_and_backpropagation(Matrix *y, Matrix *x, Parameter *par, Parameter *del_par, unsigned char t)
// 推論後に誤差逆伝播
// x >> fc >> relu >> fc >> relu >> fc >> softmax 
//	 >> del softmax and del_cross entropy loss >> delfc >> delrelu >> delfc >> delrelu >> delfc
{
	// 推論層の入出力
	Matrix *y_fc1;
	Matrix *y_relu1;
	Matrix *y_fc2;
	Matrix *y_relu2;
	Matrix *y_fc3;

	y_fc1 = init_matrix(&y_fc1, FC1_SIZE, 1);
	y_relu1 = init_matrix(&y_relu1, FC1_SIZE, 1);
	y_fc2 = init_matrix(&y_fc2, FC2_SIZE, 1);
	y_relu2 = init_matrix(&y_relu2, FC2_SIZE, 1);
	y_fc3 = init_matrix(&y_fc3, FC3_SIZE, 1);

	// 誤差逆伝播の入出力
	Matrix *delE_dely_fc3;
	Matrix *delE_dely_relu2;
	Matrix *delE_dely_fc2;
	Matrix *delE_dely_relu1;
	Matrix *delE_dely_fc1;
	Matrix *delE_delx;

	delE_dely_fc3 = init_matrix(&delE_dely_fc3, FC3_SIZE, 1);
	delE_dely_relu2 = init_matrix(&delE_dely_relu2, FC2_SIZE, 1);
	delE_dely_fc2 = init_matrix(&delE_dely_fc2, FC2_SIZE, 1);
	delE_dely_relu1 = init_matrix(&delE_dely_relu1, FC1_SIZE, 1);
	delE_dely_fc1 = init_matrix(&delE_dely_fc1, FC1_SIZE, 1);
	delE_delx = init_matrix(&delE_delx, INPUT_SIZE, 1);

	fc(y_fc1, x, par->A1, par->b1);
	ReLU(y_relu1, y_fc1); 
	fc(y_fc2, y_relu1, par->A2, par->b2);
	ReLU(y_relu2, y_fc2);
	fc(y_fc3, y_relu2, par->A3, par->b3);
	softmax(y, y_fc3);

	del_celandsm(delE_dely_fc3, y, t);
	del_fc(delE_dely_relu2, delE_dely_fc3, y_relu2, del_par->A3, del_par->b3, par->A3);
	del_ReLU(delE_dely_fc2, delE_dely_relu2, y_fc2);
	del_fc(delE_dely_relu1, delE_dely_fc2, y_relu1, del_par->A2, del_par->b2, par->A2);
	del_ReLU(delE_dely_fc1, delE_dely_relu1, y_fc1);
	del_fc(delE_delx, delE_dely_fc1, x, del_par->A1, del_par->b1, par->A1);

	free_matrix(delE_dely_fc1);
	free_matrix(delE_dely_relu1);
	free_matrix(delE_dely_fc2);
	free_matrix(delE_dely_relu2);
	free_matrix(delE_dely_fc3);
	free_matrix(y_fc3);
	free_matrix(y_relu2);
	free_matrix(y_fc2);
	free_matrix(y_relu1);
	free_matrix(y_fc1);
	free_matrix(delE_delx);

	return calc_answer(y);
}

Matrix *del_celandsm(Matrix *delE_delx, Matrix *y, unsigned char t)
// y=cross_entropy_loss(softmax(x))のxによる偏微分を計算
// dely/delx = y - t となる
{
	assert(delE_delx->row == FC3_SIZE);
	assert(delE_delx->column == 1);

	for(int i=0; i<FC3_SIZE; i++){
		if(i == t){
			delE_delx->val[i] = y->val[i] - 1;
		}else{
			delE_delx->val[i] = y->val[i];
		}
	}
	return delE_delx;
}

Matrix *del_fc(Matrix *delE_delx, Matrix *delE_dely, Matrix *x, Matrix *delE_delA, Matrix *delE_delb, Matrix *A)
// fcの偏微分
// delE/delx = A^T delE/dely
// delE/delA = delE/dely x^T
// delE/delb = delE/dely
{
	Matrix *xT;
	init_matrix(&xT, x->column, x->row);
	mul(delE_delA, delE_dely, transpose(xT, x));

	scalar(delE_delb, delE_dely, 1);

	Matrix *AT;
	init_matrix(&AT, A->column, A->row);
	mul(delE_delx, transpose(AT, A), delE_dely);

	free_matrix(AT);
	free_matrix(xT);
	return delE_delx;
}

Matrix *del_ReLU(Matrix *delE_delx, Matrix *delE_dely, Matrix *x)
// ReLUの偏微分
// delE/delxk = (xk>0)?delE/dely:0
{
	for(int i=0; i<x->row*x->column; i++){
		if(x->val[i]>0){
			delE_delx->val[i] = delE_dely->val[i];
		}else{
			delE_delx->val[i] = 0;
		}
	}
	return delE_delx;
}

Parameter *init_params(Parameter **par, char rand)
// パラメータを初期化
// rand==0のときzero_init
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
	}else{
		zero_init((*par)->A1);
		zero_init((*par)->b1);
		zero_init((*par)->A2);
		zero_init((*par)->b2);
		zero_init((*par)->A3);
		zero_init((*par)->b3);
	}
	return *par;
}
