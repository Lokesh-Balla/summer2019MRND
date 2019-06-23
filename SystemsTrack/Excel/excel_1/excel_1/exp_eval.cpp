#include "all_links.h"

int check_precedence(char c)
{
	if (c == '^')
		return 3;
	else if (c == '*' || c == '/')
		return 2;
	else if (c == '+' || c == '-')
		return 1;
	else
		return 4;
}

void push(struct stack *st, char data){
	if (st->top != st->capacity){
		st->top++;
		st->str[st->top] = data;
	}
}

void push_char(struct stack *st, char *data){
	if (st->top != st->capacity){
		st->top++;
		st->cell_adr[st->top] = (char*)malloc(sizeof(char) * 10);
		strcpy(st->cell_adr[st->top], data);
	}
}

char pop(struct stack *st){
	if (st->top != -1){
		st->top--;
	}
	return st->str[st->top + 1];
}

char *pop_char(struct stack *st){
	if (st->top != -1){
		st->top--;
	}
	return st->cell_adr[st->top + 1];
}


struct stack *create_stack(int capacity){
	struct stack *st = (struct stack *)malloc(sizeof(struct stack));
	st->top = -1;
	st->capacity = capacity;
	st->str = (char*)malloc(sizeof(char)*capacity);
	st->cell_adr = (char**)malloc(sizeof(char*)*capacity);
	return st;
}

char *generate_postfix(char *relation){
	struct stack *st = create_stack(100);
	char *postfix = (char *)malloc(sizeof(char) * 100);
	char *op = (char *)malloc(sizeof(char) * 100);
	int ind_postfix = 0, ind_op = 0;
	int i = 0;
	while (relation[i] != '\0'){
		if (relation[i] == '('){
			push(st, '(');
		}
		else if (relation[i] == ')'){
			op[ind_op] = '\0';
			//pushing previous operand
			ind_op = 0;
			while (op[ind_op] != '\0'){
				postfix[ind_postfix++] = op[ind_op];
				ind_op++;
			}
			if (ind_op > 0){
				postfix[ind_postfix++] = '|';
			}
			ind_op = 0;
			char c = pop(st);
			while (c != '('){
				postfix[ind_postfix++] = c;
				c = pop(st);
			}
		}
		else if (relation[i] != '+' && relation[i] != '-' && relation[i] != '*' && relation[i] != '/'){
			op[ind_op++] = relation[i];
		}
		else{
			op[ind_op] = '\0';
			//pushing previous operand
			ind_op = 0;
			while (op[ind_op] != '\0'){
				postfix[ind_postfix++] = op[ind_op];
				ind_op++;
			}
			if (ind_op > 0){
				postfix[ind_postfix++] = '|';
			}
			ind_op = 0;
			if (check_precedence(relation[i]) < check_precedence(st->str[st->top])){
				push(st, relation[i]);
			}
			else{
				char c = pop(st);
				while (st->top != -1 && check_precedence(c) < check_precedence(relation[i])){
					postfix[ind_postfix] = c;
					c = pop(st);
				}
				push(st, c);
				push(st, relation[i]);
			}
		}
		i++;
	}
	if (ind_op != 0){
		ind_op = 0;
		while (op[ind_op] != '\0'){
			postfix[ind_postfix++] = op[ind_op];
			ind_op++;
		}
		if (ind_op > 0){
			postfix[ind_postfix++] = '|';
		}
	}
	while (st->top != -1){
		postfix[ind_postfix++] = pop(st);
	}
	postfix[ind_postfix] = '\0';
	return postfix;
}

int evaluate_postfix(char *postfix, struct cell **sheet, char *char_arr, int *num_arr, int len){
	int i = 0, ind_op = 0;
	int result = 0;
	char *operand = (char*)malloc(sizeof(char) * 100);
	struct stack *eval = create_stack(100);
	while (postfix[i] != '\0'){
		if (postfix[i] == '+'){
			char *op1 = pop_char(eval);
			char *op2 = pop_char(eval);
			char *relation = (char*)malloc(sizeof(char) * 100);
			strcpy(relation, op2);
			strcat(relation, "+");
			strcat(relation, op1);
			result = print_formulae_value(sheet, relation, char_arr, num_arr, len);
			sprintf(relation, "%d", result);
			push_char(eval, relation);
		}
		else if (postfix[i] == '-'){
			char *op1 = pop_char(eval);
			char *op2 = pop_char(eval);
			char *relation = (char*)malloc(sizeof(char) * 100);
			strcpy(relation, op2);
			strcat(relation, "-");
			strcat(relation, op1);
			result = print_formulae_value(sheet, relation, char_arr, num_arr, len);
			sprintf(relation, "%d", result);
			push_char(eval, relation);
		}
		else if (postfix[i] == '*'){
			char *op1 = pop_char(eval);
			char *op2 = pop_char(eval);
			char *relation = (char*)malloc(sizeof(char) * 100);
			strcpy(relation, op2);
			strcat(relation, "*");
			strcat(relation, op1);
			result = print_formulae_value(sheet, relation, char_arr, num_arr, len);
			sprintf(relation, "%d", result);
			push_char(eval, relation);
		}
		else if (postfix[i] == '/'){
			char *op1 = pop_char(eval);
			char *op2 = pop_char(eval);
			char *relation = (char*)malloc(sizeof(char) * 100);
			strcpy(relation, op2);
			strcat(relation, "/");
			strcat(relation, op1);
			result = print_formulae_value(sheet, relation, char_arr, num_arr, len);
			sprintf(relation, "%d", result);
			push_char(eval, relation);
		}
		else if (postfix[i] == '%'){
			char *op1 = pop_char(eval);
			char *op2 = pop_char(eval);
			char *relation = (char*)malloc(sizeof(char) * 100);
			strcpy(relation, op2);
			strcat(relation, "%");
			strcat(relation, op1);
			result = print_formulae_value(sheet, relation, char_arr, num_arr, len);
			sprintf(relation, "%d", result);
			push_char(eval, relation);
		}
		else if (postfix[i] != '|'){
			operand[ind_op++] = postfix[i];
		}
		else{
			operand[ind_op] = '\0';
			ind_op = 0;
			push_char(eval, operand);
		}
		i++;
	}
	//printf("%d\n", result);
	free(operand);
	return result;
}
/*
void evaluate_postfix(char *postfix, struct cell **sheet){
	int i = 0, ind_op = 0;
	int result = 0;
	char *opernad = (char *)malloc(sizeof(char) * 100);
	struct stack *eval = create_stack(100);
	while (postfix[i] != '\0'){
		if (postfix[i] == '+'){
			int op1 = pop_int(eval);
			int op2 = pop_int(eval);
			result = op1 + op2;
			push_int(eval, result);
		}
		else if (postfix[i] == '-'){
			int op1 = pop_int(eval);
			int op2 = pop_int(eval);
			result = op1 - op2;
			push_int(eval, result);
		}
		else if (postfix[i] == '*'){
			int op1 = pop_int(eval);
			int op2 = pop_int(eval);
			result = op1 * op2;
			push_int(eval, result);
		}
		else if (postfix[i] == '/'){
			int op1 = pop_int(eval);
			int op2 = pop_int(eval);
			result = op1 / op2;
			push_int(eval, result);
		}
		else if (postfix[i] != '|'){
			opernad[ind_op++] = postfix[i];
		}
		else{
			opernad[ind_op] = '\0';
			ind_op = 0;
			if (postfix[i + 1] != '\0'){
				result = GET(sheet, opernad, char_arr, num_arr, len)
			}
			push_int(eval, result);
		}
		i++;
	}
	printf("%d", result);
}
*/