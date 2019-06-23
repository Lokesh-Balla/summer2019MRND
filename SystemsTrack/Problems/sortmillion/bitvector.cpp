#include "links.h"

int *setbit(int *arr, int k){
	int i = k / 32;
	int pos = k % 32;
	unsigned int flag = 1;
	flag = flag << pos;
	arr[i] = arr[i] | flag;
	return arr;
}

int *clearbit(int *arr, int k){
	int i = k / 32;
	int pos = k % 32;
	unsigned int flag = 1;
	flag = flag << pos;
	flag = ~flag;
	arr[i] = arr[i] & flag;
	return arr;
}

int testbit(int *arr, int k){
	int i = k / 32;
	int pos = k % 32;
	unsigned int flag = 1;
	flag = flag << pos;
	if (arr[i] & flag){
		return 1;
	}
	else{
		return 0;
	}
}