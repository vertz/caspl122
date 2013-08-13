#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	  FILE* pFile;
	  FILE* output=0;
	  char c;
	  int k=1;
	  int num,i1,i2,n;
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

	    if ( argv[k][0] == '-' &&  argv[k][1] == 'u'){
		  char* limit= argv[k+1];
		  n= atoi(limit);
		  k=k+2;
	    }
	  }
	  for (num=k ; num < argc; num++){
		i1=1;i2=1;
		pFile= fopen(argv[num],"r");
		if (output==NULL){
			printf("%s",argv[num]); 
			printf("%c",'\n'); 
		}
		else{
			fputs(argv[num], output); 
			fputc('\n', output);
		}
		while ( (c = fgetc (pFile)) != EOF && i2<=line_num) {
			if ( i1 == n){
				if (output==NULL){
					printf("%c",c); 
				}
				else{
					fputc(c, output); 
				}
				i1=0;
			}
			if(c == '\n'){
				if(i1>0){
					if (output==NULL){
						printf("%c",c); 
					}
					else{
						fputc(c, output); 
					}
				}
				i2++;
			}
			i1++;
		}
		fclose (pFile);
		if (output==NULL){
			printf("%c",'\n'); 
		}
		else{
			fputc('\n', output); 
		}
	}
	  
	  if (output!=NULL){
		fclose (output);
	  }
	  
	  return 0;
}