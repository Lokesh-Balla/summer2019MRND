#include<stdio.h>
#include<stdlib.h>


void matrix_transpose(){
	int m = 0, n = 0;
	printf("enter rows and col");
	scanf_s("%d %d", &m, &n);
	int **arr = (int **)malloc(sizeof(int *)*m);
	printf("enter the elements");
	for (int i = 0; i < m; i++){
		arr[i] = (int *)malloc(sizeof(int)*n);
		for (int j = 0; j < n; j++){
			scanf_s("%d", &arr[i][j]);
		}
	}
	int **res = (int **)malloc(sizeof(int *)*n);
	for (int i = 0; i < n; i++){
		res[i] = (int *)malloc(sizeof(int)*m);
		for (int j = 0; j < m; j++){
			res[i][j] = arr[j][i];
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			printf_s("%d", res[i][j]);
		}
		printf("\n");
	}
	getchar();
}

void spiral_matrix(){
	int m = 0, n = 0;
	printf("enter rows and col");
	scanf_s("%d %d", &m, &n);
	int **arr = (int **)malloc(sizeof(int *)*m);
	printf("enter the elements");
	for (int i = 0; i < m; i++){
		arr[i] = (int *)malloc(sizeof(int)*n);
		for (int j = 0; j < n; j++){
			scanf_s("%d", &arr[i][j]);
		}
	}
	int minr = 0, minc = 0, maxr = m - 1, maxc = n - 1;
	int count = 0, ind = 0;
	int res[9];
	while (count < m*n){
		for (int i = minc; i <= maxc; i++){
			res[ind++] = arr[minr][i];
			count++;
		}
		minr++;
		for (int i = minr; i <= maxr; i++){
			res[ind++] = arr[i][maxc];
			count++;
		}
		maxc--;
		for (int i = maxc; i >= minc; i--){
			res[ind++] = arr[maxr][i];
			count++;
		}
		maxr--;
		for (int i = maxr; i >= minr; i--){
			res[ind++] = arr[i][minc];
			count++;
		}
		minc++;
	}
	for (int i = 0; i < m*n; i++){
		printf("%d", res[i]);
	}
	getchar();
}


int main(){
	//matrix_transpose();
	spiral_matrix();
	return 0;
}