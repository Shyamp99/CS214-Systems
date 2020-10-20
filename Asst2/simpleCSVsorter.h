/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//Suggestion: define a struct that mirrors a record (row) of the data set

//make compare to

char* trim(char* s);
int compareTo(char* s1, char* s2);
struct LList{
	int length;
	struct Node* head;
};

struct Node{
	char** data;//holds all fields
	struct Node* next;
};
//Linked List would also be good for extra credit to store the fields
//this would allow to make a char** pointer for movie struct
struct fieldList{
	int length;
	struct fieldNode* head;
};
struct fieldNode{
	char* field;
	struct fieldNode* next;
};
struct Node* addNode(char** d, struct LList** f, int l);
void addField( char* d, struct fieldList* f);
struct Node* mergeThem(struct Node* l, struct Node* r, int compareBy);
void mergeSort(struct Node** head, int compareBy);
void parseList(struct LList *d, int length);
char** tokenParse( char** t, int length, char** ret);
char * trimQuotes(char* s);
