#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void generate_trie();
void get_anagrams(struct trie *root);
void print_anagrams(struct trie *root, char *str);
char *sortstring(char *str);
char *tolowerstring(char *str);
int check_string(struct trie *root, char *str, char *anagram);
void insertString(char *str, int len, struct trie *root);
struct trie* create_trie(char ch);

struct trie{
	char data;
	struct trie **list;
	char **anagrams;
	int anagram_index = 0;
	int index = 0;
};
