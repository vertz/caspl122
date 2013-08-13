 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_BUF 2048

int main (int argc, char ** argv){
  int fd[2];
  char buf[MAX_BUF];
  int cpid;
  if (pipe(fd)<0){
	perror("An error occurred");
	exit(errno);
  }
  cpid=fork();
  if (cpid==0){
  	close(fd[0]);	
  	write(fd[1],"hello",6);
  	exit(0);
  }
  else{
  	close(fd[1]);
  	read(fd[0],buf,sizeof(buf));
  	printf("%s\n",buf);
  }
  return 0;
}