#include "dig-recog.h"


int main(void){
	const char 
		*train_images_file = "qmnist-train-images-idx3-ubyte",
		*train_labels_file = "qmnist-train-labels-idx2-int",
		*test_images_file = "qmnist-test-images-idx3-ubyte",
		*test_labels_file = "qmnist-test-labels-idx2-int";
	Data *train_data;
	Data *train_label;
	Data *test_data;
	Data *test_label;
	load_mnist_data(train_images_file, &train_data);
	load_mnist_data(train_labels_file, &train_label);
	load_mnist_data(test_images_file, &test_data);
	load_mnist_data(test_labels_file, &test_label);
	return 0;
}
