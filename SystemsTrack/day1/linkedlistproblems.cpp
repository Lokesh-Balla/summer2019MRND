// linkedlistproblems.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream> 
#include <iterator> 
#include <unordered_map>

using namespace std;

struct node{
	int data;
	struct node *next;
	struct node *link;
};

int len_of_ll(struct node *root){
	int count = 0;
	while (root != NULL){
		count++;
		root=root->next;
	}
	return count;
}

struct node *createlist(int arr[], int l){
	struct node *start = (struct node*)malloc(sizeof(struct node));
	start->data = arr[0];
	start->next = NULL;
	struct node *temp = start;
	for (int i = 1; i < l; i++){
		struct node *tmp = (struct node*)malloc(sizeof(struct node));
		tmp->data = arr[i];
		tmp->next = NULL;
		temp->next = tmp;
		temp = tmp;
	}
	return start;
}

struct node *rev(struct node *root){
	struct node *temp = root->next;
	struct node * temp1 = root;
	root->next = NULL;
	while (temp != NULL){
		struct node *dummy = temp;
		temp = temp->next;
		dummy->next = temp1;
		temp1 = dummy;
	}
	root = temp1;
	return root;
}

void test_ll_k_nodes_reverse(){
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	int k = 5;
	int len = 17;
	struct node *root = createlist(arr, len);
	struct node *temp = root;
	struct node *temp1, *start, *link = NULL;
	int cnt = len / k;
	int count = k;
	if (len >= k){
		for (int i = 0; i < cnt; i++){
			count = k;
			start = temp;
			while (count > 0){
				temp1 = temp;
				temp = temp->next;
				count--;
			}
			temp1->next = NULL;
			start = rev(start);
			if (link != NULL){
				link->next = start;
			}
			else{
				root = start;
			}
			while (start != NULL){
				link = start;
				start = start->next;
			}
		}
		link->next = temp;
	}
	while (root != NULL){
		printf("|%d| - > ", root->data);
		root = root->next;
	}
	getchar();
}

struct node *loop_in_ll(struct node *root){
	struct node *slow = root, *fast = root;
	while (slow->next && fast->next){
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast){
			break;
		}
	}
	fast = root;
	while (fast->next != slow->next){
		fast = fast->next;
		slow = slow->next;
	}
	return slow->next;
}

void test_loop_linked_list(){
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	struct node *root = createlist(arr, 10);
	struct node *temp, *traversal = root;
	//just connecting 10 element next to 6th
	for (int i = 0; i < 9; i++)
	{
		if (i == 5){
			temp = traversal;
		}
		traversal = traversal->next;
	}
	traversal->next = temp;
	struct node *result = loop_in_ll(root);
	printf("%d | %p", result->data, result);
	getchar();
}

struct node *clonelist_using_hash_maps(struct node *root){
	struct node *temp = root, *pre = NULL;
	struct node *start = NULL;
	std::unordered_map<struct node*, struct node*> adr;
	while (temp != NULL){
		struct node *dummy = (struct node*)malloc(sizeof(struct node));
		dummy->data = temp->data;
		dummy->next = NULL;
		dummy->link = NULL;
		if (start == NULL){
			start = dummy;
		}
		else
		{
			pre->next = dummy;
		}
		pre = dummy;
		adr[temp] = dummy;
		temp = temp->next;
	}
	temp = root;
	while (temp != NULL){
		struct node *first=adr[temp];
		struct node *second = adr[temp->link];
		first->link = second;
		temp = temp->next;
	}
	return start;
}

struct node *clonelist_using_breaking_reframing(struct node *root){
	struct node *traversal = root, *pre = NULL, *start = NULL;
	while (traversal != NULL){
		struct node *dummy = (struct node*)malloc(sizeof(struct node));
		dummy->data = traversal->data;
		dummy->next = traversal->next;
		traversal->next = dummy;
		traversal = traversal->next->next;
	}
	traversal = root;
	//copying random links
	while (traversal != NULL){
		traversal->next->link = traversal->link->next;
		traversal = traversal->next->next;
	}
	traversal = root;
	start = traversal->next;
	//re-establishing the previous list and making connection in copy list
	while (traversal != NULL){
		struct node *copy_list_ptr = traversal->next;
		traversal->next = traversal->next->next;
		if (traversal->next != NULL){
			copy_list_ptr->next = traversal->next->next;
		}
		else{
			copy_list_ptr->next = traversal->next;
		}
		traversal = traversal->next;
	}
	return start;
}

