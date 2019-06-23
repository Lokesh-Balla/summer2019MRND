#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ENTRIES 32
#define BLOCK_SIZE 16
#define AVAILABLE_BLOCKS 63964
#define STARTING_BLOCK 4

struct entry{
	char filename[20];
	unsigned int start_block;
	unsigned int no_of_blocks;
	unsigned int len;
};

struct metadata{
	int header;
	struct entry records[ENTRIES];
	unsigned int no_of_files;
	char byte_vector[AVAILABLE_BLOCKS];
};

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

int length_of_file(char *filename){
	FILE *file;
	if ((file = fopen(filename, "rb")) == NULL)exit(0);
	fseek(file, 0, SEEK_END);
	return ftell(file);
}

void *get_block(char *filename, int block_number){
	FILE *fptr;
	void *buffer = malloc(BLOCK_SIZE * 1024);
	if ((fptr = fopen(filename, "rb")) == NULL)exit(0);
	rewind(fptr);
	fseek(fptr, (block_number-1) * BLOCK_SIZE * 1024, SEEK_SET);
	if ((fread(buffer, BLOCK_SIZE * 1024, 1, fptr)) < 1)
	{
		exit(0);
	}
	fclose(fptr);
	return buffer;
}

void set_block(char *filename, int block_number, void *buffer){
	FILE *fptr;
	if ((fptr = fopen(filename, "rb+")) == NULL)exit(0);
	rewind(fptr);
	fseek(fptr, (block_number-1) * BLOCK_SIZE * 1024, SEEK_SET);
	if ((fwrite(buffer, BLOCK_SIZE * 1024, 1, fptr)) < 1){ 
		exit(0); 
	}

	printf("%d", ftell(fptr));
	fclose(fptr);
}

void generate_table(struct entry *records, char *hdd){
	void *buffer = get_block(hdd, 0);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	memcpy(md, buffer, sizeof(struct metadata));
	if (md->header == 0x444E524D){
		for (int i = 0; i < ENTRIES; i++){
			records[i] = md->records[i];
		} 
		return;
	}
	else{
		md->header = 0x444E524D;
		md->no_of_files = 0;
		for (int i = 0; i < ENTRIES; i++){
			strcpy(md->records[i].filename, "");
			md->records[i].len = 0;
			md->records[i].no_of_blocks = 0;
			md->records[i].start_block = 0;
		}
		for (int i = 0; i < AVAILABLE_BLOCKS; i++){
			md->byte_vector[i] = '0';
		}
	}
	void *buffer1 = malloc(BLOCK_SIZE * 1024);
	memcpy(buffer1, md, BLOCK_SIZE * 1024);
	set_block(hdd, 0, buffer1);
}

void copy_to_disk(char *filename, char *hdd, char *destname){
	int len = length_of_file(filename) / 1024;
	int blocks = len / BLOCK_SIZE;
	void *buffer = get_block(hdd, 0);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	memcpy(md, buffer, sizeof(struct metadata));
	int count = 0, flag = 0, st_block = 0;
	for (int i = 0; i <= AVAILABLE_BLOCKS; i++){
		if (md->byte_vector[i] == '0' && flag == 0 && count == len){
			break;
		}
		if (md->byte_vector[i] == '0' && flag == 0){
			st_block = i;
			flag = 1;
			count++;
		}else if (md->byte_vector[i] == '0' && flag == 1 && count==len){
			break;
		}
		else if (md->byte_vector[i] == '0' &&flag == 1){
			count++;
		}
		else if(md->byte_vector[i] == '1' && flag == 1){
			count = 0;
			flag = 0;
		}
	}
	for (int i = st_block; i <= st_block + count; i++){
		md->byte_vector[i] = '1';
	}
	md->no_of_files += 1;
	md->header = 0x444E524D;
	strcpy(md->records[md->no_of_files].filename, destname);
	md->records[md->no_of_files].len = len;
	md->records[md->no_of_files].no_of_blocks = count;
	md->records[md->no_of_files].start_block = st_block + STARTING_BLOCK;
	void *buffer1 = malloc(BLOCK_SIZE * 1024);
	memcpy(buffer1, md, BLOCK_SIZE * 1024);
	set_block(hdd, 0, buffer1);
	FILE *fptr;
	if ((fptr = fopen(filename, "rb")) == NULL)exit(0);
	buffer = malloc(BLOCK_SIZE * 1024);
	for (int i = st_block+STARTING_BLOCK; i <= st_block + STARTING_BLOCK + count; i++){
		fread(buffer, BLOCK_SIZE*1024, 1, fptr);
		//buffer = get_block(filename, i-(st_block+STARTING_BLOCK));
		memcpy(buffer1, buffer, BLOCK_SIZE * 1024);
		set_block(hdd, i, buffer1);
	}
}

