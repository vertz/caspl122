#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_CHMODE 15
#define SYS_ISEEK 19
#define STDOUT 1
#define SYS_EXIT 1

int main (int argc , char* argv[], char* envp[])
{
	char* input_buffer[200];
	char* str;
	int read_desc;
	int write_desc;

	read_desc = system_call(SYS_OPEN ,"pass" ,0 ,777);
	write_desc = system_call(SYS_OPEN ,"pass2" ,  (64 | 2 )  ,777);


	if(read_desc < 0) {
		str="ERROR: could not find input file\n";
		system_call(SYS_WRITE,STDOUT,str,strlen(str));
		system_call(SYS_EXIT,0x55,1,1);
	}
	if(write_desc < 0) {
		str="ERROR: could not find output file\n";
		system_call(SYS_WRITE,STDOUT,str,strlen(str));
		system_call(SYS_EXIT,0x55,1,1);
	}

	while (system_call(SYS_READ ,read_desc ,input_buffer ,1) >0) {
		system_call(SYS_WRITE ,write_desc ,input_buffer ,1);
	}

	system_call(SYS_ISEEK ,write_desc ,0x729 ,0);
	system_call(SYS_WRITE ,write_desc ,argv[1] ,strlen(argv[1]));

	system_call(SYS_CLOSE, read_desc);   
	system_call(SYS_CLOSE, write_desc);

  return 0;
}