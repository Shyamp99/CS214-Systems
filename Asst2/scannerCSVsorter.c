#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


int fileType(char* name){
	char* file;
	char* dir;
	file = strstr(name,".csv");
	if(file)
		return 1;
	dir = strstr(name,".");
	if(dir)
		return 0;
	else
		return -1;
}

void forkDirectory(DIR *dp){
	char* buff =(char*) malloc(1000*sizeof(char));
	struct dirent *currFile;
	int child;

	do{
		//child=fork();
		currFile = readdir(dp);
		buff = currFile->d_name;
		if(child==0){
			if(fileType(buff) == 1){
				//sort(File);
			}
			else if(fileType(buff) == -1){
				//forkDirectory(dir);
			}
			else{
			}
		}
	}while(child==0 && buff!=NULL);
	//wait();
}

char* addSorted(char* name){
	char new [sizeof(name)/sizeof(char)+6];
	int i;
	for(i = 0; i<sizeof(name)+6; i+=1){
		if(i < sizeof(name)+3+1){
			if(name[i] == '.'){
				new[i] = 's';
				new[i+1] = 'o';
				new[i+2] = 'r';
				new[i+3] = 't';
				new[i+4] = 'e';
				new[i+5] = 'd';
			}
		}
		else{
			new[i] = name[i-6];
		}
	}
	return new;	
	
}

void prepareFile(char* info){//Take all the command line stuff from here and put it into scannerCSVsorter.c and change this to a method
	//there should be no argc and argv used below pass the info as variables
  struct LList *dataList=malloc(sizeof(struct dataList *));
  struct fieldList *fList=malloc(sizeof(struct fieldList *));
  fList->head=NULL;
  dataList->head=NULL;
  fList->length=0;
  char buff[5000];//buffer for fgets
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
  FILE newfp;
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
  //NEED TO ADD THE SORTED TAG TO THE NAME
  newfp = fopen(info,"w");
  struct Node* n=dataList->head;
  while(n!=NULL){
    for(i=0;i<fList->length-1;i++){
      fprintf(newfp, "%s,",n->data[i]);
    }
    printf(newfp, "%s\r",n->data[i]);
    n=n->next;
  }
  fclose(filePtr);
  fclose(newfp);
}
