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

Elf32_Shdr *symbolInfo=0;
Elf32_Sym  *symbolHeader=0;
char ELF[4]={0x7f,0x45,0x4c,0x46};
int i,j,k;
int main_c=0;

void check0(Elf32_Ehdr *header,char* name){
    i=0;
    while(i<4){
	if (header->e_ident[i] != ELF[i]){
		printf ("Error: %s - Not an ELF file (wrong magic bytes at the start)\n",name);
		exit(-1);
	}
	i++;
    }
    if (header->e_ident[Machine] != m32){
	printf ("Error: %s - Not 32bit ELF file\n",name);
	exit(-1);
    }
}

void check1(Elf32_Ehdr *header, Elf32_Shdr *sectionHeader, void* map){
    char* string_table=(char *)(map + (sectionHeader + header->e_shstrndx)->sh_offset );
    char* sym_table=0;
    char* sName;
    int symSize=0;
    for (i=0; i < header->e_shnum ;i++){
	  sName=string_table+sectionHeader->sh_name;
	  if (strcmp(sName,".symtab")==0){
		  symbolHeader=(Elf32_Sym *)(map+sectionHeader->sh_offset);
		  symSize=(int)(sectionHeader->sh_size/sectionHeader->sh_entsize);
	  }
	  if(strcmp(sName,".strtab")==0){
		  sym_table=(char *)(map + sectionHeader->sh_offset);
	  }
	  sectionHeader++;
    }
    sectionHeader=(Elf32_Shdr *)(map + header->e_shoff);
    for (i=0; i < symSize ;i++){
	  if(strcmp( sym_table+symbolHeader->st_name, "main") == 0){
	      main_c=1;
	      break;
	  }
	  symbolHeader++;
    }
}

/*main check*/
void main_check(int num, char **arr){
	    struct stat f_stat;
	    Elf32_Ehdr *header[num];
	    Elf32_Shdr *sectionHeader[num];
	    int files[num];
	    void* maps[num];
	    size_t size[num];
	    k=0;
	    
	    while(k<num){
		if( (files[k]=open(arr[k+1], O_RDWR)) < 0 ) {
			  perror("Error in open");
			  exit(errno);
		}
		
		if( fstat(files[k], &f_stat) != 0 ) {
			  perror("Stat failed");
			  exit(errno);
		}
		
		size[k]=f_stat.st_size;
		if ((maps[k]=mmap(0, f_stat.st_size, PROT_READ , MAP_SHARED, files[k], 0)) < 0 ) {
			  perror("Map failed");
			  exit(errno);
		}
		
		header[k]=(Elf32_Ehdr *)maps[k];
		sectionHeader[k]=(Elf32_Shdr *)(maps[k]+header[k]->e_shoff);
		
		check0(header[k],arr[k+1]);
		if(main_c<1){
		  check1(header[k],sectionHeader[k],maps[k]);
		}
		
		k++;
	    }
	    
	    for(k=0;k<num;k++){
		close(files[k]);
		munmap(maps[k], size[k]);
	    }
	    
	    if(main_c>0){
	      printf("main check: PASSED\n");
	    }
	    else{
	      printf("main check: FAILED\n");
	      exit(-1);
	    }
}

int main(int argc, char **argv) {
	if(argc>2){
	  
	    main_check(argc-1,argv);
	    
	    return 0;
	}
	printf("Error: Not enough input arguments\n");
	return -1;
} 
