#include "links.h"

//removes all the spaces and ',' in the string not just begining and ending 
void trim(char *file){
	int i = 0, s = 0;
	while (file[i] != NULL){
		if (file[i] == ' '){
			s++;
		}
		file[i] = file[i + s];
		i++;
	}
	file[i + s] = '\0';
}

//removes newline charcter at the end of the file
char *remove_newline(char *str){
	int i = 0;
	while (str[i] != '\0'){
		if (str[i] == '\n'){
			str[i] = '\0';
			break;
		}
		i++;
	}
	return str;
}

//check if it is a register or not
int check_register(char *regis){
	if (regis[1] == 'x' || regis[1] == 'X'){
		return 1;
	}
	return 0;
}

//get register number
int register_number(char *ch){
	char *registers[] = { "ax", "bx", "cx", "dx", "ex", "fx", "gx", "hx" };
	char *registers1[] = { "AX", "BX", "CX", "DX", "EX", "FX", "GX", "HX" };
	for (int i = 0; i < 8; i++){
		if (strcmp(registers[i], ch) == 0 || strcmp(registers1[i],ch) == 0){
			return i;
		}
	}
	return -1;
}

//stack implementation

//init the stack
struct stack *create_stack(int size){
	struct stack *stack = (struct stack*)malloc(sizeof(struct stack));
	stack->capacity = size;
	stack->top = -1;
	stack->lst = (unsigned int *)malloc(sizeof(unsigned int)*size);
	return stack;
}

void push(struct stack *stack, unsigned int value){
	if (stack->top != stack->capacity){
		stack->top++;
		stack->lst[stack->top] = value;
	}
}

int pop(struct stack *stack){
	if (stack->top != -1){
		stack->top--;
		return stack->lst[stack->top + 1];
	}
	return 0;
}

//check if the command is a LABEL
int check_label(char *command){
	int i = 0;
	while (command[i] != '\0'){
		if (command[i] == ':'){
			return 1;
		}
		i++;
	}
	return 0;
}

char *get_block_name(char *str){
	int i = 0;
	while (str[i] != '\0'){
		if (str[i] == ':'){
			str[i] = '\0';
			return str;
		}
		i++;
	}
}

//check in label table and return the address
int check_in_label_table(char *x,struct label_table **label, unsigned int index){
	for (int i = 0; i < index; i++){
		if (strcmp(x, label[i]->block_name) == 0){
			return label[i]->address;
		}
	}
	return -1;
}

//return no of spaces in the start
int no_of_spaces(char *str){
	int i = 0, count = 0;
	while (str[i] == ' '){
		count++;
		i++;
	}
	return count;
}