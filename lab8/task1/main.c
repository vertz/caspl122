#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define Machine 4
#define m32 1

Elf32_Ehdr *header;
Elf32_Shdr *sectionHeader;
void *map;
char ELF[4]={0x7f,0x45,0x4c,0x46};
int i;

void check(){
    i=0;
    while(i<4){
	if (header->e_ident[i] != ELF[i]){
		/*perror("Not an ELF file - it has the wrong magic bytes at the start");*/
		printf ("Error: Not an ELF file (wrong magic bytes at the start)\n");
		exit(-1);
	}
	i++;
    }
    if (header->e_ident[Machine] != m32){
	/*perror("Not an ELF file - it has the wrong magic bytes at the start");*/
	printf ("Error: Not 32bit ELF file\n");
	exit(-1);
    }
}

int printStype(int typeNum){
    if(typeNum==0){
	    printf("%-15s ","NULL");
	    return 1;
    }
    if(typeNum==1){
	    printf("%-15s ","PROGBITS");
	    return 1;
    }
    if(typeNum==2){
	    printf("%-15s ","SYMTAB");
	    return 1;
    }
    if(typeNum==3){
	    printf("%-15s ","STRTAB");
	    return 1;
    }
    if(typeNum==4){
	    printf("%-15s ","RELA");
	    return 1;
    }
    if(typeNum==5){
	    printf("%-15s ","HASH");
	    return 1;
    }
    if(typeNum==6){
	    printf("%-15s ","DYNAMIC");
	    return 1;
    }
    if(typeNum==7){
	    printf("%-15s ","NOTE");
	    return 1;
    }
    if(typeNum==8){
	    printf("%-15s ","NOBITS");
	    return 1;
    }
    if(typeNum==9){
	    printf("%-15s ","REL");
	    return 1;
    }
    if(typeNum==10){
	    printf("%-15s ","SHLIB");
	    return 1;
    }
    if(typeNum==11){
	    printf("%-15s ","DYNSYM");
	    return 1;
    }
    return 0;
}

void printSections(){
  /*e_shstrndx This member holds the section header table index of the entry associated with the section
  name string table*/
  /*sh_offset This member’s value gives the byte offset from the beginning of the file to the first
  byte in the section*/
  char* string_table=(char *)(map + (sectionHeader + header->e_shstrndx)->sh_offset );
  printf("[%-3s] %-23s %-15s %-6s\n","Nr","Name","Type","Size");
  /*e_shnum This member holds the number of entries in the section header table*/
  for (i=0; i < header->e_shnum ;i++){
	printf("[%3d] ",i);
	printf("%-23s ",string_table+sectionHeader->sh_name);
	if(printStype(sectionHeader->sh_type)!=1){
	  printf("0x%-14x",sectionHeader->sh_type);
	}
	printf("%06x\n",sectionHeader->sh_size);
	sectionHeader++;
  }
}

int main(int argc, char **argv) {
	struct stat f_stat;
	int file;
	
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
	
	check();
	
	/*e_shoff This member holds the section header table’s file offset in bytes.*/
	sectionHeader=(Elf32_Shdr *)(map+header->e_shoff);
	
	printSections();

	munmap(map, f_stat.st_size);
	close(file);
	return 0;
}
