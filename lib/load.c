// load.c
// MNISTデータのローダー

#include "dig-recog.h"

void load_mnist_labels_data(FILE *fp, Data **data);
void load_mnist_images_data(FILE *fp, Data **data);
void fread_big_endian(void *buf, size_t size, size_t n, FILE *fp);

void load_mnist_data(const char *filename, Data **data)
// MNISTデータのフォーマットに従って読み込みを行う
{
	FILE *fp = fopen(filename, "rb");
	if(fp == NULL){
		fprintf(stderr, "failed to open %s", filename);
		exit(1);
	}

	int magic_number;
	fread_big_endian(&magic_number, sizeof(magic_number), 1, fp);
	// MNISTデータはビッグエンディアンなのでそれ用のfreadが必要
	// 今回用いるデータではmagic_numberが3074のときlabelデータ, 2051のときimageデータ

	switch(magic_number){
		case 3074:
			load_mnist_labels_data(fp, data);
			break;
		case 2051:
			load_mnist_images_data(fp, data);
			break;
		default:
			fprintf(stderr, "magic number is %d.\nplease check input file.\n", magic_number);
			exit(1);
	}

	if(fclose(fp) == EOF){
		fprintf(stderr, "failed to close %s", filename);
		exit(1);
	}
}

void load_mnist_labels_data(FILE *fp, Data **data)
// labelデータを読む
{
	*data = (Data *)malloc(sizeof(Data));
	fread_big_endian(&((*data)->item_num), sizeof((*data)->item_num), 1, fp);
	fseek(fp, 4, SEEK_CUR);
	// rownum=item_num columnnum=8の行列データで各rowにはlabelの他にNISTの筆者IDなどが記されるが今回は無視する
	// 0番目がlabel
	int *buf = (int *)malloc(sizeof(int)*(*data)->item_num*8);
	fread_big_endian(buf, sizeof(int), (*data)->item_num*8, fp);
	(*data)->labels = (unsigned char *)malloc(sizeof(unsigned char)*(*data)->item_num);
	for(int i=0; i<(*data)->item_num; i++){
		(*data)->labels[i] = (unsigned char)buf[i*8];
	}
	free(buf);
}

void load_mnist_images_data(FILE *fp, Data **data)
// imageデータを読む
{
	(*data) = (Data *)malloc(sizeof(Data));
	fread_big_endian(&((*data)->item_num), sizeof((*data)->item_num), 1, fp);
	(*data)->images = (Matrix *)malloc(sizeof(Matrix)*(*data)->item_num);
	int row, column;
	fread_big_endian(&row, sizeof(row), 1, fp);
	fread_big_endian(&column, sizeof(column), 1, fp);

	// 元データはunsined charだがそれを255で割って正規化しfloatとする
	unsigned char *tmp = (unsigned char *)malloc(sizeof(int)*row*column*(*data)->item_num); 
	fread(tmp, sizeof(tmp), row*column*(*data)->item_num, fp);

	for(int i=0; i<(*data)->item_num; i++){
		float *image = (float *)malloc(sizeof(float)*row*column);
		for(int k=0; k<row*column; k++){
			image[k] = tmp[row*column*i+k]/(float)255;
		}
		(*data)->images[i].val = image;
		(*data)->images[i].row = row;
		(*data)->images[i].column = column;
	}
	free(tmp);
	return;
}


void fread_big_endian(void *buf, size_t size, size_t n, FILE *fp)
// ビッグエンディアン用のfread
{
	for(int i=0; i<n; i++){
		for(int k=0; k<size; k++){
			char tmp;
			fread(&tmp, 1, 1, fp);
			memset(buf+size*i+(size-1-k), tmp, 1);
		}
	}
}

