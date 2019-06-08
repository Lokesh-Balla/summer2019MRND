
#include "links.h"

int tokenizer(char *line,char *command,char **tokens){
	sscanf(line, "%s", command);
	//Split the tokens when ever there is a , and remove leading and ending spaces
	int temp_index = 0, tokens_index = 0;
	char *temp = (char *)malloc(sizeof(temp) * 10);
	int spaces = no_of_spaces(line);
	int i = spaces + strlen(command);
	while (line[i] != NULL){
		if (line[i] == ','){
			temp[temp_index] = '\0';
			temp_index = 0;
			trim(temp);
			tokens[tokens_index] = (char*)malloc(sizeof(char) * 10);
			strcpy(tokens[tokens_index], temp);
			tokens_index++;
			i++;
			continue;
		}
		temp[temp_index++] = line[i];
		i++;
	}
	temp[temp_index] = '\0';
	trim(temp);
	tokens[tokens_index] = (char*)malloc(sizeof(char) * 10);
	strcpy(tokens[tokens_index], temp);
	return tokens_index + 1;
}




int tokenizer_for_if(char *line, char *command, char **tokens){
	sscanf(line, "%s", command);
	//Split the tokens when ever there is a , and remove leading and ending spaces
	int temp_index = 0, tokens_index = 0;
	char *temp = (char *)malloc(sizeof(temp) * 10);
	int i = strlen(command);
	while (line[i] != NULL){
		if (line[i] == ' '){
			temp[temp_index] = '\0';
			temp_index = 0;
			tokens[tokens_index] = (char*)malloc(sizeof(char) * 10);
			strcpy(tokens[tokens_index], temp);
			tokens_index++;
			i++;
			continue;
		}
		temp[temp_index++] = line[i];
		i++;
	}
	temp[temp_index] = '\0';
	trim(temp);
	tokens[tokens_index] = (char*)malloc(sizeof(char) * 10);
	strcpy(tokens[tokens_index], temp);
	return tokens_index + 1;
}



void build_intermediate_table(char *command, char **tokens, int token_len, struct inter_lang **inter_entries, unsigned int inter_ind, struct symbol_table **sy_table, unsigned int sy_ind){
	
	//char *options[] = { "MOV", "MOV", "ADD", "SUB", "MUL", "JMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ" };

	inter_entries[inter_ind] = (struct inter_lang*)malloc(sizeof(struct inter_lang));

	if (strcmp(command, "READ") == 0){
		inter_entries[inter_ind]->in_num = inter_ind;
		inter_entries[inter_ind]->op_code = 14;
		inter_entries[inter_ind]->parameter_len = 1;
		if (check_register(tokens[0]) == 1){
			inter_entries[inter_ind]->parameters[0] = register_number(tokens[0]);
		}
		else{
			for (int i = 0; i < sy_ind; i++){
				if (strcmp(tokens[0], sy_table[i]->name) == 0){
					inter_entries[inter_ind]->parameters[0] = sy_table[i]->address;
					break;
				}
			}
		}
	}

	else if (strcmp(command, "MOV") == 0){
		if (check_register(tokens[1]) == 1){
			inter_entries[inter_ind]->in_num = inter_ind;
			inter_entries[inter_ind]->op_code = 1;
			inter_entries[inter_ind]->parameter_len = 2;
			for (int i = 0; i < sy_ind; i++){
				if (strcmp(tokens[0], sy_table[i]->name) == 0){
					inter_entries[inter_ind]->parameters[0] = sy_table[i]->address;
					break;
				}
			}
			inter_entries[inter_ind]->parameters[1] = register_number(tokens[1]);
		}
		else{
			inter_entries[inter_ind]->in_num = inter_ind;
			inter_entries[inter_ind]->op_code = 2;
			inter_entries[inter_ind]->parameter_len = 2;
			for (int i = 0; i < sy_ind; i++){
				if (strcmp(tokens[1], sy_table[i]->name) == 0){
					inter_entries[inter_ind]->parameters[1] = sy_table[i]->address;
					break;
				}
			}
			inter_entries[inter_ind]->parameters[0] = register_number(tokens[0]);
		}
	}

	else if (strcmp(command, "ADD") == 0){
		inter_entries[inter_ind]->in_num = inter_ind;
		inter_entries[inter_ind]->op_code = 3;
		inter_entries[inter_ind]->parameter_len = 3;
		inter_entries[inter_ind]->parameters[0] = register_number(tokens[0]);
		inter_entries[inter_ind]->parameters[1] = register_number(tokens[1]);
		inter_entries[inter_ind]->parameters[2] = register_number(tokens[2]);
	}


	else if (strcmp(command, "PRINT") == 0){
		inter_entries[inter_ind]->in_num = inter_ind;
		inter_entries[inter_ind]->op_code = 14;
		inter_entries[inter_ind]->parameter_len = 1;
		for (int i = 0; i < sy_ind; i++){
			if (strcmp(tokens[0], sy_table[i]->name) == 0){
				inter_entries[inter_ind]->parameters[0] = sy_table[i]->address;
				break;
			}
		}
	}

}






