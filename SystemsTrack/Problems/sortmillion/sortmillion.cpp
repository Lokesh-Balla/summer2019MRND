#include "links.h"

void print_numbers(FILE **fptr, int start){
	int *arr = (int*)malloc(sizeof(int) * 250000);
	int present_value = 0;
	for (int i = start; i < start + 1000000; i++){
		fscanf(*fptr, "%d", &present_value);
		if (present_value >= start&&present_value <= start + 1000000){
			arr = setbit(arr, present_value);
		}
		
	}
	for (int i = start; i < start + 1000000; i++){
		if (testbit(arr, i) == 1){
			printf("%d\n", i);
		}
	}

}

void sort_million(){
	char *filename = (char*)malloc(sizeof(char) * 100);
	scanf("%s", filename);
	int start = 0, jump = 1000000;
	FILE *fptr = fopen(filename, "r");
	for (int i = start; i < 10000000; i += jump){
		print_numbers(&fptr, start);
	}
	free(filename);
	fclose(fptr);
}

int main(){
	sort_million();
	return 0;
}