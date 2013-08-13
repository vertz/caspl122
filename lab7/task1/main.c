#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FUNC_NUM 6

Display(char* filename){
	int len=0,loc=0;
	FILE *file;

	if((file=fopen(filename,"r"))==0 ) {
     		 perror("Error in open");
     		 exit(errno);
   	}

	printf("choose length and location:\n");
	if(scanf("%d %d", &len,&loc)!=2){
		perror("Error in scanf");
     		exit(-1);
	}
	
	if(fseek(file ,loc , SEEK_SET )<0){
		perror("Error in seek");
     		exit(errno);
	}
	
	while (len>0) { 
		printf( " %02X", fgetc(file) ) ;
		len-- ;
	}
	
	fclose(file);
	printf("\n");
}

DisplayA(char* filename){
	int len=0,loc=0,c;
	FILE *file;

	if((file=fopen(filename,"r"))==0 ) {
     		 perror("Error in open");
     		 exit(errno);
   	}

	printf("choose length and location:\n");
	if(scanf("%d %d", &len,&loc)!=2){
		perror("Error in scanf");
     		exit(-1);
	}
	
	if(fseek(file ,loc , SEEK_SET )<0){
		perror("Error in seek");
     		exit(errno);
	}
	
	while (len>0) { 
		c=fgetc(file); 
		if ((c>31) && (c<127)){
			printf( "%c", c );
		}
		else{ 
			printf(".") ;
		}
		len-- ;
	}
	
	fclose(file);
	printf("\n");
}

Modify(char* filename){
	int val=0,loc=0;
	FILE *file;

	if((file=fopen(filename,"r+"))==0 ) {
     		 perror("Error in open");
     		 exit(errno);
   	}

	printf("choose location and val:\n");
	if(scanf("%d %02X", &loc,&val)!=2){
		perror("Error in scanf");
     		exit(-1);
	}
	
	if(fseek(file ,loc , SEEK_SET )<0){
		perror("Error in seek");
     		exit(errno);
	}
	
	fputc(val,file ) ;
	
	fclose(file);
	printf("\n");
}

CopyF(char* filename){
	FILE *src,*target;
	int len=0,sloc=0,tloc=0,c;
	char* name;

	if((src=fopen(filename,"r"))==0 ){
     		 perror("Error in open src");
     		 exit(errno);
   	}
	
	
	printf("choose target, target-location, source-location and length:\n");
	if(scanf("%s %d %d %d",name,&tloc,&sloc,&len)!=4){
		perror("Error in scanf");
     		exit(-1);
	}
	
	if((target=fopen(name,"w+"))==0 ) {
     		 perror("Error in open target");
     		 exit(errno);
   	}

	if(fseek(src,sloc,SEEK_SET)<0){
		perror("Error in seek");
     		exit(errno);
	}
	
	if(fseek(target,tloc,SEEK_SET)<0){
		perror("Error in seek");
     		exit(errno);
	}

	while (len>0){ 
		c=fgetc(src);
		putc(c,target);
		len--;
	}

	fclose(src);
	fclose(target);
}

CopyM(char* filename){
	FILE *src,*target;
	int len=0,sloc=0,tloc=0,c;
	char* name;

	if((target=fopen(filename,"w+"))==0 ){
     		 perror("Error in open src");
     		 exit(errno);
   	}
	
	
	printf("choose source, source-location, target-location and length:\n");
	if(scanf("%s %d %d %d",name,&sloc,&tloc,&len)!=4){
		perror("Error in scanf");
     		exit(-1);
	}
	
	if((src=fopen(name,"r"))==0 ) {
     		 perror("Error in open target");
     		 exit(errno);
   	}

	if(fseek(src,sloc,SEEK_SET)<0){
		perror("Error in seek");
     		exit(errno);
	}
	
	if(fseek(target,tloc,SEEK_SET)<0){
		perror("Error in seek");
     		exit(errno);
	}

	while (len>0){ 
		c=fgetc(src);
		putc(c,target);
		len--;
	}

	fclose(src);
	fclose(target);
}

Quit(char* filename){
	exit(0);
}

int main(int argc, char **argv)
{
	int file;
	int reply;
	int (*func[FUNC_NUM])(char* filename) = { Display, DisplayA, Modify, CopyF, CopyM, Quit };

	if( (file = open(argv[1], 2)) < 0 ) {
     		 perror("Error in open");
     		 exit(-1);
   	}

	while(1){
		printf("File: %s, choose action:\n", argv[1]);
		printf("1-Display\n");
		printf("2-Display(ascii)\n");
		printf("3-Modify\n");
		printf("4-Copy from file\n");
		printf("5-Copy from mem\n");
		printf("6-Quit\n");
		printf("\n");
		scanf("%d", &reply);
		if(reply>0 && reply<=FUNC_NUM){
			func[reply-1](argv[1]);
		}
		printf("\n");
	}	
	
 	return 0;
}