void build_symbol_table(char *command, char **tokens, int tokens_len, struct symbol_table **entries, unsigned int entries_ind, unsigned int entries_start){
	entries[entries_start] = (struct symbol_table*)malloc(sizeof(struct symbol_table));
	if (strcmp(command, "CONST") == 0){
		char str[] = { tokens[0][0], '\0' };
		strcpy(entries[entries_start]->name, str);
		entries[entries_start]->address = entries_ind;
		entries[entries_start]->size = 0;
		return;
	}
	if (tokens[0][1] != '['){
		strcpy(entries[entries_start]->name, tokens[0]);
		entries[entries_start]->address = entries_ind;
		entries[entries_start]->size = 1;
	}
	else{
		int num = 0;
		int i = 0, flag = 0;
		while (tokens[0][i] != ']'){
			if (tokens[0][i] == '[' && flag == 0){
				flag = 1;
				i++;
				continue;
			}
			if (flag == 1){
				num = num * 10 + (tokens[0][i] - '0');
			}
			i++;
		}
		char ch[2];
		ch[0] = tokens[0][0];
		ch[1] = '\0';
		strcpy(entries[entries_start]->name, ch);
		entries[entries_start]->address = entries_ind;
		entries[entries_start]->size = num;
	}
}





void if_block(struct inter_lang **inter_lang, unsigned int inter_lang_ind, struct stack *stack, char **tokens){
	inter_lang[inter_lang_ind] = (struct inter_lang*)malloc(sizeof(struct inter_lang));
	inter_lang[inter_lang_ind]->in_num = inter_lang_ind;
	inter_lang[inter_lang_ind]->op_code = 7;
	inter_lang[inter_lang_ind]->parameter_len = 4;
	inter_lang[inter_lang_ind]->parameters[0] = register_number(tokens[1]);
	inter_lang[inter_lang_ind]->parameters[1] = register_number(tokens[3]);
	if (strcmp(tokens[2], "EQ") == 0){
		inter_lang[inter_lang_ind]->parameters[2] = 8;
	}
	else if (strcmp(tokens[2], "LT") == 0){
		inter_lang[inter_lang_ind]->parameters[2] = 9;
	}
	else if (strcmp(tokens[2], "GT") == 0){
		inter_lang[inter_lang_ind]->parameters[2] = 10;
	}
	else if (strcmp(tokens[2], "LTEQ") == 0){
		inter_lang[inter_lang_ind]->parameters[2] = 11;
	}
	else if (strcmp(tokens[2], "GTEQ") == 0){
		inter_lang[inter_lang_ind]->parameters[2] = 12;
	}
	push(stack, inter_lang_ind);
}


void else_or_jump(struct inter_lang **inter_lang, unsigned int inter_lang_ind, struct stack *stack){
	inter_lang[inter_lang_ind] = (struct inter_lang*)malloc(sizeof(struct inter_lang));
	inter_lang[inter_lang_ind]->in_num = inter_lang_ind;
	inter_lang[inter_lang_ind]->op_code = 6;
	inter_lang[inter_lang_ind]->parameter_len = 1;
	push(stack, inter_lang_ind);
}




void endif(struct inter_lang **inter_lang, unsigned int inter_lang_ind, struct stack *stack){
	unsigned int value = inter_lang_ind - 1;
	while (inter_lang[value]->op_code != 7){
		int index = pop(stack);
		inter_lang[index]->parameters[inter_lang[index]->parameter_len - 1] = value;
		value = index;
	}
}




void print_tables(struct symbol_table **sy_table, unsigned int sy_len, struct inter_lang **in_table, unsigned int in_len){
	for (int i = 0; i < sy_len; i++){
		printf("%s %d %d\n", sy_table[i]->name, sy_table[i]->address, sy_table[i]->size);
	}

	for (int i = 0; i < in_len; i++){
		printf("%d %d %d", in_table[i]->in_num, in_table[i]->op_code, in_table[i]->parameter_len);
		for (int j = 0; j < in_table[i]->parameter_len; j++){
			printf(" %d", in_table[i]->parameters[j]);
		}
		printf("\n");
	}

}



