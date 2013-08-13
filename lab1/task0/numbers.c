#include <stdio.h>
#include <stdlib.h>

void getNumbers(int * a, int * b){
  char buff[256];
  printf("insert 2 numbers:\n");
  fflush(stdout);
  fgets(buff,254,stdin);
  *a=atoi(buff);
  fgets(buff,255,stdin);
  *b=atoi(buff);
}
