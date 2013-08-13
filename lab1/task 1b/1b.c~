 
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	  FILE* pFile;
	  char c;
	  int k=1;
	  int num,i;
	  int line_num=2;
	  if(argc>1){
	   if ( argv[k][0] == '-' &&  argv[k][1] == 'n'){
		char* limit= argv[k+1];
		line_num= atoi(limit);
		k=k+2;
	    }
	  }
	  for(num=k ; num<argc ;num++){
	      i=1;
	      pFile= fopen(argv[num],"r");
	      printf("%s",argv[num]); 
	      printf("%c",'\n');
	      while ( i<=line_num && (c = fgetc (pFile)) != EOF  ) {
		  printf("%c",c); 
		  if(c == '\n'){
			  i++;
		  }
	      }
	      fclose (pFile);
	  }
	  return 0;
}