void copy_from_disk(char *filename, char *hdd, char *destname){
	void *buffer = get_block(hdd, 0);
	FILE *fptr;
	if ((fptr = fopen(destname, "wb")) == NULL)exit(0);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	int leftover = 0;
	void *buffer1 = malloc(BLOCK_SIZE * 1024);
	memcpy(md, buffer, sizeof(struct metadata));
	int pos = 0;
	for (int i = 0; i < ENTRIES; i++){
		if (strcmp(md->records[i].filename, filename) == 0){
			pos = i;
			leftover = md->records[i].len%BLOCK_SIZE;
			for (int j = md->records[i].start_block; j <= md->records[i].start_block + md->records[i].no_of_blocks; i++){
				buffer = get_block(hdd, j + 1);
				if (j != md->records[i].start_block + md->records[i].no_of_blocks){
					fwrite(buffer, sizeof(buffer), 1, fptr);
				}
				else{
					fwrite(buffer, leftover, 1, fptr);
				}
				
			}
			break;
		}
	}
	fclose(fptr);
}

void delete_from_disk(char *filename, char *hdd){
	void *buffer = get_block(hdd, 0);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	memcpy(md, buffer, sizeof(struct metadata));
	int pos = 0;
	int replace = md->no_of_files;
	for (int i = 0; i < md->no_of_files; i++){
		if (strcmp(md->records[i].filename, filename) == 0){
			for (int j = md->records[i].start_block; j < md->records[i].start_block + md->records[i].no_of_blocks; j++){
				md->byte_vector[j] = '0';
			}
			md->records[i] = md->records[replace];
			md->no_of_files -= 1;
			break;
		}
	}
	void *buffer1 = malloc(BLOCK_SIZE * 1024);
	memcpy(buffer1, md, BLOCK_SIZE * 1024);
	set_block(hdd, 0, buffer1);
}

void debug(char *hdd){
	void *buffer = get_block(hdd, 0);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	memcpy(md, buffer, sizeof(struct metadata));
	for (int i = 1; i <= md->no_of_files; i++){
		printf("%s  %d  %d  %d  %d\n", md->records[i].filename, md->records[i].no_of_blocks, md->records[i].start_block, md->records[i].len);
	}
}

