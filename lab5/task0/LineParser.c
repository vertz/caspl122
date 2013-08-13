#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "LineParser.h"

#ifndef NULL
    #define NULL 0
#endif

#define FREE(X) if(X) free((void*)X)

void allocatePipes(cmdLine *head)
{
  if (!head || !head->next)
    return;
  
  *((int**)&head->outputPipe) = (int*) (malloc(2 * sizeof(int)));
  *((int**)&head->next->inputPipe) = head->outputPipe;
  allocatePipes(head->next);
}

char* cloneFirstWord(char* str)
{
    char* start = NULL;
    char* end = NULL;
    char* word;

    while (!end) {
        switch (*str) {
            case '>':
            case '<':
            case 0:
                end = str - 1;
                break;
            case ' ':
                if (start)
                    end = str - 1;
                break;
            default:
                if (!start)
                    start = str;
                break;
        }
        str++;
    }

    if (start == NULL)
        return NULL;

    word = (char*) malloc(end-start+2);
    strncpy(word, start, ((int)(end-start)+1)) ;
    word[ (int)((end-start)+1)] = 0;

    return word;
}

void extractRedirections(char* strLine, cmdLine *pCmdLine)
{
    char* s = strLine;

    while ( (s = strpbrk(s,"<>")) ) {
        if (*s == '<') {
            FREE(pCmdLine->inputRedirect);
            pCmdLine->inputRedirect = cloneFirstWord(s+1);
        }
        else {
            FREE(pCmdLine->outputRedirect);
            pCmdLine->outputRedirect = cloneFirstWord(s+1);
        }

        *s++ = 0;
    }
}


char* strClone(const char* source)
{
    char* clone = (char*)malloc(strlen(source) + 1);
    strcpy(clone, source);
    return clone;
}

int isEmpty(const char* str)
{
  if (!str)
    return 1;
  
  while (*str)
    if (!isspace(*(str++)))
      return 0;
    
  return 1;
}

cmdLine* parseSingleCmdLine(const char* strLine)
{
    char* delimiter = " ";
    char *line, *result;
    
    if (isEmpty(strLine))
      return NULL;
    
    cmdLine* pCmdLine = (cmdLine*)malloc( sizeof(cmdLine) ) ;
    memset(pCmdLine, 0, sizeof(cmdLine));
    
    line = strClone(strLine);
         
    extractRedirections(line, pCmdLine);
    
    result = strtok( line, delimiter);    
    while( result && pCmdLine->argCount < MAX_ARGUMENTS-1) {
        ((char**)pCmdLine->arguments)[pCmdLine->argCount++] = strClone(result);
        result = strtok ( NULL, delimiter);
    }

    FREE(line);
    return pCmdLine;
}

cmdLine* _parseCmdLines(char* line)
{
	char *nextStrCmd;
	cmdLine *pCmdLine;
	char pipeDelimiter = '|';
	
	if (isEmpty(line))
	  return NULL;
	
	nextStrCmd = strchr(line , pipeDelimiter);
	if (nextStrCmd)
	  *nextStrCmd = 0;
	
	pCmdLine = parseSingleCmdLine(line);
	if (!pCmdLine)
	  return NULL;
	
	if (nextStrCmd)
	  pCmdLine->next = _parseCmdLines(nextStrCmd+1);

	return pCmdLine;
}

cmdLine* parseCmdLines(const char* strLine)
{
	char* line, *ampersand;
	cmdLine *head, *last;
	
	if (isEmpty(strLine))
	  return NULL;
	
	line = strClone(strLine);
	if (line[strlen(line)-1] == '\n')
	  line[strlen(line)-1] = 0;
	
	ampersand = strchr( line,  '&');
	if (ampersand)
	  *(ampersand) = 0;
		
	if ( (last = head = _parseCmdLines(line)) )
	{	
	  while (last->next)
	    last = last->next;
	  last->blocking = ampersand? 0:1;
	}
	
	FREE(line);
	allocatePipes(head);
	return head;
}


void freeCmdLines(cmdLine* pCmdLine)
{
  int i;
  if (!pCmdLine)
    return;

  FREE(pCmdLine->outputPipe);
  FREE(pCmdLine->inputRedirect);
  FREE(pCmdLine->outputRedirect);
  for (i=0; i<pCmdLine->argCount; ++i)
      FREE(pCmdLine->arguments[i]);

  if (pCmdLine->next)
	  freeCmdLines(pCmdLine->next);

  FREE(pCmdLine);
}

int replaceCmdArg(cmdLine *pCmdLine, int num, const char* newString)
{
  if (num >= pCmdLine->argCount)
    return 0;
  
  FREE(pCmdLine->arguments[num]);
  ((char**)pCmdLine->arguments)[num] = strClone(newString);
  return 1;
}