void write_to_disk(struct symbol_table **sy_table, unsigned int sy_len, struct inter_lang **in_table, unsigned int in_len, struct label_table **label_table, unsigned int label_ind){
	FILE *fptr;
	if ((fptr = fopen(OUTPUT_FILE_NAME, "w")) == NULL)return;
	fprintf(fptr, "SYSTABLE\n");
	for (int i = 0; i < sy_len; i++){
		fprintf(fptr, "%s,%d,%d\n", sy_table[i]->name, sy_table[i]->address, sy_table[i]->size);
	}
	fprintf(fptr, "END_SYSTABLE\n");
	fprintf(fptr, "ICG_TABLE\n");
	for (int i = 0; i < in_len; i++){
		fprintf(fptr, "%d,%d,%d", in_table[i]->in_num, in_table[i]->op_code, in_table[i]->parameter_len);
		for (int j = 0; j < in_table[i]->parameter_len; j++){
			fprintf(fptr,",%d", in_table[i]->parameters[j]);
		}
		fprintf(fptr,"\n");
	}
	fprintf(fptr, "END_IGC_TABLE\n");
	fprintf(fptr, "LABEL_TABLE\n");
	for (int i = 0; i < label_ind; i++){
		fprintf(fptr, "%s,%d\n", label_table[i]->block_name, label_table[i]->address);
	}
	fprintf(fptr, "END_LABEL_TABLE\n");
	fclose(fptr);
}



void readfile(){
	char *filename = (char*)malloc(sizeof(char) * 100);
	printf("enter the filename\n");
	scanf("%s", filename);
	char *line = (char*)malloc(sizeof(char) * 100);
	char *command = (char*)malloc(sizeof(char) * 25);
	char **tokens = (char**)malloc(sizeof(char*) * 100);
	struct symbol_table **entries = (struct symbol_table**)malloc(sizeof(struct symbol_table*)*ENTRIES_LIMIT);
	struct inter_lang **inter_entries = (struct inter_lang**)malloc(sizeof(struct inter_lang*)*ENTRIES_LIMIT);
	struct label_table **label_table = (struct label_table**)malloc(sizeof(struct label_table*)*ENTRIES_LIMIT);
	struct stack *stack = create_stack(ENTRIES_LIMIT);
	FILE *file;

	//for symbol table
	unsigned int entries_len = 0, entries_start = 0;

	//for intermidiate language table
	unsigned int inter_ind = 0, *inter_len = 0;

	//for label table
	unsigned int label_ind = 0;


	if ((file = fopen(filename, "r")) == NULL)return;
	fseek(file, 0, SEEK_SET);
	while (fgets(line, 100, file) != NULL){
		
		line = remove_newline(line);
		
		int no_of_tokens = tokenizer(line, command, tokens);


		if (strcmp(command,"DATA") == 0 || strcmp(command,"CONST") == 0){
			build_symbol_table(command, tokens, no_of_tokens, entries, entries_len, entries_start);
			entries_len += entries[entries_start]->size;
			entries_start++;
			continue;
		}

		
		else if (check_label(command) == 1){
			label_table[label_ind] = (struct label_table*)malloc(sizeof(struct label_table));
			strcpy(label_table[label_ind]->block_name, get_block_name(command));
			label_table[label_ind]->address = entries_start;
		}
		
		else if (strcmp(command, "JUMP") == 0){
			inter_entries[inter_ind]->in_num = inter_ind;
			inter_entries[inter_ind]->op_code = 6;
			inter_entries[inter_ind]->parameter_len = 1;
			inter_entries[inter_ind]->parameters[0] = check_in_label_table(tokens[0], label_table, label_ind);
			inter_ind++;
		}

		else if (strcmp(command, "IF") == 0){
			char **tokens1 = (char**)malloc(sizeof(char*) * 100);
			no_of_tokens = tokenizer_for_if(line, command, tokens1);
			if_block(inter_entries, inter_ind, stack, tokens1);
			inter_ind += 1;
		}

		else if (strcmp(command, "ELSE") == 0){
			else_or_jump(inter_entries, inter_ind, stack);
			label_table[label_ind] = (struct label_table *)malloc(sizeof(struct label_table));
			label_table[label_ind]->address = inter_ind;
			strcpy(label_table[label_ind]->block_name, "ELSE");
			inter_ind += 1;
			label_ind++;
		}

		else if (strcmp(command, "ENDIF") == 0){
			endif(inter_entries, inter_ind, stack);
		}


		else if (strcmp(command, "END") == 0){
			write_to_disk(entries, entries_start, inter_entries, inter_ind, label_table, label_ind);
		}

		else if (no_of_tokens > 0){
			build_intermediate_table(command, tokens, no_of_tokens, inter_entries, inter_ind, entries, entries_start);
			inter_ind += 1;
			//label_ind++;
			continue;
		}

	}
	print_tables(entries, entries_start, inter_entries, inter_ind);
	fclose(file);
	file = NULL;
	free(line);
	free(filename);
	free(command);
	free(tokens);
}





int main(){
	readfile();
	return 0;
}