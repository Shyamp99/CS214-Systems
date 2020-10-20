
//
//	Created by Akshay Niranjan 9/18/18
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "simpleCSVsorter.h"
char* trim(char* s){
  if(s==NULL)
    return NULL;
  char *trimmed=malloc(sizeof(char)*(strlen(s)+1));
  strcpy(trimmed,s);
  while(isspace((unsigned char)trimmed[0])){//derefencing makes it point to just the first character
    trimmed++;
  }
  if(trimmed==NULL)
    return NULL;
  int i=strlen(trimmed)-1;
  while(isspace((unsigned char)trimmed[i])){
    i--;
  }
  if(i==strlen(trimmed)-1){
    return trimmed;
  }
  trimmed[i+1]='\0';
  return trimmed;
}
char * trimQuotes(char* s){
	char* copys=malloc((strlen(s)+1)*sizeof(char));
	strcpy(copys,s);
	if(strlen(copys)>2){
		if(copys[0]=='\"' && copys[strlen(copys)-1]=='\"'){
			copys[strlen(copys)-1]='\0';
			copys++;
		}
	}
	return copys;
}
int compareTo(char* s1, char* s2){
	if(s1==NULL)
		return -1;
	if(s2==NULL)
		return 1;
	char* copys1=trimQuotes(s1);
	char* copys2=trimQuotes(s2);
	char* t1=trim(copys1);//trim first then check if null
	char* t2=trim(copys2);
	if(t1==NULL)
		return -1;
	if(t2==NULL)
		return 1;
	int i1,i2;
	double f1,f2;
	if(sscanf(t1,"%lf",&f1)==1 && sscanf(t2,"%lf",&f2)==1){
		if(f1<f2){
			return -1;
		}
		return 1;
	}
	if(sscanf(t1,"%d",&i1)==1 && sscanf(t2,"%d",&i2)==1){
		if(i1<i2){
			return -1;
		}
		return 1;
	}
	return strcmp(t1,t2);
}
//Trees would sort on the way in but project requires mergesort
//Linked List best for holding Movies since we do not necessarily know the size of any csv document

struct Node* addNode(char** d, struct LList** f, int l){
	(*f)->length++;
	if((*f)->head==NULL){
		(*f)->head=malloc(sizeof(struct Node*));
		(*f)->head->data=d;
		(*f)->head->next=NULL;
		return (*f)->head;
	}
	struct Node *temp=malloc(sizeof(struct Node*));
	temp->data=d;
	temp->next=NULL;
	struct Node *ptr=(*f)->head;
	while(ptr->next!=NULL){
		ptr=ptr->next;
  }
	ptr->next=temp;
  return (*f)->head;
}

void addField( char* d, struct fieldList* f){
	struct fieldNode *temp=malloc(sizeof(struct fieldNode*));
	temp->field=malloc(sizeof(char)*(strlen(d)+1));
	temp->next=malloc(sizeof(f->head));
  strcpy(temp->field,d);
  //printf("im mr meseeks look at me");
	temp->next=f->head;
	f->head=temp;
  f->length++;
}
void parseList(struct LList *d, int length){
  int i;
  struct Node* n=d->head;
  while(n!=NULL){
    for(i=0;i<length;i++){
      printf("%s,",n->data[i]);
    }
    printf("\n");
    n=n->next;
  }
}
void freeList(struct fieldNode *n, int first){
  if(n==NULL){
    return;
  }
  freeList(n->next,0);
  if(first==1){
    printf("%s\r",n->field);
  }
  else{
    printf("%s,",n->field);
  }
  free(n);
}

char** tokenParse( char** t, int length, char** ret){
  int i=strlen(*t);
  int start=0;
  int end=0;
  int count=0;
  while(end<i && count<length){
    if((*t)[end]=='\"'){
      start=end;
      end++;
      while(end+1<i && (*t)[end]!='\"'){
        end++;
      }
      ret[count]=malloc(sizeof(char)*(end-start+2));
      strncpy(ret[count],(*t)+start,end-start+1);
      end+=2;
    }
    else{
      start=end;
      while(end<i && (*t)[end]!=','){
        end++;
      }
      if(start==end){
        ret[count]=malloc(sizeof(char));
        ret[count]='\0';
      }
      else{
        ret[count]=malloc(sizeof(char)*(end-start+1));
        strncpy(ret[count],(*t)+start,end-start);
      }
      end++;
    }
    count++;
  }
}

void fieldParse( char** t,struct fieldList* f){
  int i=strlen(*t);
  int start=0;
  int end=0;
  char *ret;
  while(end<i){
    if((*t)[end]=='\"'){
      start=end;
      end++;
      while(end+1<i && (*t)[end]!='\"'){
        end++;
      }
      ret=malloc(sizeof(char)*(end-start+2));
      strncpy(ret,(*t)+start,end-start+1);
      addField(ret,f);
      end+=2;
    }
    else{
      start=end;
      while(end<i && (*t)[end]!=','){
        end++;
      }
      if(start==end){
        ret=malloc(sizeof(char));
        ret='\0';
        addField(ret,f);
      }
      else{
        ret=malloc(sizeof(char)*(end-start+1));
        strncpy(ret,(*t)+start,end-start);
        addField(ret,f);
      }
      end++;
    }
  }
}

int main(int argc, char** argv){
  if(argc<3){
    fprintf(stderr,"STDERR\n");
    return -1;
  }
  if(strcmp(argv[1],"-c")!=0){
    fprintf(stderr,"STDERR no -c\n");
    return -1;
  }
  struct LList *dataList=malloc(sizeof(struct dataList *));
  struct fieldList *fList=malloc(sizeof(struct fieldList *));
  fList->head=NULL;
  dataList->head=NULL;
  fList->length=0;
  char buff[5000];//buffer for fgets
  freopen("movie_metadata.csv", "r", stdin);
  FILE *filePtr=stdin;
  fgets(buff, 5000, filePtr);
  char* p=strtok(buff,"\r");
  fieldParse(&p,fList);
  if(fList->length<1){
    fprintf(stderr,"STDERR no columns. Nothing to sort\n");
    return -1;
  }
  freopen("redir.csv", "w", stdout);
  while (fgets(buff, 5000, filePtr)){
    char* token=strtok(buff,"\r");
    char**data=malloc(sizeof(char*)*fList->length);
    tokenParse(&token,fList->length,data);
    dataList->head=addNode(data,&dataList,fList->length);
  }
  int j=0;
  int i=0;
  struct fieldNode* tempPtr=fList->head;
  while(tempPtr && compareTo(tempPtr->field,argv[2])!=0){
    tempPtr=tempPtr->next;
    i++;
  }
  if(tempPtr==NULL){
    fprintf(stderr,"STDERR incorrect column name\n");
    return -1;
  }
  i=fList->length-i-1;
  mergeSort(&(dataList->head),i);

  freeList(fList->head,1);

  struct Node* n=dataList->head;
  while(n!=NULL){
    for(i=0;i<fList->length-1;i++){
      printf("%s,",n->data[i]);
    }
    printf("%s\r",n->data[i]);
    n=n->next;
  }
  fclose(filePtr);
  return 1;
}
