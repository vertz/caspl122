#include "lib.h"
#include <errno.h> 

#define NOBITS 8
#define DEBUG 1

/* a global array, MergeSectionIndices[i] == 1 iff we need to merge section
   i from the first file with a section from the second file, with the same name. */
int *MergeSectionIndices;

int IsELF(Elf32_Ehdr *header) {
  if (header->e_ident[EI_MAG0] != ELFMAG0 ||
      header->e_ident[EI_MAG1] != ELFMAG1 ||
      header->e_ident[EI_MAG2] != ELFMAG2 || 
      header->e_ident[EI_MAG3] != ELFMAG3 ||
      header->e_type != ET_REL)
    return(0);
  return(1);
}

int check_undef(void *start, Elf32_Sym *symbol) {
        if(symbol->st_shndx == SHN_UNDEF) {
                printf("Undefined symbol: %s\n", get_symbol_name(symbol, start));
                return(1);
        }
        return(0);
}

/**
  * Here, you will need to go over all symbols in the first input file.
  * If an undefined symbol was found, get it's name, and look for it in
  * the second input file.  Once found, get the name of the section 
  * it appears in, and find the index of a sectioned with such a name
  * in the first file (=i). mark this section as a merge candidate
  * (MergeSectionIndices[i] = 1).
  * Now, fix the value of the symbol in the symbol table of the first file
  * ( sy_in_first_file->st_value = sym_in_second_file->st_value + section_in_first_file->sh_size)
  * and the section index in which it will reside (i).
  *
  */
int resolve_all_symbols(void *start1, void * start2) {

        return 0;
}
                
                

/**
  * a detailed description is given in the lab's html page.
  */
void write_file(int fdOut, char *start1, char *start2) {
	Elf32_Ehdr* header1=(Elf32_Ehdr *)start1;
	Elf32_Ehdr* header2=(Elf32_Ehdr *)start2;
	Elf32_Shdr* sectionHeader1=(Elf32_Shdr *)(start1+header1->e_shoff);
	Elf32_Shdr* sectionHeader2=(Elf32_Shdr *)(start2+header2->e_shoff);
	char* string_table1=(char *)(start1 + (sectionHeader1 + header1->e_shstrndx)->sh_offset );
	char* string_table2=(char *)(start2 + (sectionHeader2 + header2->e_shstrndx)->sh_offset );
	int i,j,newOffset=header1->e_ehsize;
	char* name,*coname;
	int len=header1->e_ehsize;


	char* elfH=malloc(header1->e_ehsize);
	if(elfH == 0){
		perror("Error in malloc header file1 ");
		exit(errno);
	}
	char* elfSH=malloc((header1->e_shentsize)*(header1->e_shnum));
	if(elfSH == 0){
		perror("Error in malloc section header file1 ");
		exit(errno);
	}
	memcpy(elfH,start1,header1->e_ehsize);
	memcpy(elfSH,(char*)(start1+header1->e_shoff),(header1->e_shentsize)*(header1->e_shnum));

	if(DEBUG>1){
		printf("cleared step 0\n");
	}

	Elf32_Ehdr* newHeader=(Elf32_Ehdr *)elfH;
	Elf32_Shdr* newSectionHeader=(Elf32_Shdr *)elfSH;
	for (i=0; i < newHeader->e_shnum ;i++){
		if(i>0){
			newSectionHeader->sh_offset=newOffset;
		}
		if((newSectionHeader->sh_type != NOBITS)){ 
			name=string_table1+newSectionHeader->sh_name;
			if((strcmp(name,".text")==0)||(strcmp(name,".data")==0)||(strcmp(name,".rodata")==0)){
				if(DEBUG>2){
					printf("searching for %s\n",name);
				}
				for (j=0; j < header2->e_shnum ;j++){
					coname=string_table2+sectionHeader2->sh_name;
					if(strcmp(name,coname)==0){
						if(DEBUG>2){
							printf("found %s\n",name);
						}
						newSectionHeader->sh_size=newSectionHeader->sh_size + sectionHeader2->sh_size;
						break;
					}
					sectionHeader2++;
				}
				sectionHeader2=sectionHeader2-j;
				if(DEBUG>2){
					printf("finished searching for %s\n",name);
				}
			}
			newOffset=newOffset+newSectionHeader->sh_size;
		}
		newSectionHeader++;
	}
	newHeader->e_shoff=newOffset;

	if(DEBUG>1){
		printf("cleared step 1\n");
	}

	if(write(fdOut,elfH,len)<len){
		perror("Error in copy elf header file1 ");
		exit(errno);
	}
	
	char* start_copy;
	if(DEBUG>0){
		printf("[%-3s] %-23s %-6s %-6s\n","Nr","Name","Size","Offset");
	}
	for (i=0; i < header1->e_shnum ;i++){ 
		name=string_table1+sectionHeader1->sh_name;
		if(DEBUG>0){
			printf("[%3d] ",i);
			printf("%-23s ",name);
			printf("%06x ",sectionHeader1->sh_size);
			printf("%06x \n",sectionHeader1->sh_offset);
		}
		if(sectionHeader1->sh_type != NOBITS){ 
			start_copy=(char*)(start1+sectionHeader1->sh_offset);
			if(write(fdOut,start_copy,sectionHeader1->sh_size)<sectionHeader1->sh_size){
				perror("Error in copy file1 sections");
				if(DEBUG>0){
					printf("Error in copy file1 section %d \n",i);
				}
				exit(errno);
			}
			if((strcmp(name,".text")==0)||(strcmp(name,".data")==0)||(strcmp(name,".rodata")==0)){
				for (j=0; j < header2->e_shnum ;j++){
					coname=(char*)(string_table2+sectionHeader2->sh_name);
					if(strcmp(name,coname)==0){
						start_copy=(char*)(start2+sectionHeader2->sh_offset);
						if(write(fdOut,start_copy,sectionHeader2->sh_size)<sectionHeader2->sh_size){
							perror("Error in copy file2 sections");
							if(DEBUG>0){
								printf("Error in copy file2 section %d \n",j);
							}
							exit(errno);
						}
						break;
					}
					sectionHeader2++;
				}
				sectionHeader2=sectionHeader2-j;
			}
		}
		sectionHeader1++;
	}

	if(DEBUG>1){
		printf("cleared step 2\n");
	}	

	len=(header1->e_shentsize)*(header1->e_shnum);
	if(write(fdOut,elfSH,len)<len){
		perror("Error in copy elf SecHeader file1 ");
		exit(errno);
	}

	if(DEBUG>1){
		printf("cleared step 3\n");
	}

	free(elfH);
	free(elfSH);
}


