#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int f1(char* c){
	return strlen(c);
}

int f2(char* c){
	int ans=0,i=0;
	int size=strlen(c);
	while(i<size){
		if(c[i]==' '){
			ans++;
		}
		i++;
	}
	return ans;
}

typedef int (*myFunc)(char*);

void myFilter(char** c_array,int c_size,myFunc f){
	int i=0,k;
	while(i<c_size){
		k=f(c_array[i]);
		if(k>4){
			printf("%s\n",c_array[i]);
		}
		i++;
	}
}

typedef struct {
  char *name;
  int (*func)(char*);
}  fun_desc ;

void myFilter2(){
	fun_desc arr[2];
	arr[0].name="Characters counter"; 
	arr[0].func= &f1;
	arr[1].name="Spaces counter"; 
	arr[1].func= &f2;

	printf("Please choose a function:\n");
	int i;
	for (i=0; i < 2; i++){
		printf("%d. %s\n", i, arr[i].name);
	}

	scanf("%d",&i);
	if ( i<=1 && i>=0){
	    char* str[3];
	    str[0]="hello\0";
	    str[1]="1 2 3 4 5\0";
	    str[2]="1 2 3 4 5 6 7\0";
	    myFilter(str,3,arr[i].func);
	}
}

int main (int argc, char** argv){
	printf("%d\n",f1("hello"));
	printf("%d\n",f2("hello"));
	printf("%d\n",f1("hello world"));
	printf("%d\n",f2("hello world"));
	char* str[3];
	str[0]="hello\0";
	str[1]="1 2 3 4 5\0";
	str[2]="1 2 3 4 5 6 7\0";
	int i;
	for(i=0 ; i<3 ;i++){
	  printf("%d) %s\n",i,str[i]);
	}
	printf("myFilter f1 :\n");
	myFilter(str,3,&f1);
	printf("myFilter f2 :\n");
	myFilter(str,3,&f2);
	printf("myFilter2 :\n");
	myFilter2();
	return 0;
}