void test_auxiliary_deep_clone(){
	struct node *node1 = (struct node*)malloc(sizeof(struct node*));
	node1->data = 1;
	struct node *node2 = (struct node*)malloc(sizeof(struct node*));
	node2->data = 2;
	struct node *node3 = (struct node*)malloc(sizeof(struct node*));
	node3->data = 3;
	struct node *node4 = (struct node*)malloc(sizeof(struct node*));
	node4->data = 4;
	struct node *node5 = (struct node*)malloc(sizeof(struct node*));
	node5->data = 5;
	struct node *node6 = (struct node*)malloc(sizeof(struct node*));
	node6->data = 6;
	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = node5;
	node5->next = node6;
	node6->next = NULL;
	node1->link = node3;
	node2->link = node4;
	node3->link = node6;
	node4->link = node2;
	node5->link = node1;
	node6->link = node5;
	//hash map check
	struct node *res1 = clonelist_using_hash_maps(node1);
	while (res1 != NULL){
		printf("|%d | %d | %d|->", res1->data, res1->next, res1->link);
		res1 = res1->next;
	}
	printf("\n\n\n");
	//breaking and reframing
	struct node *res2 = clonelist_using_breaking_reframing(node1);
	while (res2 != NULL){
		printf("|%d | %d | %d|->", res2->data, res2->next, res2->link);
		res2 = res2->next;
	}
	getchar();
}

struct node *add_ll(struct node *n1, struct node *n2){
	int value = n1->data + n2->data;
	struct node *par = NULL;
	struct node *present_value = (struct node*)malloc(sizeof(struct node));
	if (n1->next != NULL && n2->next != NULL){
		par = add_ll(n1->next, n2->next);
	}
	if (par != NULL){
		if (par->data > 9){
			value += par->data / 10;
			par->data = par->data % 10;
		}
		present_value->data = value;
		present_value->next = par;
		return present_value;
	}
	else{
		present_value->data = value;
		present_value->next = NULL;
		return present_value;
	}
}

struct node *add_ll_balance(struct node *root, int carry){
	struct node *par = NULL;
	struct node *present_value = (struct node*)malloc(sizeof(struct node));
	present_value->data = root->data;
	if (root->next != NULL){
		par = add_ll_balance(root->next, carry);
	}
	if (par != NULL){
		if (par->data > 9){
			present_value->data += par->data / 10;
			par->data = par->data % 10;
		}
		present_value->next = par;
		return present_value;
	}
	else{
		present_value->data += carry / 10;
		present_value->next = NULL;
		return present_value;
	}
}

void test_ll_add(){
	int num1[5] = { 9, 9, 9, 9, 9 };
	int num2[3] = { 1, 1, 1 };
	struct node *n1 = createlist(num1, 5);
	struct node *n2 = createlist(num2, 3);
	struct node *preceding = NULL, *start = NULL, *result = NULL, *left_over = NULL, *temp = NULL;
	int len1 = 5, len2 = 3, diff = 0;
	if (len1 > len2){
		diff = len1 - len2 - 1;
		preceding = n1;
		start = n1;
		for (int i = 0; i < diff; i++)
		{
			start = preceding;
			preceding = preceding->next;
		}
		result = add_ll(start, n2);
		temp = preceding->next;
		preceding->next = NULL;
		left_over = add_ll_balance(n1, result->data);
		preceding->next = temp;
		result->data = result->data % 10;
		if (left_over->data > 9){
			struct node *first_node = (struct node*)malloc(sizeof(struct node));
			first_node->next = left_over;
			first_node->data = left_over->data / 10;
			left_over->data = left_over->data % 10;
			left_over = first_node;
		}
		start = left_over;
		while (left_over->next != NULL){
			left_over = left_over->next;
		}
		left_over->next = result;
	}
	else if (len2 > len1){
		diff = len2 - len1 - 1;
		preceding = n2;
		start = n2;
		for (int i = 0; i < diff; i++)
		{
			start = preceding;
			preceding = preceding->next;
		}
		result = add_ll(n1, start);
		temp = preceding->next;
		preceding->next = NULL;
		left_over = add_ll_balance(n1, result->data);
		preceding->next = temp;
		result->data = result->data % 10;
		if (left_over->data > 9){
			struct node *first_node = (struct node*)malloc(sizeof(struct node));
			first_node->next = left_over;
			first_node->data = left_over->data / 10;
			left_over->data = left_over->data % 10;
			left_over = first_node;
		}
		start = left_over;
		while (left_over->next != NULL){
			left_over = left_over->next;
		}
		left_over->next = result;
	}
	else{
		result = add_ll(n1, n2);
		if (result->data > 9){
			struct node *first_node = (struct node*)malloc(sizeof(struct node));
			first_node->next = result;
			first_node->data = result->data / 10;
			left_over->data = result->data % 10;
			result = first_node;
		}
		start = result;
	}
	while (start != NULL){
		printf("|%d| -> ", start->data);
		start = start->next;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_ll_k_nodes_reverse();
	test_loop_linked_list();
	test_auxiliary_deep_clone();
	test_ll_add();
	return 0;
}

