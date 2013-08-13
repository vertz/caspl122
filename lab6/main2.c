 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 2048
#define STDOUT 1
#define STDIN 0

int main (int argc, char ** argv){

  int fd[2],dupid;
  pid_t cpid1,cpid2;
  char *arg1[3]={"ls","-l",NULL};
  char *arg2[3]={"wc","-l",NULL};


  pipe(fd);
  cpid1=fork();
  if (cpid1==0){
	close(STDOUT);   
	dupid=dup(fd[1]);	/*Duplicate the write-end of the pipe*/
	if(dupid<0){
	    perror("Faild to duplicate out file");
	    exit(errno);
	}
	close(fd[1]);
	execvp (arg1[0],arg1);
	exit(0);
  }
  else{
  	close(fd[1]);   /*parent process: close the write end of the pipe.*/
  	cpid2=fork();
  	if (cpid2==0){
		close(STDIN); 
		dupid=dup(fd[0]);  /*Duplicate the read-end of the pipe*/
		if(dupid<0){
		  perror("Faild to duplicate out file");
		  exit(errno);
		} 
		close(fd[0]);
		execvp (arg2[0],arg2); 
  	}
  	else{
  		close(fd[0]);    /*parent process: close the pipe input.*/
  		waitpid(cpid1,NULL,0);
  		waitpid(cpid2,NULL,0);
  	}
  }

  return 0;
}