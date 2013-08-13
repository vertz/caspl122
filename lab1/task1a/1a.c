 
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	  char c;
	  int line_num=2;
	  if(argc>1){
	   if ( argv[1][0] == '-' &&  argv[1][1] == 'n'){
		char* limit= argv[2];
		line_num= atoi(limit);
	    } 
	  }
	  int i=1;
	  while ( i<=line_num && (c = fgetc (stdin))  ) {
	      printf("%c",c); /*print a character*/
	      if(c == '\n'){
		      i++;
	      }
	  }
	  return 0;
}