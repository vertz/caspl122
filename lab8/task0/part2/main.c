#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#define Machine 4

Elf32_Ehdr *header;
char ELF[4]={0x7f,0x45,0x4c,0x46};
int i,j;

void printMagic(){
    for(j=0 ; j<2 ; j++){
	    printf ("Magic: ");
	    for (i=1 ; i < EI_NIDENT; i++){
		if(i<4){
		  if (header->e_ident[i] != ELF[i]){
			if(i>1){
			  printf ("\n");
			}
			/*perror("Not an ELF file - it has the wrong magic bytes at the start");*/
			printf ("Not an ELF file - it has the wrong magic bytes at the start\n");
			exit(-1);
		  }
		}
		if(j==0){
		    printf ("%c ", header->e_ident[i]);
		}
		else{
		    printf ("%x ", header->e_ident[i]);
		}
	    }
	    printf ("\n");
    }
}

void printType(){
    printf ("Type: ");
    if(header->e_type==0){
	printf("No file type");
    }
    if(header->e_type==1){
	    printf("Relocatable file");
    }
    if(header->e_type==2){
	    printf("Executable file");
    }
    if(header->e_type==3){
	    printf("Shared object file");
    }
    if(header->e_type==4){
	    printf("Core file");
    }
    if(header->e_type==0xff00){
	    printf("Processor-specific");
    }
    if(header->e_type==0xffff){
	    printf("Processor-specific");
    }
    printf ("\n");
}

void printMachine(){
    if (header->e_ident[Machine] == 0){
	printf ("%s\n", "Invalid class");
    }
    if (header->e_ident[Machine] == 1){
	printf ("%s\n", "32-bit object");
    }
    if (header->e_ident[Machine] == 2){
	printf ("%s\n", "64-bit object");
    }
}

void printElf(){
    printMagic();
    printType();
    printf ("Entry point:  0x%x\n",header->e_entry);
    printf ("e_shstrndx:  %d\n",header->e_shstrndx);  /*Section header string table index*/
    printf ("The number of section header entries:  %d\n",header->e_shnum);
    printf ("The number of program header entries:  %d\n",header->e_phnum);
    printf ("The file offset in which the section header table resides:  %d\n",header->e_shoff);
    printMachine();
}

int main(int argc, char **argv) {
	struct stat f_stat;
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
	
	printElf();
	
	munmap(map, f_stat.st_size);
	close(file);
	return 0;
}
