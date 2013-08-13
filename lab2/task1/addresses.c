#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;
int addr6;

int foo();
void point_at(void *p);

int main (int argc, char** argv){
	static int addr1;
	int addr2;
	int addr3;
	char* yos="ree";
	int * addr4 = (int*)(malloc(50));
	printf("add1: %p\n",&addr1);/*text*/
	printf("add2: %p\n",&addr2); /*stack*/
	printf("add3: %p\n",&addr3);/*stack*/
	printf("foo: %p\n",foo);  
	printf("add5: %p\n",&addr5);/*text*/
	point_at(&addr5);
	printf("add6: %p\n",&addr6);/*text*/
	printf("yos: %p\n",yos); /*text*/
	printf("add4: %p\n",addr4);/*heap*/
	return 0;
}

int foo(){
	return -1;
}

void point_at(void *p){
	int local;
	int dist1 = (size_t)&addr6 - (size_t)p; /*text*/
	int dist2 = (size_t)&local - (size_t)p;/*stack*/
	int dist3 = (size_t)&foo - (size_t)p;/*text*/
	printf("(text) dist1: %d\n",dist1);
	printf("(stack) dist2: %d\n",dist2);
	printf("dist3: %d\n",dist3);
}