/*
struct entry *generate_table(struct entry *records, char *hdd){
	FILE *fptr;
	int mrnd;
	int no_of_files = 0, free_blocks = AVAILABLE_BLOCKS;
	if ((fptr = fopen(hdd, "rb+")) == NULL)exit(0);
	if (fread(&mrnd, sizeof(mrnd), 1, fptr) < 1)exit(0);
	if (mrnd == 0x444E524D){
		fread(&no_of_files, sizeof(no_of_files), 1, fptr);
		for (int i = 0; i < ENTRIES; i++){
			fread(&records[i], sizeof(struct entry), 1, fptr);
		}
	}
	else{
		rewind(fptr);
		mrnd = 0x444E524D;
		fwrite(&mrnd, sizeof(mrnd), 1, fptr);
		//no of files
		fwrite(&no_of_files, sizeof(no_of_files), 1, fptr);
		//table
		for (int i = 0; i < ENTRIES; i++){
			strcpy(records[i].filename, "NULL");
			records[i].len = 0;
			records[i].no_of_blocks = 0;
			records[i].start_block = 0;
			fwrite(&records[i], sizeof(struct entry), 1, fptr);
		}
		//byte array
		rewind(fptr);
		fseek(fptr, 2 * BLOCK_SIZE * 1024, SEEK_CUR);
		char *arr = (char*)malloc(sizeof(char) * AVAILABLE_BLOCKS);
		for (int i = 0; i < AVAILABLE_BLOCKS; i++){
			arr[i] = '0';
		}
		fwrite(arr, sizeof(char), AVAILABLE_BLOCKS, fptr);
		free(arr);
		arr = NULL;
		//no fo free blocks
		rewind(fptr);
		fseek(fptr, 3 * BLOCK_SIZE * 1024, SEEK_CUR);
		fwrite(&free_blocks, sizeof(free_blocks), 1, fptr);
	}
	fclose(fptr);
	fptr = NULL;
	return records;
}


void copy_to_blocks(char *filename, char *hdd, int start_block, int len){
	FILE *fptr, *fp;
	if ((fptr = fopen(hdd, "rb+")) == NULL)exit(0);
	if ((fp = fopen(filename, "rb")) == NULL)exit(0);
	void *buffer = malloc(BLOCK_SIZE * 1024);
	rewind(fp);
	rewind(fptr);
	fseek(fptr, start_block * BLOCK_SIZE * 1024, SEEK_CUR);
	for (int i = 0; i < len; i++){
		fread(buffer, BLOCK_SIZE * 1024, 1, fp);
		fwrite(buffer, BLOCK_SIZE * 1024, 1, fptr);
	}
}



void copy_to_disk(char *filename, char *hdd, char *destname, struct entry *records){
	FILE *fptr, *fp;
	int len = length_of_file(filename), start_block = 5;
	int mrnd, no_of_blocks = len / (BLOCK_SIZE * 1024);
	if ((fptr = fopen(hdd, "rb+")) == NULL)exit(0);
	if ((fp = fopen(filename, "rb")) == NULL)exit(0);
	rewind(fptr);
	struct entry tvalue;
	struct entry present, previous;
	int no_of_files = 0, free_blocks = 0;
	char ch;
	if ((fptr = fopen(hdd, "rb+")) == NULL)exit(0);
	if (fread(&mrnd, sizeof(mrnd), 1, fptr) < 1)exit(0);
	if (mrnd == 0x444E524D){
		if(fread(&no_of_files, sizeof(no_of_files), 1, fptr) < 1)exit(0);
		fseek(fptr, -1, SEEK_CUR);
		no_of_files++;
		if (fwrite(&no_of_files, sizeof(no_of_files), 1, fptr) < 1)exit(0);
		fseek(fptr, no_of_files - 1, SEEK_CUR);
		fread(&previous, sizeof(struct entry), 1, fptr);
		strcpy(present.filename, destname);
		present.no_of_blocks = no_of_blocks;
		present.start_block = previous.start_block + previous.no_of_blocks + 1;
		present.len = len / 1024;
		fwrite(&present, sizeof(struct entry), 1, fptr);
		rewind(fptr);
		fseek(fptr, (2 * BLOCK_SIZE * 1024)+present.start_block, SEEK_CUR);
		ch = '1';
		for (int i = 0; i < present.no_of_blocks; i++){
			fwrite(&ch, sizeof(char), 1, fptr);
		}
		rewind(fptr);
		fseek(fptr, 3 * BLOCK_SIZE * 1024, SEEK_CUR);
		fread(&free_blocks, sizeof(int), 1, fptr);
		fseek(fptr, -4, SEEK_CUR);
		free_blocks += 1;
		fwrite(&free_blocks, sizeof(int), 1, fptr);
		fclose(fptr);
		fclose(fp);
		copy_to_blocks(filename, hdd, present.start_block, present.len);
	}
}

void list_files(struct entry *records){
	for (int i = 0; i < ENTRIES; i++){
		if (strcmp(records[i].filename, "NULL") != 0){
			printf("%d", records[i].filename);
		}
		else{
			break;
		}
	}
}
*/
void display_prompt(){
	char *command = (char *)malloc(sizeof(char) * 100);
	char *filename = (char *)malloc(sizeof(char) * 100);
	char *dest = (char *)malloc(sizeof(char) * 100);
	char *hdd = (char *)malloc(sizeof(char) * 100);
	struct entry *records = (struct entry*)malloc(sizeof(struct entry) * 32);
	printf("Enter the HDD name : ");
	fgets(hdd, 100, stdin);
	hdd = remove_newline(hdd);
	generate_table(records, hdd);
	while (true){
		printf("Enter the command : ");
		fgets(command, 100, stdin);
		command = remove_newline(command);
		char *option = (char*)malloc(sizeof(char) * 50);
		sscanf(command, "%s", option);
		if (strcmp(option, "copytodisk") == 0){
			sscanf(command, "%s %s %s", option, filename, dest);
			copy_to_disk(filename, hdd, dest);
		}
		else if (strcmp(option, "copyfromdisk") == 0){
			char *src = (char*)malloc(sizeof(char) * 20);
			char *dst = (char*)malloc(sizeof(char) * 20);
			sscanf(command, "%s %s %s", option, src, dst);
			copy_from_disk(src, hdd, dst);
			free(src);
			free(dst);
		}
		else if (strcmp(option, "delete") == 0){
			char *src = (char*)malloc(sizeof(char) * 20);
			sscanf(command, "%s %s %s", option, src);
			delete_from_disk(src, hdd);
			free(src);
		}
		else if (strcmp(option, "debug") == 0){
			debug(hdd);
		}
		else if (strcmp(option, "exit") == 0){
			exit(0);
		}
	}
	free(filename);
	free(command);
	free(dest);
	free(hdd);
}



int main(){
	display_prompt();
	return 0;
}