#include "links.h"

char *generate_filename(char *fname, int num){
	char *filename = (char*)malloc(sizeof(char) * 100);
	int i = 0;
	sprintf(filename, "%s%d", fname, num);
	return filename;
}

int count_the_numbers_in_file(char *filename){
	if (filename == NULL){
		return 0;
	}
	char *fname = (char*)malloc(sizeof(char) * 100);
	sprintf(fname, "%s%s", filename, ".txt");
	FILE *file = fopen(fname, "r");
	int count = 0;
	int present_value = 0;
	while (fscanf(file, "%d", &present_value) == 1){
		count++;
	}
	fclose(file);
	return count;
}

int *convert_to_array(char *filename){
	if (filename == NULL){
		return 0;
	}
	int i = 0;
	char *fname = (char*)malloc(sizeof(char) * 100);
	sprintf(fname, "%s%s", filename, ".txt");
	FILE *fptr = fopen(fname, "r");
	int *arr = (int *)malloc(sizeof(int) * 1000000000);
	while (fscanf(fptr, "%d", &arr[i]) == 1){
		i++;
	}
	fclose(fptr);
	arr = (int *)realloc(arr, sizeof(int)*i);
	return arr;
}

int *sort_the_numbers(int *arr, int n){
	if (arr == NULL){
		return 0;
	}
	int i, j;
	for (i = 0; i < n - 1; i++){
		for (j = 0; j < n - i - 1; j++){
			if (arr[j] > arr[j + 1]){
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
	return arr;
}