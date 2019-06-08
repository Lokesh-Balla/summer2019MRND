#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int check_word(char matrix[2][5], char *word, int r, int c,int rows,int cols,int op1,int op2, int index,int sr,int sc){
	if (index == strlen(word) - 1 && word[index] == matrix[r][c]){
		printf("starts at %d %d and ends at %d %d\n", sr, sc, r, c);
		return 1;
	}
	if (r < rows && c < cols && r >=0 && c>=0){
		if (index < strlen(word) && word[index] == matrix[r][c]){
			return check_word(matrix, word, r + op1, c + op2, rows, cols, op1, op2, index + 1, sr, sc);
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

int matrix_backtrack_v2(char matrix[2][5], char *word, int rows, int cols){
	int count = 0;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			if (word[0] == matrix[i][j]){
				for (int op1= -1; op1 <= 1; op1++)
				{
					for (int op2 = -1; op2 <= 1; op2++)
					{
						if (!(op1 == 0 && op2 == 0) && ){
							count += check_word(matrix, word, i, j, rows, cols, op1, op2, 0, i, j);
						}
						
					}
				}
			}
		}
	}
	return count;
}


int matrix_backtrack(char matrix[2][5], char *word, int rows, int cols, int r, int c, int i,int dir){
	int count = 0;
	while (r < rows && c < cols){
		if (i == strlen(word)-1 && word[i] == matrix[r][c]){
			printf("end %d %d ",r,c);
			return 1;
			break;
		}
		if (i < strlen(word) && word[i] == matrix[r][c]){
			if (dir == 0){
				printf("start %d %d ", r, c);
				count+=matrix_backtrack(matrix, word, rows, cols, r + 1, c, i + 1,1);
				count+=matrix_backtrack(matrix, word, rows, cols, r, c + 1, i + 1,2);
				count+=matrix_backtrack(matrix, word, rows, cols, r + 1, c + 1, i + 1,3);
				count+=matrix_backtrack(matrix, word, rows, cols, r - 1, c + 1, i + 1,4);
				count+=matrix_backtrack(matrix, word, rows, cols, r + 1, c - 1, i + 1,5);
				count+=matrix_backtrack(matrix, word, rows, cols, r - 1, c, i + 1, 6);
				count+=matrix_backtrack(matrix, word, rows, cols, r, c - 1, i + 1, 7);
				count+=matrix_backtrack(matrix, word, rows, cols, r - 1, c - 1, i + 1, 8);
				
			}
			if (dir == 1){
				count+=matrix_backtrack(matrix, word, rows, cols, r + 1, c, i + 1, 1);
				break;
			}
			else if (dir == 2){
				count+=matrix_backtrack(matrix, word, rows, cols, r, c + 1, i + 1, 2);
				break;
			}
			else if (dir == 3){
				count+=matrix_backtrack(matrix, word, rows, cols, r + 1, c + 1, i + 1, 3);
				break;
			}
			else if (dir == 4){
				count+=matrix_backtrack(matrix, word, rows, cols, r - 1, c + 1, i + 1, 4);
				break;
			}
			else if (dir == 5){
				count+=matrix_backtrack(matrix, word, rows, cols, r + 1, c - 1, i + 1, 5);
				break;
			}
			else if(dir==6){
				count+=matrix_backtrack(matrix, word, rows, cols, r - 1, c, i + 1, 6);
				break;
			}
			else if (dir == 7){
				count+=matrix_backtrack(matrix, word, rows, cols, r, c - 1, i + 1, 7);
				break;
			}
			else if (dir == 8){
				count+=matrix_backtrack(matrix, word, rows, cols, r - 1, c - 1, i + 1, 8);
				break;
			}
		}
		if (dir != 0){
			count = 0;
			break;
		}
		c++;
		if (c == cols){
			r++;
			c = 0;
		}
	}
	return count;
}


int main(){
	char matrix[4][4] = { { 'a', 'l', 'b', 'x' }, 
						 { 'a', 'l', 'e', 'b' }, 
						 { 'a', 'e', 'l', 'b' },
						 { 'x', 'z', 'y', 'b' } };
	char  matrix1[2][5] = { { 'h', 'i', 'h', 'i', 'h' },
							{ 'e', 'e', 'h', 'e', 'e' } };

	
	//printf("The count is %d\n",matrix_backtrack(matrix1, "hi", 2, 5, 0, 0, 0, 0));
	printf("the count is %d\n", matrix_backtrack_v2(matrix1, "e", 2, 5));


	return 0;
}