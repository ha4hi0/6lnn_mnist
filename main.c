#include "dig-recog.h"

void save_params(Parameter *params, const char *filename);
void load_params(Parameter *params, const char *filename);

int main(void){
	const char 
		*train_images_file = "qmnist_data/qmnist-train-images-idx3-ubyte",
		*train_labels_file = "qmnist_data/qmnist-train-labels-idx2-int",
		*test_images_file  = "qmnist_data/qmnist-test-images-idx3-ubyte",
		*test_labels_file  = "qmnist_data/qmnist-test-labels-idx2-int";
	Data *train_data;
	Data *train_label;
	Data *test_data;
	Data *test_label;
	load_mnist_data(train_images_file, &train_data);
	load_mnist_data(train_labels_file, &train_label);
	load_mnist_data(test_images_file, &test_data);
	load_mnist_data(test_labels_file, &test_label);

	init_genrand(time(NULL));

	ParamManager *pm;
	pm = init_PM(&pm);
	DataManager *train_dm;
	DataManager *test_dm;
	init_DM(&train_dm, train_data, train_label);
	init_DM(&test_dm, test_data, test_label);
	train(pm, train_dm, test_dm);
	save_params(pm->param, "param.data");
	return 0;
}

void save_params(Parameter *params, const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if(fp == NULL){
		cerr("failed to open %s\n", filename);
	}
	fwrite(&(params->A1->row), sizeof(int), 1, fp);
	fwrite(&(params->A1->column), sizeof(int), 1, fp);
	fwrite(params->A1->val, sizeof(float), 1, fp);
	fwrite(&(params->b1->row), sizeof(int), 1, fp);
	fwrite(&(params->b1->column), sizeof(int), 1, fp);
	fwrite(params->b1->val, sizeof(float), 1, fp);

	fwrite(&(params->A2->row), sizeof(int), 1, fp);
	fwrite(&(params->A2->column), sizeof(int), 1, fp);
	fwrite(params->A2->val, sizeof(float), 1, fp);
	fwrite(&(params->b2->row), sizeof(int), 1, fp);
	fwrite(&(params->b2->column), sizeof(int), 1, fp);
	fwrite(params->b2->val, sizeof(float), 1, fp);

	fwrite(&(params->A3->row), sizeof(int), 1, fp);
	fwrite(&(params->A3->column), sizeof(int), 1, fp);
	fwrite(params->A3->val, sizeof(float), 1, fp);
	fwrite(&(params->b3->row), sizeof(int), 1, fp);
	fwrite(&(params->b3->column), sizeof(int), 1, fp);
	fwrite(params->b3->val, sizeof(float), 1, fp);
}

void load_params(Parameter *params, const char *filename)
{
	FILE *fp = fopen(filename, "wb");
	if(fp == NULL){
		cerr("failed to open %s\n", filename);
	}
	fread(&(params->A1->row), sizeof(int), 1, fp);
	fread(&(params->A1->column), sizeof(int), 1, fp);
	fread(params->A1->val, sizeof(float), 1, fp);
	fread(&(params->b1->row), sizeof(int), 1, fp);
	fread(&(params->b1->column), sizeof(int), 1, fp);
	fread(params->b1->val, sizeof(float), 1, fp);

	fread(&(params->A2->row), sizeof(int), 1, fp);
	fread(&(params->A2->column), sizeof(int), 1, fp);
	fread(params->A2->val, sizeof(float), 1, fp);
	fread(&(params->b2->row), sizeof(int), 1, fp);
	fread(&(params->b2->column), sizeof(int), 1, fp);
	fread(params->b2->val, sizeof(float), 1, fp);

	fread(&(params->A3->row), sizeof(int), 1, fp);
	fread(&(params->A3->column), sizeof(int), 1, fp);
	fread(params->A3->val, sizeof(float), 1, fp);
	fread(&(params->b3->row), sizeof(int), 1, fp);
	fread(&(params->b3->column), sizeof(int), 1, fp);
	fread(params->b3->val, sizeof(float), 1, fp);
}
