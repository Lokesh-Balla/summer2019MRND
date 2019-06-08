#include<stdio.h>
#include<stdlib.h>

void compress_string(){
	char *str = (char *)malloc(sizeof(char) * 100);
	int count = 0, current = 0, traversal = 0;
	int index = 0;
	printf("Enter the String\n");
	scanf("%s", str);
	while (str[traversal] != '\0'){
		if (str[traversal] == str[current]){
			count++;
			traversal++;
		}
		else{
			if (count > 1){
				char *temp = (char *)malloc(sizeof(char) * 10);
				index = 0;
				while (count != 0){
					temp[index++] = count % 10 + '0';
					count = count / 10;
				}
				for (int i = index - 1; i >= 0; i--)
				{
					str[current + i + 1] = temp[index - i - 1];
				}
				current += index + 1;
				str[current] = str[traversal];
			}
			else{
				current++;
			}
			count = 1;
			traversal++;
		}
	}
	index = 0;
	if (count > 1){
		char *temp = (char *)malloc(sizeof(char) * 10);
		while (count != 0){
			temp[index++] = count % 10 + '0';
			count = count / 10;
		}
		for (int i = index - 1; i >= 0; i--)
		{
			str[current + i + 1] = temp[i];
		}
		current += index + 1;
		count = 1;
	}
	else{
		current++;
	}
	str[current] = '\0';
	printf("%s", str);
}

int main(){
	compress_string();
	return 0;
}