#include "util.h"
#include "task2.h"

#define STDOUT 1
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define MAX_SIZE 8192

int main (int argc , char* argv[], char* envp[])
{
  char buf[MAX_SIZE];
  struct linux_dirent *dnt;
  int sum=0;
  int target=system_call(SYS_OPEN,".",0);
  int size=system_call(SYS_GETDENTS,target,buf,MAX_SIZE);
  dnt=(struct linux_dirent*)buf;
  while(sum<size){
	  if(dnt->d_name[0] != '.'){
		system_call(SYS_WRITE,STDOUT,dnt->d_name,strlen(dnt->d_name));
	  	system_call(SYS_WRITE,STDOUT,"\n",1);
	  }
	  sum+=dnt->d_reclen;
	  dnt=(struct linux_dirent*)(buf+sum);
  }
  system_call(SYS_WRITE,STDOUT,"\n",1);
  return 0;
}