#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <iterator> 
#include <unordered_map>
#include <set>

using namespace std;

long a_power_n(long a, long n){
	if (n == 1){
		return a;
	}
	else if (n % 2 == 0){
		return a_power_n(a, n / 2)*a_power_n(a, n / 2);
	}
	else{
		return a_power_n(a, n / 2)*a_power_n(a, n / 2) * a;
	}
}

int **multiply_mat(int **a, int **res, int n){
	if (n == 1){
		return a;
	}
	else if (n == 2){
		res[0][0] = a[0][0] * a[0][0] + a[0][1] * a[1][0];
		res[0][1] = a[0][0] * a[0][1] + a[0][1] * a[1][1];
		res[1][0] = a[1][0] * a[0][0] + a[1][1] * a[1][0];
		res[1][1] = a[1][0] * a[0][1] + a[1][1] * a[1][1];
		return res;
	}
	else{
		return multiply_mat(a, res, n - 1);
	}
	
}

long fib_using_mat(int n){
	int **mat1 = (int **)malloc(sizeof(int*) * 2);
	mat1[0] = (int *)malloc(sizeof(int) * 2);
	mat1[1] = (int *)malloc(sizeof(int) * 2);
	mat1[0][0] = 0;
	mat1[0][1] = 1;
	mat1[1][0] = 1;
	mat1[1][1] = 1;
	int mat2[2][1] = { { 0 }, { 1 } };
	int **res = (int **)malloc(sizeof(int) * 2);
	res[0] = (int *)malloc(sizeof(int) * 2);
	res[1] = (int *)malloc(sizeof(int) * 2);
	res = multiply_mat(mat1, res, n);
	printf("");
	return 5;
}

long sum_of_arr(int arr[],int len){
	int sum = 0;
	if (len >= 0){
		sum = arr[len] + sum_of_arr(arr, len-1);
	}
	return sum;
}

long fibonacii(int k,std::unordered_map<long,long> mem){
	long val1 = 0, val2 = 0;
	if (k <= 1){
		return k;
	}
	if (mem[k - 1] != NULL){
		val1 = mem[k - 1];
	}
	else{
		val1 = fibonacii(k - 1,mem);
		mem[k - 1] = val1;
	}
	if (mem[k - 2] != NULL){
		val2 = mem[k - 2];
	}
	else{
		val2 = fibonacii(k - 2,mem);
		mem[k - 2] = val2;
	}
	mem[k] = val1 + val2;
	return val1 + val2;
}

long no_of_ways_steps(long steps, long k){
	long total_ways = 0;
	if (steps <= 1){
		return 1;
	}
	for (int i = 1; i <= k; i++){
		total_ways += no_of_ways_steps(steps - i, k);
	}
	return total_ways;
}

void reverse_text(int arr[],int len){
	int i = 0;
	int flag = 0;
	std::unordered_map<int, char> rev_char;
	rev_char[0] = '0';
	rev_char[1] = '1';
	rev_char[2] = '2';
	rev_char[3] = 'E';
	rev_char[4] = 'h';
	rev_char[5] = 'S';
	rev_char[6] = '9';
	rev_char[7] = 'L';
	rev_char[8] = '8';
	rev_char[9] = '6';
	char *str = (char *)malloc(sizeof(char) * (len+1));
	for (i = 0; i < len; i++){
		if (arr[i]>0){
			flag = 1;
		}
		if (flag==0 && arr[i] == 0){
			str[i] = ' ';
			continue;
		}
		str[i] = rev_char[arr[i]];
	}
	str[i] = '\0';
	printf("%s ", str);
	free(str);
}

void print_n_digit_numbers(int i, int limit, int arr[]){
	int flag = 0;
	if (i < limit){
		for (int j = 0; j <= 9; j++){
			arr[i] = j;
			print_n_digit_numbers(i + 1, limit, arr);
		}
	}
	if (i == limit){
		for (int k = 0; k < limit; k++)
		{
			if (arr[k]>0){
				flag = 1;
			}
			if (flag == 0 && arr[k] == 0){
				continue;
			}
			printf("%d", arr[k]);
		}
		printf(" ");
	}
}

void print_n_digit_numbers_reverse(int i, int limit, int arr[]){

	if (i < limit){
		for (int j = 0; j <= 9; j++){
			arr[i] = j;
			print_n_digit_numbers_reverse(i + 1, limit, arr);
		}
	}
	if (i == limit){
		reverse_text(arr, limit);
	}
}

void permutations(int i, int arr[], int res[], int len){
	if (i < len){
		for (int j = 0; j < len; j++)
		{
			res[i] = arr[j];
			permutations(i + 1, arr, res, len-1);
		}
	}
	if (i == len){
		for (int k = 0; k < len; k++)
		{
			
			printf("%d", res[k]);
		}
		printf(" ");
	}
}


void permutation1(int arr[], int start, int len){
	if (start == len){
		for (int i = 0; i <= len; i++)
		{
			printf("%d", arr[i]);
		}
		printf("\n");
	}
	for (int i = start; i <= len; i++){
			int temp = arr[i];
			arr[i] = arr[len];
			arr[len] = temp;
			permutation1(arr, start + 1, len);
			temp = arr[i];
			arr[i] = arr[len];
			arr[len] = temp;
	}
}




int main(){
	int arr[] = { 1, 2, 3, 4, 5 };
	//printf("%d", sum_of_arr(arr, 4));
	//std::unordered_map<long, long> mem;
	//printf("%d", fibonacii(5,mem));
	//printf("%d", no_of_ways_steps(5, 2));
	
	
	//int *arr1 = (int *)malloc(sizeof(int) * 20);
	//print_n_digit_numbers(0, 3, arr1);
	//print_n_digit_numbers_reverse(0, 3, arr1);
	
	
	//int permutation_arr[] = { 1, 3, 5 };
	//int *res = (int *)malloc(sizeof(int) * 10);
	//permutations(0, permutation_arr, res, 3);
	//permutation1(permutation_arr, 0, 2);

	//printf("%ld\n", a_power_n(3, 3));

	printf("%ld\n", fib_using_mat(3));
	
	return 0;
}