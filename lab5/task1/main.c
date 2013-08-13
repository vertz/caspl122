#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "LineParser.h"


#define MAX_BUF 2048
#define MAX_HISTORY 10  
#define OVER_WRITE 1

extern char **environ;
char history[MAX_HISTORY+1][MAX_BUF];
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

int execute(cmdLine* pCmdLine){
	int cpid =0;
	int enve;
	int i,j,exc,sta,index=0;
	char* name,*as,*to=0;

	while (index<(pCmdLine->argCount)){
		if (pCmdLine->arguments[index][0]=='$') {
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
			printf("\n");			
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
			printf("\n");
			for(i=env_count-env;i<env_count;i++){
			    printf("%s \n",environ[i]);
			}
		}
		else{
			printf("\n");
			cpid = fork();
			if (cpid == 0){	
				if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1){
					perror("An error occurred");
					exit(0);
				}
			}
			if(pCmdLine->blocking){
				waitpid(-1,&sta,0);
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
		printf("%s:",cdir);
		fgets(buff, MAX_BUF, stdin);
		cmd = parseCmdLines(buff);
		if (!cmd){
			continue; 
		}
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