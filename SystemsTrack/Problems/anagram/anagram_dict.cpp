#include "links.h"


struct trie* create_trie(char ch)
{
	struct trie *temp = (struct trie*)malloc(sizeof(struct trie));
	temp->list = NULL;
	temp->anagrams = NULL;
	temp->anagram_index = 0;
	temp->data = ch;
	temp->index = 0;
	return temp;
}

void insertString(char *str, int len, struct trie *root)
{
	int i = 0;
	struct trie *traversal = root;
	int flag = 0;
	for (i = 0; i < len; i++){
		if (traversal->list != NULL){
			for (int j = 0; j < traversal->index; j++)
			{
				if (str[i] == traversal->list[j]->data)
				{
					traversal = traversal->list[j];
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				traversal->list = (struct trie**)realloc(traversal->list, sizeof(struct trie*)*(traversal->index + 1));
				struct trie *temp = create_trie(str[i]);
				traversal->list[traversal->index] = temp;
				traversal->index = traversal->index + 1;
				traversal = temp;
			}
			else{
				flag = 0;
			}
		}
		else
		{
			traversal->list = (struct trie**)realloc(traversal->list, sizeof(struct trie*)*(traversal->index + 1));
			struct trie *temp = create_trie(str[i]);
			traversal->list[traversal->index] = temp;
			traversal->index = traversal->index + 1;
			traversal = temp;
			//printf("added %s\n", str);
		}
	}
}

int check_string(struct trie *root, char *str, char *anagram){
	int i = 0;
	struct trie *traversal = root;
	int flag = 0;
	int len = strlen(str);
	if (traversal->data == '$' && traversal->list == NULL){
		return 1;
	}
	for (i = 0; i < len; i++){
		if (traversal->list != NULL){
			for (int j = 0; j < traversal->index; j++)
			{
				if (str[i] == traversal->list[j]->data)
				{
					traversal = traversal->list[j];
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				return 1;
			}
			else{
				flag = 0;
			}
		}
	}
	traversal->anagrams = (char **)realloc(traversal->anagrams, sizeof(char*)*(traversal->anagram_index + 1));
	traversal->anagrams[traversal->anagram_index] = (char*)malloc(sizeof(char) * 100);
	strcpy(traversal->anagrams[traversal->anagram_index], anagram);
	traversal->anagram_index = traversal->anagram_index + 1;
	return 0;
}

char *tolowerstring(char *str){
	int i = 0;
	while (str[i] != NULL){
		str[i] = tolower(str[i]);
		i++;
	}
	return str;
}

char *sortstring(char *str){
	int len = strlen(str);
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (str[i] > str[j]) {
				int temp = str[i];
				str[i] = str[j];
				str[j] = temp;
			}
		}
	}
	return str;
}

void print_anagrams(struct trie *root, char *str){
	int i = 0;
	struct trie *traversal = root;
	int flag = 0;
	int len = strlen(str);
	for (i = 0; i < len; i++){
		if (traversal->list != NULL){
			for (int j = 0; j < traversal->index; j++)
			{
				if (str[i] == traversal->list[j]->data)
				{
					traversal = traversal->list[j];
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				printf("No anagrams\n");
				return;
			}
			else{
				flag = 0;
			}
		}
	}
	for (int i = 0; i < traversal->anagram_index; i++){
		printf("%s\n", traversal->anagrams[i]);
	}
}

void get_anagrams(struct trie *root){
	char *input = (char*)malloc(sizeof(char) * 100);
	while (true){
		printf("enter the string\n");
		scanf("%s", input);
		input = tolowerstring(input);
		input = sortstring(input);
		print_anagrams(root, input);
	}
	free(input);
}

void generate_trie(){
	FILE *fptr;
	char *filename = (char *)malloc(sizeof(char) * 100);
	printf("enter filename : ");
	scanf("%s", filename);
	fptr = fopen(filename, "r");
	char *str = (char*)malloc(sizeof(char) * 100);
	struct trie *root = create_trie('$');
	while (fscanf(fptr, "%s", str) == 1){
		str = tolowerstring(str);
		char *anagram = (char *)malloc(sizeof(char) * 100);
		strcpy(anagram, str);
		str = sortstring(str);
		int res = check_string(root ,str, anagram);
		if (res == 1){
			insertString(str, strlen(str), root);
			res = check_string(root, str, anagram);
		}
	}
	get_anagrams(root);
	fclose(fptr);
	free(filename);
}

int main(){
	generate_trie();
	return 0;
}
