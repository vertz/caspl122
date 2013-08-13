
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LineParser.h"

#define MAX_BUF 2048

int execute(cmdLine* pCmdLine){
	int cpid =0;
        if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1){
		perror("An error occurred");
		exit(0);
	}
	return cpid;
}

int main(int argc, char **argv)
{
	char buff[MAX_BUF];
	cmdLine *cmd = NULL;
	char cdir[MAX_BUF];
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
		execute(cmd);
		freeCmdLines(cmd);
		printf("\n");
	}
 	return 0;
}
