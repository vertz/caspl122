#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1

int main (int argc , char* argv[], char* envp[])
{
  if(argc>1){
    char * str;
    int i;
    if(strcmp(argv[1],"-h")==0){
      str="hello there ~!";
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }
    else if(strcmp(argv[1],"-g")==0){
      for(i = 2 ; i < argc ; i++){
	system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
	system_call(SYS_WRITE,STDOUT," ",1);
      }
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }
    else{
	str = itoa(argc);
	system_call(SYS_WRITE,STDOUT, str,strlen(str));
	system_call(SYS_WRITE,STDOUT," arguments \n", 12);
	for (i = 0 ; i < argc ; i++)
	  {
	    system_call(SYS_WRITE,STDOUT,"argv[", 5);
		str = itoa(i);
	    system_call(SYS_WRITE,STDOUT,str,strlen(str));
	    system_call(SYS_WRITE,STDOUT,"] = ",4);
	    system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
	    system_call(SYS_WRITE,STDOUT,"\n",1);
	  }
    }
  }
  return 0;
}
