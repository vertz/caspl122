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
char* sym_table=0;
int symSize=0;
char* dupSymbol=0;
char ELF[4]={0x7f,0x45,0x4c,0x46};
int i,j,k,h,m,n;
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

void search(char* s,int l,Elf32_Sym  **symbolHeader1,char** sym_tableT,int *symSizeT,int number){
  Elf32_Sym  *symbolHeaderT=0;
  for(m=0;m<number;m++){
    if(m!=l){
        symbolHeaderT=symbolHeader1[m];
	for (j=0; j < symSizeT[m] ;j++){
	  if(symbolHeaderT->st_shndx != 0){
		if(strcmp( sym_tableT[m]+symbolHeaderT->st_name, s) == 0){
		    return;
		}
	  }
	  symbolHeaderT++;
	}
    }
  }
  
  printf("no missing symbols: FAILED %s\n",s);
  exit(-1);
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
	      i=symSize;
	  }
	  symbolHeader++;
    }
}

void check2(Elf32_Ehdr *header1,Elf32_Ehdr *header2,char* sym_table1,char* sym_table2,Elf32_Sym  *symbolHeader1,Elf32_Sym  *symbolHeader2,int symSize1,int symSize2){
    char *s1,*s2;
    Elf32_Sym  *symbolHeader_tmp2=symbolHeader2;
    for (i=0; i < symSize1 ;i++){
	  s1=sym_table1+symbolHeader1->st_name;
	 if (symbolHeader1->st_shndx != 0){
	    if(strcmp( s1, "") != 0){
	      for (j=0; j < symSize2 ;j++){
		s2=sym_table2+symbolHeader_tmp2->st_name;
		if (symbolHeader2->st_shndx != 0){
		  if(strcmp( s1, s2) == 0){
		      printf("duplicate check: FAILED %s \n",s1);
		      exit(-1);
		  }
		}
		symbolHeader_tmp2++;
	      }
	      symbolHeader_tmp2=symbolHeader2;
	    }
	  }
	  symbolHeader1++;
    }
}

void check3(int numb,Elf32_Sym  **symbolHeader1,char** sym_table1,int *symSize1){
    for(j=0;j<numb;j++){
	Elf32_Sym  *symbolHeader_tmp=symbolHeader1[j];
	for (i=0; i < symSize1[j] ;i++){
	  if(symbolHeader_tmp->st_shndx == 0){
	    if(strcmp( sym_table1[j]+symbolHeader_tmp->st_name, "") != 0){
		search(sym_table1[j]+symbolHeader_tmp->st_name,n,symbolHeader1,sym_table1,symSize1,numb);
	    }
	  }
	  symbolHeader_tmp++;
	}
    }
}

/*main check*/
void main_check(int num, char **arr){
	    struct stat f_stat;
	    Elf32_Ehdr *headers[num];
	    Elf32_Shdr *sectionHeaders[num];
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
		
		headers[k]=(Elf32_Ehdr *)maps[k];
		sectionHeaders[k]=(Elf32_Shdr *)(maps[k]+headers[k]->e_shoff);
		
		check0(headers[k],arr[k+1]);
		if(main_c<1){
		  check1(headers[k],sectionHeaders[k],maps[k]);
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

void inital_symbols(Elf32_Ehdr *header, Elf32_Shdr *sectionHeader, void* map){
    char* string_table=(char *)(map + (sectionHeader + header->e_shstrndx)->sh_offset );
    char* sName;
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
}

void duplicate_symbols(int num, char **arr){
	    struct stat f_stat;
	    Elf32_Ehdr *headers[num];
	    Elf32_Shdr *sectionHeaders[num];
	    Elf32_Sym  *symbolHeaders[num];
	    char* sym_tables[k];
	    int symSizes[k];
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
		
		headers[k]=(Elf32_Ehdr *)maps[k];
		sectionHeaders[k]=(Elf32_Shdr *)(maps[k]+headers[k]->e_shoff);
		
		inital_symbols(headers[k],sectionHeaders[k],maps[k]);
		sym_tables[k]=sym_table;
		symSizes[k]=symSize;
		symbolHeaders[k]=symbolHeader;
		
		k++;
	    }
	    
	    for(k=0;k<num;k++){
		for(h=k+1;h<num;h++){
		    check2(headers[k],headers[h],sym_tables[k],sym_tables[h],symbolHeaders[k],symbolHeaders[h],symSizes[k],symSizes[h]);
		}
	    }
	    
	    for(k=0;k<num;k++){
		close(files[k]);
		munmap(maps[k], size[k]);
	    }
	    
	    printf("duplicate check: PASSED\n");
}

void undefined_symbols(int num, char **arr){
	    struct stat f_stat;
	    Elf32_Ehdr *headers[num];
	    Elf32_Shdr *sectionHeaders[num];
	    Elf32_Sym  *symbolHeaders[num];
	    char* sym_tables[k];
	    int symSizes[k];
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
		
		headers[k]=(Elf32_Ehdr *)maps[k];
		sectionHeaders[k]=(Elf32_Shdr *)(maps[k]+headers[k]->e_shoff);
		
		inital_symbols(headers[k],sectionHeaders[k],maps[k]);
		sym_tables[k]=sym_table;
		symSizes[k]=symSize;
		symbolHeaders[k]=symbolHeader;
		
		k++;
	    }
	    
	    for(k=0;k<num;k++){
		check3(num,symbolHeaders,sym_tables,symSizes);
	    }
	    
	    for(k=0;k<num;k++){
		close(files[k]);
		munmap(maps[k], size[k]);
	    }
	    
	    printf("no missing symbols: PASSED\n");
}

/*void check3(int numb,Elf32_Sym  **symbolHeader1,char** sym_table1,int *symSize1,Elf32_Ehdr *header, Elf32_Shdr *sectionHeader, void* map,int h){*/

int main(int argc, char **argv) {
	if(argc>2){
	  
	    main_check(argc-1,argv);
	    duplicate_symbols(argc-1,argv);
	    undefined_symbols(argc-1,argv);
	    
	    return 0;
	}
	printf("Error: Not enough input arguments\n");
	return -1;
}