#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define START 2290000
#define HDD "harddisk.hdd"

struct entry{
	char filename[16];
	unsigned int start;
	unsigned int offset;
	struct entry *next;
};

void create_entry(char *filename, unsigned int start, unsigned int length, struct entry *root);
void delete_entry(char *filename, struct entry *root);
void list_files(struct entry *root);
void format(struct entry *root);
void copy_to_disk(char *filename, unsigned int start, unsigned int length, struct entry *root);
void copy_from_disk(char *filename, char *dest, struct entry *root);
void display_prompt(struct entry *root);
struct entry *load_disk();
char *remove_newline(char *str);

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

struct entry *load_disk(){
	struct entry *root = NULL;
	int size = 0;
	FILE *fp;
	if ((fp = fopen(HDD, "ab+")) == NULL){
		exit(0);
	}
	fseek(fp, 0, SEEK_SET);
	//checking if the hardisk is new
	struct entry *value = (struct entry*)malloc(sizeof(struct entry));
	int flag = -1;
	if (fread(&flag, sizeof(flag), 1, fp) < 0)exit(0);
	if (flag==0){
6		flag = 1;
		printf("%d\n", ftell(fp));
		fseek(fp, 0, SEEK_SET);
		if (fwrite(&flag, sizeof(int), 1, fp) < 1)exit(0);
		root = (struct entry*)malloc(sizeof(struct entry));
		//root->filename = (char*)malloc(sizeof(char) * 16);
		strcpy(root->filename, "NA");
		root->offset = 0;
		root->start = START;
		root->next = NULL;
		if (fwrite(root, 24, 1, fp)<0)exit(0);
		fclose(fp);
		return root;
	}
	fseek(fp, 0, SEEK_SET);
	//loading the table into memory
	
	struct entry *traversal = NULL;
	struct entry *pre = traversal;
	//fclose(fp);
	//fp = fopen(HDD, "rb");
	if (fread(&flag, sizeof(flag), 1, fp) < 0)exit(0);
	while (fread(value, 24, 1, fp)>0){
			if (pre == NULL){
				root = value;
				pre = value;
				value = value->next;
				continue;
			}
			pre->next = value;
			pre = value;
			value->next = NULL;
		//fseek(fp, ftell(fp), SEEK_SET);
	}
	//fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return root;
}

void list_files(struct entry *root){
	while (root != NULL){
		printf("%s\n", root->filename);
		root = root->next;
	}
}

void copy_from_disk(char *filename, char *dest, struct entry *root){
	FILE *fptr;
	if ((fptr = fopen(HDD, "ab+")) == NULL)exit(0);
	FILE *dst;
	if ((dst = fopen(dest, "ab+")) == NULL)exit(0);
	int start = 0, end = 0;
	while (root != NULL){
		if (strcmp(root->filename, filename) == 0){
			start = root->start;
			end = start + root->offset;
			break;
		}
		root = root->next;
	}
	char c;
	for (int i = start; i < end; i++){
		if (fread(&c, sizeof(char), 1, fptr) == NULL)exit(0);
		if (fwrite(&c, sizeof(char), 1, dst) == NULL)exit(0);
	}
	fclose(fptr);
	fclose(dst);
}

void copy_to_disk(char *filename, unsigned int start, unsigned int length, struct entry *root){
	FILE *fptr;
	if ((fptr = fopen(HDD, "ab+")) == NULL){
		exit(0);
	}
	fseek(fptr, 0, SEEK_SET);
	FILE *file;
	if ((file = fopen(filename, "ab+")) == NULL){
		exit(0);
	}
	fseek(file, 0, SEEK_SET);
	//entert into the ll
	struct entry *traversal = root;
	unsigned int st = 0;
	st = root->start;
	while (traversal->next != NULL){
		st = traversal->start;
		traversal = traversal->next;
	}
	if (traversal != NULL){
		st += traversal->offset;
	}
	struct entry *newnode = (struct entry *)malloc(sizeof(struct entry));
	//newnode->filename = (char*)malloc(sizeof(char) * 16);
	strcpy(newnode->filename, filename);
	newnode->start = st;
	newnode->offset = length;
	newnode->next = NULL;
	traversal->next = newnode;
	fwrite(newnode, 24, 1, fptr);
	fseek(fptr, st, SEEK_SET);
	fseek(file, 0, SEEK_SET);
	char c;
	for (int i = st; i < st + length; i++){
		if(fread(&c, sizeof(c), 1, file)<1)exit(0);
		if (fwrite(&c, sizeof(c), 1, fptr) < 1){
			exit(0);
		}
	}
	fclose(fptr);
	fclose(file);
}

int length_of_file(char *filename){
	FILE *file;
	if ((file = fopen(filename, "ab+")) == NULL){
		exit(0);
	}
	int count = 0;
	char c;
	while (c=fgetc(file)){
		if (c == EOF) break;
		count++;
	}
	return count;
}

void display_prompt(struct entry *root){
	char *command = (char *)malloc(sizeof(char) * 100);
	char *filename = (char *)malloc(sizeof(char) * 16);
	while ( 1 ){
		printf("\nEnter the command : ");
		fgets(command, 100, stdin);
		command = remove_newline(command);
		char *option = (char*)malloc(sizeof(char) * 50);
		sscanf(command, "%s", option);
		if (strcmp(option, "copytodisk") == 0){
			sscanf(command, "%s%s", option, filename);
			int len = length_of_file(filename);
			copy_to_disk(filename, 0, len , root);
		}
		else if (strcmp(option, "copyfromdisk") == 0){
			char *src = (char*)malloc(sizeof(char) * 16);
			char *dst = (char*)malloc(sizeof(char) * 16);
			sscanf(command, "%s %s %s", option, src, dst);
			copy_from_disk(src, dst, root);
		}
		else if (strcmp(option, "ls") == 0){
			list_files(root);
		}
		else if (strcmp(option, "exit") == 0){
			exit(0);
		}
	}
	free(filename);
	free(command);
}

int main(){
	struct entry *root = load_disk();
	display_prompt(root);
	return 0;
}