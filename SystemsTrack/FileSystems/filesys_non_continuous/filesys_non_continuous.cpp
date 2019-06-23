#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ENTRIES 32
#define BLOCK_SIZE 16384
#define AVAILABLE_BLOCKS 6400
#define STARTING_BLOCK 2

struct entry{
	char filename[20];
	unsigned int size;
	unsigned int no_of_blocks;
	unsigned int block;
};

struct metadata{
	int magic_number;
	unsigned int block_size = 0;
	unsigned int no_of_blocks;
	unsigned int no_of_empty_blocks;
	char byte_vector[AVAILABLE_BLOCKS];
};

struct filerecord{
	unsigned int no_of_files;
	struct entry records[ENTRIES];
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

char *get_block(char *filename, int block_number){
	FILE *fptr;
	char *buffer = (char*)malloc(BLOCK_SIZE);
	if ((fptr = fopen(filename, "rb")) == NULL)exit(0);
	int num = block_number * BLOCK_SIZE;
	fseek(fptr, num, SEEK_SET);
	if ((fread(buffer, BLOCK_SIZE, 1, fptr)) < 1)
	{
		exit(0);
	}
	fclose(fptr);
	return buffer;
}

void set_block(char *filename, int block_number, char *buffer){
	FILE *fptr;
	if ((fptr = fopen(filename, "rb+")) == NULL)exit(0);
	fseek(fptr, block_number * BLOCK_SIZE, SEEK_SET);
	if ((fwrite(buffer, BLOCK_SIZE, 1, fptr)) < 1){
		exit(0);
	}
	fclose(fptr);
}

void copy_to_disk(char *filename, char *hdd, char *dest){
	char *buffer = (char*)malloc(BLOCK_SIZE);
	char *buffer1 = (char*)malloc(BLOCK_SIZE);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	struct filerecord *fr = (struct filerecord*)malloc(sizeof(struct filerecord));
	buffer = get_block(hdd, 0);
	memcpy(md, buffer, sizeof(struct metadata));
	memcpy(fr, buffer + sizeof(struct metadata), sizeof(struct filerecord));
	
	int len_of_file = length_of_file(filename);
	int no_of_blocks = len_of_file / BLOCK_SIZE;
	int trailing_space = len_of_file - (BLOCK_SIZE * no_of_blocks);
	int i;

	int count = 0, flag = 0, st_block = 0;
	int *arr = (int *)malloc(sizeof(int) * 10000);
	for (i = 1; i < AVAILABLE_BLOCKS; i++){
		if (md->byte_vector[i] == '0' && count <= no_of_blocks){
			arr[count++] = i;
			md->byte_vector[i] = '1';
		}
	}
	if (trailing_space != 0){
		arr[count++] = i + 1;
		md->byte_vector[i + 1] = '1';
	}
	fr->no_of_files += 1;
	strcpy(fr->records[fr->no_of_files].filename, dest);
	fr->records[fr->no_of_files].size = len_of_file;
	fr->records[fr->no_of_files].no_of_blocks = count - 1;
	FILE *fptr;
	if ((fptr = fopen(filename, "rb")) == NULL)exit(0);
	i = 1;
	if (no_of_blocks > 1){
		fr->records[fr->no_of_files].block = arr[0];
		memcpy(buffer, &arr[1], count - 1);
		memcpy(buffer1, buffer, BLOCK_SIZE);
		set_block(hdd, arr[0], buffer1);
		while (i < count){
			fread(buffer, BLOCK_SIZE, 1, fptr);
			memcpy(buffer1, buffer, BLOCK_SIZE);
			set_block(hdd, arr[i], buffer1);
			i++;
		}
		if (trailing_space != 0){
			fread(buffer, trailing_space, 1, fptr);
			memcpy(buffer1, buffer, BLOCK_SIZE);
			set_block(hdd, arr[i], buffer1);
		}
	}
	else{
		fr->records[fr->no_of_files].block = arr[0];
		if (trailing_space != 0){
			fread(buffer, trailing_space, 1, fptr);
			memcpy(buffer1, buffer, BLOCK_SIZE);
			set_block(hdd, arr[i], buffer1);
		}
	}
	memcpy(buffer1, md, sizeof(struct metadata));
	memcpy((char*)buffer1 + sizeof(struct metadata), fr, sizeof(struct filerecord));
	memcpy(buffer, buffer1, BLOCK_SIZE);
	set_block(hdd, 0, buffer);
	fclose(fptr);
	free(buffer);
	free(buffer1);
	free(arr);
}

void copy_from_disk(char *filename, char *hdd, char *dest){
	char *buffer = (char*)malloc(BLOCK_SIZE);
	char *buffer1 = (char*)malloc(BLOCK_SIZE);
	struct metadata *md = (struct metadata*)malloc(sizeof(struct metadata));
	struct filerecord *fr = (struct filerecord*)malloc(sizeof(struct filerecord));
	buffer = get_block(hdd, 0);
	memcpy(md, buffer, sizeof(struct metadata));
	memcpy(fr, (char*)buffer + sizeof(struct metadata), sizeof(struct filerecord));

	int len_of_file, trailing_space, no_of_blocks;

	int arr[10000] = { 0 };
	for (int i = 1; i <= fr->no_of_files; i++){
		if (strcmp(fr->records[i].filename, filename) == 0){
			len_of_file = fr->records[i].size;
			no_of_blocks = len_of_file / BLOCK_SIZE;
			trailing_space = len_of_file - (BLOCK_SIZE * no_of_blocks);
			if (fr->records[i].no_of_blocks > 1){
				buffer = get_block(hdd, fr->records[i].block);
				memcpy(&arr[0], buffer, fr->records[i].no_of_blocks);
				FILE *fptr;
				if ((fptr = fopen(dest, "wb")) == NULL)exit(0);
				int j;
				for (j = 0; j < fr->records[i].no_of_blocks - 1; j++){
					buffer = get_block(hdd, arr[j]);
					memcpy(buffer1, buffer, BLOCK_SIZE);
					
					if ((fwrite(buffer1, BLOCK_SIZE, 1, fptr)) < 1)exit(0);
				}
				if (trailing_space!=0){
					buffer = get_block(hdd, arr[j]);
					if ((fwrite(buffer1, trailing_space, 1, fptr)) < 1)exit(0);
				}
				fclose(fptr);
				return;
			}
			else{
				buffer = get_block(hdd, fr->records[i].block);
				memcpy(buffer1, buffer, fr->records[i].size);
				FILE *fptr;
				if ((fptr = fopen(dest, "wb")) == NULL)exit(0);
				if ((fwrite(buffer1, fr->records[i].size, 1, fptr)) < 1)exit(0);
				fclose(fptr);
				return;
			}
		}
	}
}

void init(char *filename){
	int len = length_of_file(filename);
	char *buffer = (char*)malloc(BLOCK_SIZE);
	char *buffer1 = (char*)malloc(BLOCK_SIZE);
	buffer = get_block(filename, 0);
	struct metadata *md = (struct metadata *)malloc(sizeof(struct metadata));
	memcpy(md, buffer, sizeof(struct metadata));
	if (md->magic_number != 0x444E524D){
		printf("do you wish to format [Y/N]\n");
		char ch;
		scanf("%c", &ch);
		if (ch == 'N' || ch == 'n'){
			return;
		}
		md->magic_number = 0x444E524D;
		md->block_size = BLOCK_SIZE;
		md->no_of_blocks = len / BLOCK_SIZE;
		md->no_of_empty_blocks = 0;
		md->byte_vector[0] = '1';
		for (int i = 1; i < AVAILABLE_BLOCKS; i++){
			md->byte_vector[i] = '0';
		}
		struct filerecord *fr = (struct filerecord *)malloc(sizeof(struct filerecord));
		memcpy(fr, (char*)buffer + sizeof(struct metadata), sizeof(struct filerecord));
		fr->no_of_files = 0;
		for (int i = 0; i < ENTRIES; i++){
			strcpy(fr->records[i].filename, "NOFILE");
			fr->records[i].block = 0;
			fr->records[i].no_of_blocks = 0;
			fr->records[i].size = 0;
		}

		memcpy(buffer1, md, sizeof(struct metadata));
		memcpy((char*)buffer1 + sizeof(struct metadata), fr, sizeof(struct filerecord));
		set_block(filename, 0, buffer1);
	}
	free(buffer);
	free(buffer1);
}

void format(char *hdd){
	int len = length_of_file(hdd);
	char *buffer = (char*)malloc(BLOCK_SIZE);
	char *buffer1 = (char*)malloc(BLOCK_SIZE);
	buffer = get_block(hdd, 0);
	struct metadata *md = (struct metadata *)malloc(sizeof(struct metadata));
	memcpy(md, buffer, sizeof(struct metadata));
	md->magic_number = 0x444E524D;
	md->block_size = BLOCK_SIZE;
	md->no_of_blocks = len / BLOCK_SIZE;
	md->no_of_empty_blocks = 0;
	md->byte_vector[0] = '1';
	for (int i = 1; i < AVAILABLE_BLOCKS; i++){
		md->byte_vector[i] = '0';
	}
	struct filerecord *fr = (struct filerecord *)malloc(sizeof(struct filerecord));
	memcpy(fr, (char*)buffer + sizeof(struct metadata), sizeof(struct filerecord));
	fr->no_of_files = 0;
	for (int i = 0; i < ENTRIES; i++){
		strcpy(fr->records[i].filename, "NOFILE");
		fr->records[i].block = 0;
		fr->records[i].no_of_blocks = 0;
		fr->records[i].size = 0;
	}

	memcpy(buffer1, md, sizeof(struct metadata));
	memcpy((char*)buffer1 + sizeof(struct metadata), fr, sizeof(struct filerecord));
	set_block(hdd, 0, buffer1);
	free(buffer);
	free(buffer1);
}

void debug(char *hdd){
	char *buffer = (char*)malloc(BLOCK_SIZE);
	buffer = get_block(hdd, 0);
	struct filerecord *fr = (struct filerecord *)malloc(sizeof(struct filerecord));
	memcpy(fr, (char*)buffer + sizeof(struct metadata), sizeof(struct filerecord));
	for (int i = 1; i <= fr->no_of_files; i++){
		printf("%s	%d	%d	%d\n", fr->records[i].filename,fr->records[i].block,fr->records[i].no_of_blocks,fr->records[i].size);
	}
}

void display_prompt(){
	char *command = (char *)malloc(sizeof(char) * 100);
	char *filename = (char *)malloc(sizeof(char) * 100);
	char *dest = (char *)malloc(sizeof(char) * 100);
	char *hdd = (char *)malloc(sizeof(char) * 100);
	struct entry *records = (struct entry*)malloc(sizeof(struct entry) * 32);
	printf("Enter the HDD name : ");
	fgets(hdd, 100, stdin);
	hdd = remove_newline(hdd);
	//generate_table(records, hdd);
	init(hdd);
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
			//delete_from_disk(src, hdd);
			free(src);
		}
		else if (strcmp(option, "debug") == 0){
			debug(hdd);
		}
		else if (strcmp(option, "format") == 0){
			format(hdd);
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