#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define Machine 4
#define m32 1

Elf32_Ehdr *header=0;
Elf32_Shdr *sectionHeader=0;
Elf32_Shdr *symbolInfo=0;
Elf32_Sym  *symbolHeader=0;
void *map;
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

void printSymbols(){
  /*e_shstrndx This member holds the section header table index of the entry associated with the section
  name string table*/
  /*sh_offset This member’s value gives the byte offset from the beginning of the file to the first
  byte in the section*/
  char* string_table=(char *)(map + (sectionHeader + header->e_shstrndx)->sh_offset );
  char* sym_table=0;
  char* sName;
  int symSize=0;
  int cou=-1;
  for (i=0; i < header->e_shnum ;i++){
	sName=string_table+sectionHeader->sh_name;
	/*.symtab This section holds a symbol table*/
	if ((sectionHeader->sh_type)==2){
		symbolHeader=(Elf32_Sym *)(map+sectionHeader->sh_offset);
		symSize=(int)(sectionHeader->sh_size/sectionHeader->sh_entsize);
		cou=sectionHeader->sh_link;
	}
	/*.strtab This section holds strings, most commonly the strings that represent the names associated
	with symbol table entries*/
	/*if(strcmp(sName,".strtab")==0){*/
	if(i==cou){
		sym_table=(char *)(map + sectionHeader->sh_offset);
		printf("%d\n",sectionHeader->sh_offset);
	}
	sectionHeader++;
  }
  sectionHeader=(Elf32_Shdr *)(map + header->e_shoff);
  printf("[%-3s] %-31s %-23s\n","Nr","Name","Section");
  for (i=0; i < symSize ;i++){
	printf("[%3d] ",i);
	printf("%-31s ",sym_table+symbolHeader->st_name);
	if (symbolHeader->st_shndx > 0 && symbolHeader->st_shndx < header->e_shnum){
	    printf("%-23s ",string_table + (sectionHeader + symbolHeader->st_shndx)->sh_name);
	}
	printf("\n");
	symbolHeader++;
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
	
	if(argc>2){
	  if(strcmp( argv[2], "-0") == 0){
	     printElf();
	  }
	  if(strcmp( argv[2], "-1") == 0){
	      check();
	      sectionHeader=(Elf32_Shdr *)(map+header->e_shoff);
	      printSections();
	  }
	  if(strcmp( argv[2], "-2") == 0){
	      check();
	      sectionHeader=(Elf32_Shdr *)(map+header->e_shoff);
	      printSymbols();
	  }
	}
	else{
	      check();
	      sectionHeader=(Elf32_Shdr *)(map+header->e_shoff);
	      printSymbols();
	}
	
	munmap(map, f_stat.st_size);
	close(file);
	return 0;
}