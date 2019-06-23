#include "all_links.h"

struct cell **create_sheet(int r, int c){
	struct cell **sheet = (struct cell **)malloc(sizeof(struct cell*)*r);
	for (int i = 0; i < r; i++)
	{
		sheet[i] = (struct cell *)malloc(sizeof(struct cell)*c);
		for (int j = 0; j < c; j++)
		{
			sheet[i][j].data = 0;
			sheet[i][j].flag = 0;
		}
	}
	return sheet;
}

int no_of_operators(char *relation){
	int i = 0, count = 0;
	while (relation[i] != '\0'){
		if (relation[i] == '+' || relation[i] == '-' || relation[i] == '*' || relation[i] == '/' || relation[i] == '%' || relation[i] == '(' || relation[i] == ')'){
			count++;
		}
		i++;
	}
	return count;
}

int is_expression(char *relation){
	int i = 0;
	while (relation[i] != '\0'){
		if (relation[i] >= 'A'&&relation[i] <= 'Z' || relation[i] == '+' || relation[i] == '-' || relation[i] == '*' || relation[i] == '/'){
			return 1;
		}
		i++;
	}
	return 0;
}

void print_sheet(struct cell **sheet){
	int value = 0, count = 0;
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++)
		{
			value = sheet[i][j].data;
			while (value != 0){
				count++;
				value = value / 10;
			}
			if (sheet[i][j].flag == 0){
				printf("|%d", sheet[i][j].data);
			}
			else{
				char *chr_arr = NULL;
				int *num_arr = NULL;
				printf("|%d", print_formulae_value(sheet,sheet[i][j].formula,chr_arr,num_arr,0));
			}
			printf("|");
		}
		printf("\n");
	}
}

int print_formulae_value(struct cell **sheet,char *relation, char *char_arr, int *num_arr, int len){
	int i = 0, j = 0;
	int flag = 0;
	int res = 0;
	if (no_of_operators(relation) > 1){
		relation = generate_postfix(relation);
		res = evaluate_postfix(relation, sheet, char_arr, num_arr, len);
		return res;
	}
	char oper = ' ';
	char *op1 = (char *)malloc(sizeof(char) * 10);
	char *op2 = (char *)malloc(sizeof(char) * 10);
	while (relation[i] != 0){
		if (relation[i] != '+' && relation[i] != '*' && relation[i] != '-' && relation[i] != '/' && flag == 0){
			op1[i] = relation[i];
			op1[i + 1] = '\0';
		}
		else if (relation[i] == '+' || relation[i] == '*' || relation[i] == '-' || relation[i] == '/' && flag == 0){
			flag = 1;
			oper = relation[i];
			op1[i] = '\0';
		}
		else if (relation[i] != '+' && relation[i] != '*' && relation[i] != '-' && relation[i] != '/' && flag == 1){
			op2[j++] = relation[i];
		}
		i++;
	}
	op2[j] = '\0';
	int limit = len;
	for (i = 0; i < limit; i++){
		char *compare = (char *)malloc(sizeof(char) * 10);
		sprintf(compare, "%c%d", char_arr[i],num_arr[i]);
		if (strcmp(op1, compare) == 0 || strcmp(op2, compare) == 0){
			printf("\nDeadlock detected\n");
			char_arr[0] = 'x';
			char_arr[1] = 'x';
			return 0;
		}
		else{
			if (is_expression(op1) == 1){
				char_arr[len] = op1[0] - '0';
				int num = 0;
				j = 1;
				while (op1[j] != '\0'){
					num = num * 10 + (op1[j] - '0');
					j++;
				}
				num_arr[len] = num;
				len = len + 1;
			}
			if (is_expression(op2) == 1){
				char_arr[len] = op2[0] - '0';
				int num = 0;
				j = 1;
				while (op2[j] != '\0'){
					num = num * 10 + (op2[j] - '0');
					j++;
				}
				num_arr[len] = num;
				len = len+1;
			}
		}
	}
	if (oper == '+'){
		return GET(sheet, op1, char_arr, num_arr, len) + GET(sheet, op2, char_arr, num_arr, len);
	}
	else if (oper == '-'){
		return GET(sheet, op1, char_arr, num_arr, len) - GET(sheet, op2, char_arr, num_arr, len);
	}
	else if (oper == '*'){
		return GET(sheet, op1, char_arr, num_arr, len) * GET(sheet, op2, char_arr, num_arr, len);
	}
	else if (oper == '/'){
		return GET(sheet, op1, char_arr, num_arr, len) / GET(sheet, op2, char_arr, num_arr, len);
	}
	else if (oper == '%'){
		return GET(sheet, op1, char_arr, num_arr, len) % GET(sheet, op2, char_arr, num_arr, len);
	}
	else if (oper == ' '){
		return GET(sheet, op1, char_arr, num_arr, len);
	}
}


