#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	  FILE* pFile;
	  FILE* output=0;
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
	    if ( argv[k][0] == '-' &&  argv[k][1] == 'o'){
		output= fopen(argv[k+1],"w");
		if (output==NULL){
		    perror ("Error opening file");
		}
		k=k+2;
	    }
	  }
	  for(num=k ; num<argc ;num++){
	      i=1;
	      pFile= fopen(argv[num],"r");
	      if (output==NULL){
		printf("%s",argv[num]); 
		printf("%c",'\n'); 
	      }
	      else{
		fputs(argv[num], output);
		fputc('\n', output);
	      }
	      while ( i<=line_num && (c = fgetc (pFile)) != EOF  ) {
		  if (output==NULL){
			printf("%c",c); 
		  }
		  else{
			fputc(c, output); 
		  }
		  if(c == '\n'){
			  i++;
		  }
	      }
	      if (output==NULL){
		printf("%c",'\n'); 
	      }
	      else{
		fputc('\n', output); 
	      }
	      fclose (pFile);
	  }
	  
	  if (output!=NULL){
		fclose (output);
	  }
	  
	  return 0;
}