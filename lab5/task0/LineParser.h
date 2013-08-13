#define MAX_ARGUMENTS 256

typedef struct cmdLine
{
    char* const arguments[MAX_ARGUMENTS]; /* command line arguments (arg 0 is the command)*/
    int argCount;		/* number of arguments */
    char const* inputRedirect;	/* input redirection path. NULL if not redirected */
    char const* outputRedirect;	/* output redirection path. NULL if not redirected */
    char blocking;		/* boolean indicating blocking/non-blocking */
    int* const inputPipe;	/* pointer to input pipe. NULL if do not exist */
    int* const outputPipe;	/* pointer to output pipe. NULL if do not exist */
    struct cmdLine* next;	/* next cmdLine in chain */    
} cmdLine;

/* Parses a given string to arguments and other indicators */
/* Returns NULL when there's nothing to parse */ 
/* When successful, returns a single cmdLine or a linked list
(when piping is indicated) */
cmdLine* parseCmdLines(const char* strLine);	/* Parse string line */

/* Releases all allocated memory for the chain (linked list) */
void freeCmdLines(cmdLine* pCmdLine);		/* Free parsed line */

/* Replaces arguments[num] with newString */
/* Returns 0 if num is out-of-range. If Ok: returns 1 */
int replaceCmdArg(cmdLine *pCmdLine, int num, const char* newString);  