int main(int argc, char **argv){
  int fdMain, fdSecondary, fdOut;
  struct stat fd_stat, fd_stat1;
  void *map_start, *map_start1;
  Elf32_Ehdr *header;
  /*Elf32_Off current_offset;
  int i;*/
  
  if (argc != 4){
    printf("Usage: %s file_name file_name output\n",argv[0]);
    exit(-1);
  }
  
  if ( (fdMain = open(argv[1], O_RDONLY)) < 0){
    perror("error in open:");
    exit(-2);
  }
  
  if ( (fdSecondary = open(argv[2], O_RDONLY)) < 0){
    perror("error in open:");
    exit(-3);
  }
  
  if (fstat(fdMain, &fd_stat) != 0){
    perror("stat failed:");
    exit(-4);
  }
  
  if (fstat(fdSecondary, &fd_stat1) != 0){
    perror("stat failed:");
    exit(-5);
  }
  
  if ( (map_start = mmap(0, fd_stat.st_size, 
			 PROT_READ | PROT_WRITE, MAP_PRIVATE,
			 fdMain, 0)) <0 ){
    perror("mmap failed:");
    exit(-6);
  }
  
  if ( (map_start1 = mmap(0, fd_stat1.st_size, 
			  PROT_READ | PROT_WRITE, MAP_PRIVATE,
			  fdSecondary, 0)) <0 ){
    perror("mmap failed:");
    exit(-7);
  }

  /* check if these are elf relocatables */
  if(!IsELF( (Elf32_Ehdr *)map_start)) {
    printf("file %s is not an elf relocatable\n",argv[1]);
    exit(-1);
  }
  
  if(!IsELF( (Elf32_Ehdr *)map_start1)) {
    printf("file %s is not an elf relocatable\n",argv[2]);
    exit(-1);
  }

  /* Mark all sections as non-merged */
  header = (Elf32_Ehdr *)map_start;
  MergeSectionIndices = (int *) malloc(sizeof(int)*(header->e_shnum+1));
  bzero (MergeSectionIndices, sizeof(int)*(header->e_shnum +1));
  
  /*  first, resolve any undefined symbols*/
  resolve_all_symbols(map_start,map_start1);

  if ( (fdOut = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0600)) < 0){
    perror("error in open:");
    exit(-3);
  }

  /* Write the output file */
  write_file(fdOut, map_start, map_start1);
  
  close(fdOut);
  printf("Done\n");

  return 0;
}