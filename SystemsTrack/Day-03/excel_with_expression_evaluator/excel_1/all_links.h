#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>


int GET(struct cell **sheet, char *cell, char *char_arr, int *num_arr, int len);
struct cell **create_sheet(int r, int c);
int is_expression(char *relation);
void print_sheet(struct cell **sheet);
int print_formulae_value(struct cell **sheet, char *relation, char *char_arr, int *num_arr, int len);
int check_deadlock(struct cell **sheet, char *relation, int row, int col);
struct cell **SET(struct cell **sheet, int row, int col, int value, char *relation);
void EXPORT(struct cell **sheet, char *argument);
struct cell **IMPORT(struct cell **sheet, char *argument);
char *check_and_add_csv(char *filename);
struct cell **parse_command(char *command, struct cell **sheet, char *filename);
void display_console(struct cell **sheet);

//exp_eval
int check_precedence(char c);
void push(struct stack *st, char data);
void push_int(struct stack *st, int data);
char pop(struct stack *st);
int pop_int(struct stack *st);
struct stack *create_stack(int capacity);
char *generate_postfix(char *relation);
int evaluate_postfix(char *postfix, struct cell **sheet, char *char_arr, int *num_arr, int len);


struct cell{
	int data;
	char formula[100];
	int flag;
};

struct stack{
	int top;
	int capacity;
	char *str;
	char **cell_adr;
};

