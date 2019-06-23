#include<stdio.h>
#include<stdlib.h>


int *sort_with_sum(int *arr, int len){
	int sum = 0;
	int *res = (int *)malloc(sizeof(int)*len);
	for (int i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	for (int i = 0; i < len-sum; i++)
	{
		res[i] = 0;
	}
	for (int i = len - sum; i < len; i++)
	{
		res[i] = 1;
	}
	return res;
}

int *sort_with_count(int *arr, int len){
	int *res = (int *)malloc(sizeof(int)*len);
	int count_of_one = 0;
	for (int i = 0; i < len; i++)
	{
		if (arr[i] == 1)
			count_of_one++;
	}
	for (int i = 0; i < len - count_of_one; i++)
	{
		res[i] = 0;
	}
	for (int i = len - count_of_one; i < len; i++)
	{
		res[i] = 1;
	}
	return res;
}

int *sort_with_swap_from_two_sides(int *arr, int length){
	int start = 0, end = length - 1;
	while (start < end){
		if (arr[start] == 1 && arr[end] == 0){
			arr[start] = 0;
			arr[end] = 1;
			start++;
			end--;
			continue;
		}
		if (arr[start] == 0){
			start++;
		}
		if (arr[end] == 1){
			end--;
		}
	}
	return arr;
}

int *sort_three_colors(int *arr, int len){
	int count_zero = 0, count_one = 0;
	int *res = (int *)malloc(sizeof(int)*len);
	for (int i = 0; i < len; i++)
	{
		if (arr[i] == 0)
			count_zero++;
		if (arr[i] == 1)
			count_one++;
	}
	for (int i = 0; i < count_zero; i++)
	{
		res[i] = 0;
	}
	for (int i = count_zero; i < count_zero+count_one; i++)
	{
		res[i] = 1;
	}
	for (int i = count_zero+count_one; i < len; i++)
	{
		res[i] = 2;
	}
	return res;
}

int main(){
	int elements_number;
	printf("enter the number of elements that will be present\n");
	scanf("%d", &elements_number);
	int *arr = (int *)malloc(sizeof(int)*elements_number);
	printf("enter the elements(only 0 and 1 are allowed)\n");
	for (int i = 0; i < elements_number; i++){
		scanf_s("%d", &arr[i]);
	}
	int *res1 = sort_with_sum(arr, elements_number);
	for (int i = 0; i < elements_number; i++)
	{
		printf("%d ", res1[i]);
	}
	printf("\n");
	int *res2 = sort_with_sum(arr, elements_number);
	for (int i = 0; i < elements_number; i++)
	{
		printf("%d ", res2[i]);
	}
	printf("\n");
	int *res3 = sort_with_swap_from_two_sides(arr, elements_number);
	for (int i = 0; i < elements_number; i++)
	{
		printf("%d ", res3[i]);
	}

	printf("enter the number of elements that will be present\n");
	scanf("%d", &elements_number);
	int *arr1 = (int *)malloc(sizeof(int)*elements_number);
	printf("enter the elements(only 0, 1 and 2 are allowed)\n");
	for (int i = 0; i < elements_number; i++){
		scanf_s("%d", &arr1[i]);
	}
	int *res4 = sort_three_colors(arr1, elements_number);
	for (int i = 0; i < elements_number; i++)
	{
		printf("%d ", res4[i]);
	}
	getchar();
	return 0;
}