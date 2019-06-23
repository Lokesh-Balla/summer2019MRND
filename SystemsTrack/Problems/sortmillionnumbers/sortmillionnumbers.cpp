#include "links.h"


void merge(char *file1, char *file2){
	int *arr_file1 = (int*)malloc(sizeof(int) * 11000);
	int *arr_file2 = (int*)malloc(sizeof(int) * 11000);
	int len1 = 0, len2 = 0;
	if (file1 != NULL){
		arr_file1 = convert_to_array(file1);
		len1 = count_the_numbers_in_file(file1);
		arr_file1 = sort_the_numbers(arr_file1, len1);
	}
	if (file2 != NULL){
		arr_file2 = convert_to_array(file2);
		len2 = count_the_numbers_in_file(file2);
		arr_file2 = sort_the_numbers(arr_file2, len2);
	}
	char *fname = (char*)malloc(sizeof(char) * 100);
	sprintf(fname, "%s%s", file1, ".txt");
	FILE *fptr = fopen(fname, "w");
	int i = 0, j = 0;
	if (file1 != NULL&&file2 != NULL){
		while (i < len1 && j < len2){
			if (arr_file1[i] <= arr_file2[j]){
				fprintf(fptr, "%d\n", arr_file1[i]);
				printf("%d\n", arr_file1[i]);
				i++;
			}
			else{
				fprintf(fptr, "%d\n", arr_file2[j]);
				printf("%d\n", arr_file2[j]);
				j++;
			}
		}
	}
	if (file1 != NULL){
		while (i < len1){
			fprintf(fptr, "%d\n", arr_file1[i]);
			printf("%d\n", arr_file1[i]);
			i++;
		}
	}
	if (file2 != NULL){
		while (j < len2){
			fprintf(fptr, "%d\n", arr_file2[j]);
			printf("%d\n", arr_file2[j]);
			j++;
		}
	}
	fclose(fptr);
}


void split(char *filename,int file_no){
	int number_limit = 10000;
	int count = 0;
	int present_value = 0;
	int i = 0;
	int size = 0;
	FILE *fptr;
	char *fname = (char*)malloc(sizeof(char) * 105);
	if (file_no == 0){
		sprintf(fname, "%s%s", filename, ".txt");
		 fptr = fopen(fname, "r");
	}
	else{
		//fname = generate_filename(filename, file_no);
		sprintf(fname, "%s%s", filename, ".txt");
		fptr = fopen(fname, "r");
	}
	count = count_the_numbers_in_file(filename);
	FILE *fp1, *fp2;
	//generating two temp files
	char *f1 = (char*)malloc(sizeof(char) * 100);
	char *f2 = (char*)malloc(sizeof(char) * 100);
	char *f1_send = (char*)malloc(sizeof(char) * 100);
	char *f2_send = (char*)malloc(sizeof(char) * 100);
	if (count > number_limit){
		//f1 = generate_filename(filename, file_no + 1);
		//sprintf(f1, "%s%s", f1, ".txt");
		sprintf(f1, "%s%d%s", filename, file_no+1, ".txt");
		sprintf(f1_send, "%s%d", filename, file_no + 1);
		//f2 = generate_filename(filename, file_no + 3);
		//sprintf(f2, "%s%s", f2, ".txt");
		sprintf(f2, "%s%d%s", filename, file_no+2, ".txt");
		sprintf(f2_send, "%s%d", filename, file_no + 2);
		fp1 = fopen(f1, "w");
		fp2 = fopen(f2, "w");
		size = count / 2;
		i = 0;
		while (i < size){
			i += fscanf(fptr, "%d", &present_value);
			fprintf(fp1, "%d\n", present_value);
		}
		while (i < count){
			i += fscanf(fptr, "%d", &present_value);
			fprintf(fp2, "%d\n", present_value);
		}
		fclose(fp1);
		fclose(fp2);
		split(f1_send, file_no + 1);
		split(f2_send, file_no + 2);
		if (strcmp(f1_send, "input123456") == 0){
			printf("hi");
		}
		merge(f1_send, f2_send);
	}
	else{
		merge(filename, NULL);
	}
	
	fclose(fptr);
}

int main(){
	char *filename = (char *)malloc(sizeof(char) * 10);
	scanf("%s", filename);
	split(filename, 0);
	return 0;
}