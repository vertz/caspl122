 
#include "LineParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_BUF 2048
#define MAX_HISTORY 10  
#define OVER_WRITE 1
#define STDOUT 1
#define STDIN 0

extern char **environ;
cmdLine *tmpCmd=NULL;
char history[MAX_HISTORY+1][MAX_BUF];
char buf[MAX_BUF];
int fd[2];
int env=0,env_count=0;
int countHis=0,printHis=0;

int checkEnv(){
	int i=0;
	while(environ[i]){
		i++;
	}
	if(i==env_count){
		return 0;
	}
	else if(i<env_count){
		env_count=i;
		return -1;
	}
	else{
		env_count=i;
		return 1;
	}
}

void createPipes(cmdLine* pCmdLine){
	while (pCmdLine->next)	{
		if (pipe(pCmdLine->outputPipe)<0){
			perror("Error in creating pipes");
			exit(errno);
		}
		pCmdLine=pCmdLine->next;
	}
}

int execute(cmdLine* pCmdLine){
	int cpid =0;
	int i,j,enve,exc,sta,index=0;
	int dupin,dupout,fdCount=1;
	char* name,*as,*to=0,inc=0;

	while (index<(pCmdLine->argCount)){
		inc=pCmdLine->arguments[index][0];
		if (inc=='$') {
			name = pCmdLine -> arguments[index];
			name++;
			as = getenv(name);
			if(as == NULL){
				printf("Error - variable was not found.\n");
			}
			else{	
				j=0;
				while(!as[j]=='='){
					j++;
				}
				to=(char*) malloc(strlen(as)-j);
				strncpy(to, as+j, strlen(to));
				replaceCmdArg(pCmdLine, index,to);
				free(to);
			}
		}
		else if((inc=='>') || (inc=='<') || (inc=='|')){
			fdCount++;
		}
		index++;
	}

		
		if(strcasecmp(pCmdLine->arguments[0],"chdir") == 0){
			if (chdir(pCmdLine->arguments[1])){
				printf("\n");
				perror("Error in path");
			}
	        }
		else if(strcasecmp(pCmdLine->arguments[0],"hist") == 0){
			printHis=countHis;
			while(printHis>0 && printHis>countHis-MAX_HISTORY){
				printHis--;
				printf("%s\n",history[printHis%MAX_HISTORY]);
			}
		}
		else if(strcasecmp(pCmdLine->arguments[0],"rep") == 0){
			if(countHis>0){
			  pCmdLine = parseCmdLines(history[(countHis-1)%MAX_HISTORY]);
			  exc=execute(pCmdLine);
			  if(pCmdLine->blocking){
				waitpid(exc,&sta,0);
			  }
			}
		}
		else if(strcasecmp(pCmdLine->arguments[0],"print") == 0){
			int printI = 1;
			while(printI<pCmdLine->argCount){
			      printf("%s ",pCmdLine->arguments[printI]);
			      printI++;
			}
			printf("\n");
		}
		else if(strcasecmp(pCmdLine->arguments[0],"set") == 0){
			if(pCmdLine->argCount==3){
			    enve=setenv(pCmdLine->arguments[1], pCmdLine->arguments[2],OVER_WRITE);
			    if(enve<0){
			      perror("An error occurred");
			    }
			    else if(checkEnv()>0){
			       env++;
			    } 
			}
			else{
			   printf("wrong syntex :: set x y");
			   printf("\n");
			}
		}
		else if(strcasecmp(pCmdLine->arguments[0],"unset") == 0){
			if(pCmdLine->argCount==2){
			    enve=unsetenv(pCmdLine->arguments[1]);
			    if(enve<0){
			      perror("An error occurred");
			    } 
			    else if(checkEnv()<0){
			       env--;
			    } 
			}
			else{
			    printf("wrong syntex :: unset x");
			    printf("\n");
			}
		}
		else if(strcasecmp(pCmdLine->arguments[0],"env") == 0){
			for(i=env_count-env;i<env_count;i++){
			    printf("%s \n",environ[i]);
			}
		}
		else{
			tmpCmd=pCmdLine;
			int pidArr[fdCount];
			i=0;
			int bol =0;
			while(tmpCmd){
				cpid = fork();
				if (cpid == 0 ){
					if(tmpCmd->inputRedirect != NULL){
						close(STDIN);
						fopen(tmpCmd->inputRedirect, "r");
					}
					if( tmpCmd->outputRedirect != NULL){
						close(STDOUT);
						fopen(tmpCmd->outputRedirect, "a");
					}
					if( (tmpCmd->inputPipe)!= NULL){
						close(STDIN);
						dupin= dup( (tmpCmd->inputPipe)[0] );  /*Duplicate the read-end of the pipe*/
						if(dupin==(-1)){
							perror("Faild to duplicate input");
							exit(errno);
						}
						close( (tmpCmd->inputPipe)[0] );
					}
					if( (tmpCmd->outputPipe) != NULL){
						close(STDOUT);
						dupout= dup( (tmpCmd->outputPipe)[1] );  /*uplicate the write-end of the pipe*/
						if(dupout==(-1)){
							perror("Faild to duplicate output");
							exit(errno);
						}
						close( (tmpCmd->outputPipe)[1]);
					}
					if (execvp(tmpCmd->arguments[0], tmpCmd->arguments) < 0) {
						perror("Faild to execute command");
						exit(errno);
					}
				}
				else{
					pidArr[i]= cpid;
					i++;
					if( (tmpCmd->outputPipe) != NULL){
						close( (tmpCmd->outputPipe)[1]);
					}
					if( (tmpCmd->inputPipe) != NULL){
						close( (tmpCmd->inputPipe)[0] );
					}
					if ( tmpCmd->blocking == 1 ){
						bol=1;
					}
				}
				tmpCmd= tmpCmd->next;
			}
			if (bol==1){
				for (index=0; index < i; index++){
					waitpid(pidArr[index],NULL,0);
				}
			}
		}	

	
	return cpid;
}


int main(int argc, char **argv)
{
	char buff[MAX_BUF];
	cmdLine *cmd = NULL;
	char cdir[MAX_BUF];

	while(environ[env_count]){
		env_count++;

	}

	while(1){ 
		getcwd(cdir,MAX_BUF);
		printf("<myshell> %s:",cdir);
		fgets(buff, MAX_BUF, stdin);
		cmd = parseCmdLines(buff);
		if (!cmd){
			continue; 
		}
		createPipes(cmd);
		if(strcasecmp(cmd->arguments[0],"quit") == 0){
			freeCmdLines(cmd);
			break;
		}
		else{
			execute(cmd);
		}
		if(countHis>0){
			if(strcasecmp(cmd->arguments[0],"rep") != 0){
				strncpy(history[MAX_HISTORY],buff, strlen(buff)-1);
				if(!(strcasecmp(history[(countHis-1)%MAX_HISTORY],history[MAX_HISTORY])==0)){
			   		 strncpy(history[countHis%MAX_HISTORY],buff, strlen(buff)-1);
			   		 countHis++;
				}
			}
		}
		else if(countHis==0){
			if(strcasecmp(cmd->arguments[0],"rep") != 0){
				strncpy(history[countHis%MAX_HISTORY],buff, strlen(buff)-1);
				countHis++;
			}
		}
		freeCmdLines(cmd);
		printf("\n");
	}
 	return 0;
}