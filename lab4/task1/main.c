#include "util.h"

#define STDOUT 1
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6

int main (int argc , char* argv[], char* envp[])
{
  int c=0,j=1,k=10;
  int rFile=0,wFile=0;
  char* str;
  if(2 < argc){
	if(strcmp(argv[1],"-n")==0){
	     j=3;
             k=atoi(argv[2]);
  	}
  }
   if(j < argc){
        rFile = system_call(SYS_OPEN,argv[j],0,0);
	if(rFile<0){
		str="cannot open input file.\n";
		system_call(SYS_WRITE,STDOUT,str,strlen(str));
		system_call(SYS_EXIT, 0x55);
	}
	j=4;
	if(j < argc){
		wFile = system_call(SYS_OPEN,argv[j],64|1,0600);
		if(wFile<0){
			str="cannot open output file.\n";
			system_call(SYS_WRITE,STDOUT,str,strlen(str));
			system_call(SYS_EXIT, 0x55);
		}
	}
	char buf[1];

	while(system_call(SYS_READ,rFile,buf,1)){
		if(buf[0]=='\n'){
			c=0;
			if(wFile <= 0){
				system_call(SYS_WRITE,STDOUT,"\n",1);
			}
			else{
				system_call(SYS_WRITE,wFile,"\n",1);
			}
		}
		else if(c < k){
			c++;
		}
		else{
			if(wFile <= 0){
				system_call(SYS_WRITE,STDOUT,buf,1);
			}
			else{
				system_call(SYS_WRITE,wFile,buf,1);
			}
		}
	}
	if(wFile <= 0){
		system_call(SYS_CLOSE, rFile);
	}
	else{
		system_call(SYS_CLOSE, rFile);
		system_call(SYS_CLOSE, wFile);
	}
	system_call(SYS_WRITE,wFile,"\n",1);
  }
  return 0;
}