int GET(struct cell **sheet, char *cell, char *char_arr, int *num_arr, int len){
	//if only number return
	int is_exp = is_expression(cell);
	if (is_exp == 0){
		int num = 0, i = 0;
		while (cell[i] != '\0'){
			num = num * 10 + (cell[i] - '0');
			i++;
		}
		return num;
	}
	int col = cell[0] - 'A';
	int i = 1;
	int row = 0;
	while (cell[i] != '\0' && cell[i] != '\n'){
		if (cell[i] == '0'){
			row = row * 10;
		}
		else{
			row = row * 10 + cell[i] - '0';
		}
		i++;
	}
	if (sheet[row - 1][col].flag == 0){
		return sheet[row - 1][col].data;
	}
	else{
		return print_formulae_value(sheet,sheet[row - 1][col].formula,char_arr, num_arr,len);
	}
}

int check_deadlock(struct cell **sheet, char *relation, int row, int col){
	char *char_arr = (char*)malloc(sizeof(char)*100);
	int *num_arr = (int*)malloc(sizeof(char) * 100);
	char_arr[0] = 'A' + col;
	num_arr[0] = row;
	//int n = print_formulae_value(sheet, relation, char_arr, num_arr, 1);
	relation = generate_postfix(relation);
	int res = evaluate_postfix(relation, sheet, char_arr, num_arr, 1);
	if (char_arr[0] == 'x' && char_arr[1] == 'x'){
		return 1;
	}
	return 0;
}

struct cell **SET(struct cell **sheet, int row, int col, int value, char *relation){
	if (relation == NULL){
		sheet[row - 1][col].data = value;
		sheet[row - 1][col].flag = 0;
		return sheet;
	}
	else{
		int result = check_deadlock(sheet, relation, row, col);
		if (result != 1){
			strcpy(sheet[row - 1][col].formula, relation);
			sheet[row - 1][col].flag = 1;
		}
		return sheet;
	}

}



void EXPORT(struct cell **sheet, char *argument){
	FILE *fp;
	int k = 0, flag = 0; 
	fp = fopen(argument, "w");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (sheet[i][j].flag == 0){
				fprintf(fp, "%d,", sheet[i][j].data);
			}
			else{
				fprintf(fp, "%s,", sheet[i][j].formula);
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

struct cell **IMPORT(struct cell **sheet, char *argument){
	FILE *fp;
	fp = fopen(argument, "r");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			char c;
			int k = 0;
			char *str = (char *)malloc(sizeof(str) * 10);
			while ((c = fgetc(fp)) != ',' && c != EOF && c != '\n'){
				str[k++] = c;
			}
			str[k] = '\0';
			if (strcmp(str,"") == 0){
				j--;
				continue;
			}
			int exp = is_expression(str);
			if (exp == 0){
				int num = 0;
				k = 0;
				while (str[k] != '\0'){
					num = num * 10 + (str[k] - '0');
					k++;
				}
				sheet[i][j].data = num;
				sheet[i][j].flag = 0;
			}
			else{
				strcpy(sheet[i][j].formula, str);
				sheet[i][j].flag = 1;
			}
			free(str);
			
		}
	}
	fclose(fp);
	return sheet;
}

char *check_and_add_csv(char *filename){
	char *str = (char *)malloc(sizeof(char) * 100);
	int i = 0, flag = 0;
	while (filename[i] != '\0'){
		if (filename[i] == '.'&&filename[i + 1] == 'c'&&filename[i + 2] == 's'&&filename[i + 3] == 'v'){
			flag = 1;
			break;
		}
		i++;
	}
	if (flag == 1)
	{
		return filename;
	}
	else{
		strcpy(str, filename);
		strcat(str, ".csv");
		return str;
	}
}

