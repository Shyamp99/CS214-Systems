
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "simpleCSVsorter.h"
struct Node* mergeThem(struct Node* l, struct Node* r, int compareBy){
	struct Node* ret=NULL;//default
	if(!l)
		return r;
	if(!r)
		return l;
	//printf("%s,%s,%d\n",l->data[compareBy],r->data[compareBy],compareTo(l->data[compareBy],r->data[compareBy]));
	if(compareTo(l->data[compareBy],r->data[compareBy])<1){
		ret=l;
		l->next=mergeThem(l->next,r,compareBy);
	}
	else{
		ret=r;
		r->next=mergeThem(l,r->next,compareBy);
	}
	return ret;
}

//Suggestion: prototype a mergesort function
void mergeSort(struct Node** head, int compareBy){
        //printf("here\n");
	if((*head)==NULL||(*head)->next==NULL)
		return;
        //printf("here\n");
	struct Node* left=*head;
	struct Node* middle=*head;
	struct Node* right=(*head)->next;
	//printf("here\n");
	while(right!=NULL){
		right=right->next;
		if(right!=NULL){
			middle=middle->next;//this moves the pointer to the middle and finds it regardless of the length
			right=right->next;
		}
	}
	right=middle->next;
	middle->next=NULL;
	mergeSort(&left,compareBy);
	mergeSort(&right,compareBy);
	(*head)=mergeThem(left,right,compareBy);
}
