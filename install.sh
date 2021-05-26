#!/bin/bash

GITHUB_LINK_ADDRESS="https://github.com/facebookresearch/qmnist/raw/master/"

TRAIN_IMAGES_FILENAME="qmnist-train-images-idx3-ubyte.gz"
TRAIN_LABELS_FILENAME="qmnist-train-labels-idx2-int.gz"
TEST_IMAGES_FILENAME="qmnist-test-images-idx3-ubyte.gz"
TEST_LABELS_FILENAME="qmnist-test-labels-idx2-int.gz"

FILES=($TRAIN_IMAGES_FILENAME $TRAIN_LABELS_FILENAME $TEST_IMAGES_FILENAME $TEST_LABELS_FILENAME)

function check_and_download(){
	if [ -e ${1%.gz} ]; then
		echo "${1%.gz} already exist."
		return 2
	elif [ -e ${1} ]; then
		echo "${1} already exist."
		return 1
	else
		wget "${GITHUB_LINK_ADDRESS}$1" || return 3
		return 0
	fi
}

cd qmnist_data

for filename in ${FILES[@]}
do
	check_and_download $filename
	tmp=$?
	if [ $tmp -eq 3 ]; then
		echo "failed to get ${filename}"
		exit 1
	fi
	if [ $tmp -eq 2 ]; then
		continue
	fi
	gzip -d $filename
	if [ $? -ne 0 ]; then
		echo "failed to unzip ${filename}"
		exit 1
	fi
done
