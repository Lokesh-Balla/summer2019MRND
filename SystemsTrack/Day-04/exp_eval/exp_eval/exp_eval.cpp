#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int get(char op, struct cell *list);

struct cell{
	int data;
	char formula[100];
	int flag;
};

struct stack{
	int top;
	int capacity;
	char *str;
	int *arr;
};

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

void push_int(struct stack *st, int data){
	if (st->top != st->capacity){
		st->top++;
		st->arr[st->top] = data;
	}
}

char pop(struct stack *st){
	if (st->top != -1){
		st->top--;
	}
	return st->str[st->top + 1];
}

int pop_int(struct stack *st){
	if (st->top != -1){
		st->top--;
	}
	return st->arr[st->top + 1];
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

struct stack *create_stack(int capacity){
	struct stack *st = (struct stack *)malloc(sizeof(struct stack));
	st->top = -1;
	st->capacity = capacity;
	st->str = (char*)malloc(sizeof(char)*capacity);
	st->arr = (int *)malloc(sizeof(char)*capacity);
	return st;
}

void evaluate_postfix(char *postfix,struct cell *list){
	int i = 0, ind_op = 0;
	int result = 0;
	char *opernad = (char *)malloc(sizeof(char) * 100);
	struct stack *eval = create_stack(100);
	while (postfix[i]!='\0'){
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
				result = get(opernad[0], list);
			}
			push_int(eval, result);
		}
		i++;
	}
	printf("%d",result);
}

int get(char op,struct cell *list){
	if (op >= '0' && op <= '9'){
		return op - '0';
	}
	return list[op - 'a'].data;
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

void parse_command(char *command, struct cell *list)
{
	int i = 0, j = 0;
	char *varaible = (char *)malloc(sizeof(char) * 100);
	char *relation = (char *)malloc(sizeof(char) * 100);
	while (command[i] != '='){
		varaible[i] = command[i];
		i++;
	}
	varaible[i] = '\0';
	i++;
	while (command[i] != '\0'){
		if (command[i] == '\n'){
			command[i] = '\0';
		}
		relation[j++] = command[i++];
	}
	relation[j] = '\0';
	char var;
	i = 0;
	while (varaible[i] != '\0'){
		var = varaible[i];
		i++;
	}
	int exp_check = is_expression(relation);
	if (exp_check == 0){
		list[var - 'a'].flag = 0;
		i = 0;
		int num = 0;
		while (relation[i] != '\0'){
			num = num * 10 + (relation[i] - '0');
			i++;
		}
		list[var - 'a'].data = num;
	}
	else{
		list[var - 'a'].flag = 1;
		strcpy(list[var - 'a'].formula, relation);
		char *res = generate_postfix(relation);
		printf("%s\n", res);
		evaluate_postfix(res, list);
	}
	free(relation);
	free(varaible);
}

void display_console(){
	char *command = (char *)malloc(sizeof(char) * 50);
	struct cell *list = (struct cell*)malloc(sizeof(struct cell) * 26);
	while (true){
		printf("\n>");
		fgets(command, 100, stdin);
		parse_command(command, list);
	}
	free(command);
}

int main(){
	display_console();
	return 0;
}