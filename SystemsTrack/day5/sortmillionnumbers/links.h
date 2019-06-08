#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

//support
char *generate_filename(char *fname, int num);
int count_the_numbers_in_file(char *filename);
int *sort_the_numbers(int *arr, int n);
int *convert_to_array(char *filename);

//sortmillion
void split(char *filename, int file_no);
void merge(char *file1, char *file2);