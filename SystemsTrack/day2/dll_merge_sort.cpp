#include <stdio.h>

#include <stdlib.h>

struct node{
	int data;
	struct node *next;
	struct node *pre;
};

int len_of_ll(struct node *root){
	int count = 0;
	while (root != NULL){
		count++;
		root = root->next;
	}
	return count;
}

struct node *create_double_ll(int arr[], int l){
	struct node *start = (struct node*)malloc(sizeof(struct node));
	start->data = arr[0];
	start->next = NULL;
	start->pre = NULL;
	struct node *temp = start;
	for (int i = 1; i < l; i++){
		struct node *tmp = (struct node*)malloc(sizeof(struct node));
		tmp->data = arr[i];
		tmp->next = NULL;
		tmp->pre = temp;
		temp->next = tmp;
		temp = tmp;
	}
	return start;
}

struct node *merge(struct node *left, struct node *right){
	struct node *result = NULL;
	if (left == NULL){
		return right;
	}
	if (right == NULL){
		return left;
	}
	if (left->data < right->data){
		result = left;
		result->next = merge(left->next, right);
		result->next->pre = left;
	}
	else{
		result = right;
		result->next = merge(left, right->next);
		result->next->pre = right;
	}
	return result;
}


struct node *split(struct node *root){
	struct node *traversal = root;
	struct node *head = root;
	int i = 1;
	if (root == NULL){
		return root;
	}
	int len = len_of_ll(root);
	if (len == 0 || len == 1){
		return root;
	}
	while (i < len / 2){
		traversal = traversal->next;
		i++;
	}
	struct node *nlist = traversal->next;
	traversal->next = NULL;

	head=split(head);
	nlist=split(nlist);
	return merge(head, nlist);
	

}

void test_merge_sort(){
	int arr[] = { 10, 40, 5, 75, 6, 3};
	int arr1[] = { 1, 1, 1, 1, 1 ,1};
	int len = 6;
	struct node *root = create_double_ll(arr1, len);
	struct node *res=split(root);
	while (res != NULL){
		printf("%d -> ", res->data);
		res = res->next;
	}
	
}




int main(){
	test_merge_sort();
	return 0;
}