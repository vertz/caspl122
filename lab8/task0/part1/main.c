#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

char ELF[4]={0x7f,0x45,0x4c,0x46};
int i;

int main(int argc, char **argv) {
	struct stat f_stat;
	Elf32_Ehdr *header;
	int file;
	void *map;
	
	if( (file=open(argv[1], O_RDWR)) < 0 ) {
     		  perror("Error in open");
     		  exit(errno);
   	}
	
	if( fstat(file, &f_stat) != 0 ) {
		  perror("Stat failed");
		  exit(errno);
	}
	
	if ((map=mmap(0, f_stat.st_size, PROT_READ , MAP_SHARED, file, 0)) < 0 ) {
		  perror("Map failed");
		  exit(errno);
	}
	
	header=(Elf32_Ehdr *)map;
	
	printf ("Magic: ");
	for (i=1 ; i < 4; i++){
	    if (header->e_ident[i] != ELF[i]){
		    if(i>1){
		      printf ("\n");
		    }
		    /*perror("Not an ELF file - it has the wrong magic bytes at the start");*/
		    printf ("Not an ELF file - it has the wrong magic bytes at the start\n");
		    return -1;
	    }
	    else{
		    printf ("%c ", header->e_ident[i]); /*u can change to x and get 45 4c 46*/
	    }
	}
	printf ("\n");
	
	printf("Entry point address:  0x%x\n",header->e_entry);
	
	munmap(map, f_stat.st_size);
	close(file);
	return 0;
}
