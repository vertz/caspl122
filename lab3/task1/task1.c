 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct entry entry;
struct entry {
 entry *next;
 char *lastname;
 int count;
};

/* If there is an entry in the list with last_name - add 1 to the 
       counter and return list. Otherwise, add a new entry to the end of the list and return list. 
       If list is null - create a new entry and return a pointer to the entry. */
entry* register_entry(entry* list, char * last_name){
	struct entry *current=list;
	int bol=0;
	while ( (current != NULL) && (bol == 0) ){
		if ( strcmp(current->lastname, last_name) == 0 ){
			current->count++;
			bol=1;
		}
		else if(current->next != NULL){
			current=current->next;
		}
		else{
			bol=2;
		}
	}
	if(bol != 1){
		char* newLastname=(char*)malloc(sizeof(char)*(strlen(last_name)+1));
		struct entry *nextEnt=(entry*)malloc( sizeof(struct entry) );
		newLastname=strcpy(newLastname, last_name);
		nextEnt->lastname=newLastname;
		nextEnt->count=1;
		nextEnt->next=NULL;
		if(current != NULL){
			current->next=nextEnt;
		}
		else{
			list=nextEnt;
		}
	}
	return list;
}

/*For each entry in the list apply the function handler (that is given as a parameter).*/
void foreach(entry* list, void (*handler)(entry * node)){
	struct entry *current=list;
	while ( current != NULL ){
		handler(current);
		current=current->next;
	}
} 

/*Print one entry to the standard output (e.g. "Cohen - 2")*/
void print_entry(entry * node){
	if(node != NULL){
		printf("%s - %d\n",node->lastname,node->count);
	}
} 

/* Return the list length */
int list_length(entry* list){
	int ans=0 ;
	struct entry *current=list;
	while ( current != NULL ){
		ans++;
		current=current->next;
	}
	return ans;
} 

/* Free the memory allocated by the list */
void free_list(entry* list){
	struct entry *current=list;
	struct entry *toDelete=list;
	while ( current != NULL ){
		current=current->next;
		free(toDelete->lastname);
		free(toDelete);
		toDelete=current;
	}
}

void testLL(){
  entry * list = register_entry(NULL, "last_name1");
  list = register_entry(list , "last_name1");
  list = register_entry(list , "last_name2");
  printf("list contains %d entries:\n", list_length(list));
  foreach(list, print_entry);
  free_list(list);
}

void task1b(char *nameF){
  	FILE* pFile = fopen(nameF,"r");
	if (pFile==NULL){
		perror ("Error opening file");
	}
	else{	
		entry * list=NULL;
		int maxLength=254;
		char s[maxLength],s1[maxLength],s2[maxLength];
		while(fgets(s, maxLength, pFile) != NULL){
			sscanf(s, "%s %[^\r\n]", s1, s2);
			list = register_entry(list,s2);
		}
		printf("There are %d different last names:\n", list_length(list));
		foreach(list, print_entry);
		free_list(list);
	}
	fclose (pFile);
}

int main(int argc, char **argv) {
	if(argv[1] != NULL){
	  task1b(argv[1]); 
	}
	return 0;
}
