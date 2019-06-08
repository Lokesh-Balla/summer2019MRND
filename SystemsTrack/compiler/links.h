#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define START_ADDRESS 1000
#define ENTRIES_LIMIT 100
#define OUTPUT_FILE_NAME "output.txt"


struct stack{
	int top;
	int capacity;
	unsigned int *lst;
};

struct symbol_table{
	char name[20];
	unsigned int address;
	unsigned int size;
};

struct inter_lang{
	unsigned in_num;
	unsigned op_code;
	unsigned parameters[10];
	unsigned parameter_len;
};

struct label_table{
	char block_name[20];
	unsigned int address;
};

//compiler
void build_symbol_table(char *command, char **tokens, int tokens_len, struct symbol_table *entries, int entries_len);
int tokenizer(char *line, char *command, char **tokens);
void readfile();

//support
void trim(char *file);
char *remove_newline(char *str);
int check_register(char *regis);
int register_number(char *ch);
struct stack *create_stack(int size);
void push(struct stack *stack, unsigned int value);
int pop(struct stack *stack);
int check_label(char *command);
char *get_block_name(char *str);
int check_in_label_table(char *x,struct label_table **label, unsigned int index);
int no_of_spaces(char *str);