struct cell **parse_command(char *command, struct cell **sheet, char *filename){
	int space = 0;
	//converting to upper
	char *backup = (char *)malloc(sizeof(char) * 100);
	strcpy(backup, command);
	int i = 0, j = 0, option_len = 0;
	while (command[i] != '\0'){
		if (command[i] == '\n'){
			command[i] = '\0';
			break;
		}
		command[i] = toupper(command[i]);
		i++;
	}
	char *option = (char *)malloc(sizeof(char) * 100);
	char *arguments = (char *)malloc(sizeof(char) * 100);
	char *without_change = (char *)malloc(sizeof(char) * 100);
	i = 0;
	while (command[i] != '\0'){
		if (command[i] != ' ' && space == 0){
			option[i] = command[i];
			option_len = i;
		}
		else if (command[i] == ' ' && space == 0){
			option[i] = '\0';
			space = 1;
		}
		else if (command[i] != ' ' && space == 1){
			arguments[j] = command[i];
			without_change[j++] = backup[i];
		}
		else if (command[i] == ' ' && space == 1){
			printf("Invlaid command format\n");
			return sheet;
		}
		i++;
	}
	arguments[j] = '\0';
	without_change[j] = '\0';
	option[option_len + 1] = '\0';
	i = 0, j = 0;
	if (strcmp(option, "SET") == 0){
		//parsing the arguments
		int equ = 0;
		char *rc = (char *)malloc(sizeof(char) * 5);
		int value = 0;
		int row = 0, col = 0;
		i = 0;
		int eq = 0;
		j = 0;
		char *relation = (char *)malloc(sizeof(char) * 50);
		while (arguments[i] != '\0'){
			if (arguments[i] != '=' && eq == 0){
				rc[i] = arguments[i];
			}
			else if (arguments[i] == '=' && eq == 0){
				eq = 1;
				rc[i] = '\0';
			}
			else if (arguments[i] != '=' && eq == 1){
				relation[j++] = arguments[i];
			}
			i++;
		}
		relation[j] = '\0';
		col = rc[0] - 'A';
		i = 1;
		while (rc[i] != '\0'){
			if (rc[i] == '0'){
				row = row * 10;
			}
			else{
				row = row * 10 + (rc[i] - '0');
			}
			i++;
		}
		int has_realtion = is_expression(relation);
		if (has_realtion != 1){
			int value = 0, i = 0;
			while (relation[i] != '\0'){
				value = value * 10 + (relation[i] - '0');
				i++;
			}
			sheet = SET(sheet, row, col, value, NULL);
		}
		else{
			sheet = SET(sheet, row, col, value, relation);
		}
		free(rc);
	}
	else if (strcmp(option, "GET") == 0){
		char *dummy = NULL;
		int *dummy1 = NULL;
		int res=GET(sheet, arguments, dummy, dummy1, 0);
		printf("%d", res);
		//arguments = generate_postfix(arguments);
		//evaluate_postfix(arguments, sheet, dummy, dummy1, 0);
	}
	else if (strcmp(option, "PRINT") == 0){
		print_sheet(sheet);
	}
	else if (strcmp(option, "EXPORT") == 0){
		without_change = check_and_add_csv(without_change);
		strcpy(filename, without_change);
		EXPORT(sheet, without_change);
	}
	else if (strcmp(option, "IMPORT") == 0){
		without_change = check_and_add_csv(without_change);
		strcpy(filename, without_change);
		sheet = IMPORT(sheet, without_change);
	}
	else if (strcmp(option, "SAVE") == 0){
		if (strcmp(filename,"0000")!=0){
			EXPORT(sheet, filename);
		}
	}
	else if (strcmp(option, "EXIT") == 0){
		exit(0);
	}
	return sheet;
}

void display_console(struct cell **sheet){
	char *command = (char *)malloc(sizeof(char) * 50);
	char *filename = (char *)malloc(sizeof(char) * 50);
	strcpy(filename, "0000");
	while (true){
		printf("\n>");
		fgets(command, 100, stdin);
		parse_command(command, sheet, filename);
	}
	free(filename);
	free(command);
}

int main(){
	int rows = 10, cols = 10;
	struct cell **sheet = create_sheet(rows, cols);
	display_console(sheet);
	return 0;
}