// train.c

#include "dig-recog.h"

void train(ParamManager *pm, DataManager *train_dm, DataManager *test_dm)
// 6層ニューラルネットのパラメータを勾配法で学習
{
	Parameter *del_par;
	del_par = init_params(&(del_par), 0);
	Matrix *x;
	Matrix *y;
	y = init_matrix(&y, FC3_SIZE, 1);
	unsigned char label;

	{
		cerr("\nEPOC 0:\n");
		int train_count=0;
		double train_loss=0;
		for(int i=0; i<train_dm->item_num; i++){
			if(train_dm->label_data->labels[i] == inference(y, &(train_dm->image_data->images[i]), pm->param)){
				train_count++;
			}
			train_loss += cross_entropy_loss(y, train_dm->label_data->labels[i]);
		}
		int test_count=0;
		double test_loss=0;
		for(int i=0; i<test_dm->item_num; i++){
			if(test_dm->label_data->labels[i] == inference(y, &(test_dm->image_data->images[i]), pm->param)){
				test_count++;
			}
			test_loss += cross_entropy_loss(y, test_dm->label_data->labels[i]);
		}
		cerr("test result:\n");
		cerr("train_data: ACCURACY %f%% LOSS %f\n", 100*train_count/(double)(train_dm->item_num), train_loss/train_dm->item_num);
		cerr("test_data: ACCURACY %f%% LOSS %f\n", 100*test_count/(double)(test_dm->item_num), test_loss/test_dm->item_num);
	}
	for(int e=0; e<EPOC; e++){
		cerr("\nEPOC %d:\n", e+1);
		cerr("training...\n");
		shuffle(train_dm->index, train_dm->item_num);
		for(int batch=0; batch<train_dm->item_num/BATCH; batch++){
			for(int i=0; i<BATCH; i++){
				int idx=train_dm->index[batch*BATCH+i];
				x = &(train_dm->image_data->images[idx]);
				label = train_dm->label_data->labels[idx];
				inference_and_backpropagation(y, x, pm->param, del_par, label);
				add_param(pm->delparam_sum, del_par);
			}
			if((batch+1)%30==0){
				cerr("■ ");
			}
			update_param(pm);
		}
		cerr("\nFIN\n");
		{
			int train_count=0;
			double train_loss=0;
			for(int i=0; i<train_dm->item_num; i++){
				if(train_dm->label_data->labels[i] == inference(y, &(train_dm->image_data->images[i]), pm->param)){
					train_count++;
				}
				train_loss += cross_entropy_loss(y, train_dm->label_data->labels[i]);
			}
			int test_count=0;
			double test_loss=0;
			for(int i=0; i<test_dm->item_num; i++){
				if(test_dm->label_data->labels[i] == inference(y, &(test_dm->image_data->images[i]), pm->param)){
					test_count++;
				}
				test_loss += cross_entropy_loss(y, test_dm->label_data->labels[i]);
			}
			cerr("test result:\n");
			cerr("train_data: ACCURACY %f%% LOSS %f\n", 100*train_count/(double)(train_dm->item_num), train_loss/train_dm->item_num);
			cerr("test_data: ACCURACY %f%% LOSS %f\n", 100*test_count/(double)(test_dm->item_num), test_loss/test_dm->item_num);
		}
	}
}

ParamManager *init_PM(ParamManager **pm)
// ParamManagerの初期化
{
	*pm = (ParamManager*)malloc(sizeof(ParamManager));
	(*pm)->eta = ETA;
	(*pm)->param = init_params(&((*pm)->param), 2);
	(*pm)->delparam_sum = init_params(&((*pm)->delparam_sum), 0);
	return *pm;
}

void add_param(Parameter *param, Parameter *par)
{
	add(param->A1, param->A1, par->A1);
	add(param->b1, param->b1, par->b1);
	add(param->A2, param->A2, par->A2);
	add(param->b2, param->b2, par->b2);
	add(param->A3, param->A3, par->A3);
	add(param->b3, param->b3, par->b3);
}

void update_param(ParamManager *pm)
{
	scalar(pm->delparam_sum->A1, pm->delparam_sum->A1, -pm->eta/BATCH);
	scalar(pm->delparam_sum->b1, pm->delparam_sum->b1, -pm->eta/BATCH);
	scalar(pm->delparam_sum->A2, pm->delparam_sum->A2, -pm->eta/BATCH);
	scalar(pm->delparam_sum->b2, pm->delparam_sum->b2, -pm->eta/BATCH);
	scalar(pm->delparam_sum->A3, pm->delparam_sum->A3, -pm->eta/BATCH);
	scalar(pm->delparam_sum->b3, pm->delparam_sum->b3, -pm->eta/BATCH);
	add_param(pm->param, pm->delparam_sum);
	zero_init(pm->delparam_sum->A1);
	zero_init(pm->delparam_sum->b1);
	zero_init(pm->delparam_sum->A2);
	zero_init(pm->delparam_sum->b2);
	zero_init(pm->delparam_sum->A3);
	zero_init(pm->delparam_sum->b3);
}

DataManager *init_DM(DataManager **dm, Data *image_data, Data *label_data)
{
	*dm = (DataManager*)malloc(sizeof(DataManager));
	(*dm)->item_num = image_data->item_num;
	int row = image_data->images[0].row;
	int column = image_data->images[0].column;
	for(int i=0; i<image_data->item_num; i++){
		shape(image_data->images+i, row*column, 1);
	}
	(*dm)->image_data = image_data;
	(*dm)->label_data = label_data;
	(*dm)->index = (int*)malloc(sizeof(int)*image_data->item_num);
	for(int i=0; i<image_data->item_num; i++){
		(*dm)->index[i] = i;
	}
	return *dm;
}

void shuffle(int *index, int num)
{
	for(int i=num-1; i>0; i--){
		int idx=genrand_int32()/((double)0xffffffff+1.0)*(i+1.0);
		assert(idx<=i);
		int tmp = index[i];
		index[i] = index[idx];
		index[idx] = tmp;
	}
}
